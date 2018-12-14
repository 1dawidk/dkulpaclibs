#include <bluetooth/bluetooth.h>
#include "BluetoothManager.h"

BluetoothManager::BluetoothManager() {
}

void BluetoothManager::onStop() {
    close(cli);
    close(soc);
    sdp_close(session);
    runThread=false;
}

bool BluetoothManager::isCliConnected() {
    if(cli==0 || soc==0)
        return false;

    static struct sockaddr socadr={0};
    static socklen_t soclen=sizeof(socadr);

    return (getpeername(cli,&socadr,&soclen)==0);
}

void BluetoothManager::write(const char *data, int len) {
    if(isCliConnected()){
        send(cli, data, len, 0);
    }
}


void BluetoothManager::onRun() {
    runThread=true;
    string tmp;
    string cmd;

    cmdBuf="";

    while(runThread){
        if(isCliConnected()){
            tmp= readCliBuff();
            cmd= retrieveCmd(tmp);

            if(!cmd.empty()){

            }
            usleep(200000);
        } else {
            if(cli!=0)
                close(cli);
            if(soc!=0)
                close(soc);

            sdp_close(session);
            open();
            waitForClient();
        }
    }
}

sdp_session_t *BluetoothManager::register_service(uint8_t ch) {
    uint32_t svc_uuid_int[]= { 0x000001101  , 0x00001000, 0x80000080, 0x5f9b34fb };
    const char *service_name = "Mort BT Commands Protocol";
    const char *service_dsc = "Used to controll Mort Bot";
    const char *service_prov = "Dawid Kulpa";

    bdaddr_t *anyAddr=new bdaddr_t();
    bdaddr_t *localAddr=new bdaddr_t();
    localAddr->b[0]=0;
    localAddr->b[1]=0;
    localAddr->b[2]=0;
    localAddr->b[3]=0xff;
    localAddr->b[4]=0xff;
    localAddr->b[5]=0xff;

    uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid,
            svc_class_uuid;
    sdp_list_t *l2cap_list = 0,
            *rfcomm_list = 0,
            *root_list = 0,
            *proto_list = 0,
            *access_proto_list = 0,
            *svc_class_list = 0,
            *profile_list = 0;
    sdp_data_t *channel = 0;
    sdp_profile_desc_t profile;
    sdp_record_t record = { 0 };
    sdp_session_t *session = 0;

    // set the general service ID
    sdp_uuid128_create(&svc_uuid, &svc_uuid_int);
    sdp_set_service_id(&record, svc_uuid);

    char str[256] = "";
    sdp_uuid2strn(&svc_uuid, str, 256);

    // set the service class
    sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
    svc_class_list = sdp_list_append(0, &svc_class_uuid);
    sdp_set_service_classes(&record, svc_class_list);

    // set the bluetooth profile information
    sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
    profile.version = 0x0100;
    profile_list = sdp_list_append(0, &profile);
    sdp_set_profile_descs(&record, profile_list);

    // make the service record publicly browsable
    sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
    root_list = sdp_list_append(0, &root_uuid);
    sdp_set_browse_groups(&record, root_list);

    // set l2cap information
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    l2cap_list = sdp_list_append(0, &l2cap_uuid);
    proto_list = sdp_list_append(0, l2cap_list);

    // register the RFCOMM channel for RFCOMM sockets
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    channel = sdp_data_alloc(SDP_UINT8, &ch);
    rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(rfcomm_list, channel);
    sdp_list_append(proto_list, rfcomm_list);

    access_proto_list = sdp_list_append(0, proto_list);
    sdp_set_access_protos(&record, access_proto_list);

    // set the name, provider, and description
    sdp_set_info_attr(&record, service_name, service_prov, service_dsc);

    // connect to the local SDP server, register the service record,
    // and disconnect
    session = sdp_connect(anyAddr, localAddr, SDP_RETRY_IF_BUSY);
    sdp_record_register(session, &record, 0);

    // cleanup
    sdp_data_free(channel);
    sdp_list_free(l2cap_list, 0);
    sdp_list_free(rfcomm_list, 0);
    sdp_list_free(root_list, 0);
    sdp_list_free(access_proto_list, 0);
    sdp_list_free(svc_class_list, 0);
    sdp_list_free(profile_list, 0);

    return session;
}

void BluetoothManager::open() {
    char buff[20];
    rem_addr = { 0 };    //Remote address

    //Create socket
    soc = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    //Find free port
    int ch, status;
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = {0};
    for( ch = 1; ch <= 30; ch++ ) {
        loc_addr.rc_channel = ch;
        status = bind(soc, (struct sockaddr *)&loc_addr, sizeof( loc_addr ) );
        if( status == 0 ) break;
    }

    //Register SPP service
    session= register_service(ch);

    //Put socket into listening mode, set timeout val
    listen(soc, 1);
    ba2str(&loc_addr.rc_bdaddr, buff);
}

void BluetoothManager::waitForClient() {
    char buf[20];

    socklen_t opt = sizeof(rem_addr);
    cli = accept(soc, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );
    //supervisor->WriteTalkFile(TALKFILE_UI_KEY, BM_TAG, "cout", string("New client: ")+buf);
}

string BluetoothManager::readCliBuff() {
    int bytes_read;
    char buf[256];

    FD_ZERO(&set);
    FD_SET(cli, &set);
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if(select(cli + 1, &set, NULL, NULL, &timeout)==0)
        return "";

    bytes_read = read(cli, buf, 20);
    if( bytes_read > 0 ) {
        buf[bytes_read]=0;
        return string(buf);
    }

    return "";
}

string BluetoothManager::retrieveCmd(string msg) {
    string cmd;

    for(int i=0; i<msg.length(); i++){
        if(msg[i]=='\n') {
            cmd = cmdBuf;
            cmdBuf = "";
        } else if(msg[i]=='~'){
            cmdBuf="";
        } else {
            cmdBuf+=msg[i];
        }
    }

    return cmd;
}
