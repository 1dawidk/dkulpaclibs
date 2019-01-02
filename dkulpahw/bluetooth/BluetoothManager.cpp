#include <bluetooth/bluetooth.h>
#include <fcntl.h>
#include <dkulpaclibs/misc/debug/Log.h>
#include "BluetoothManager.h"

BluetoothManager::BluetoothManager() {
}

void BluetoothManager::start() {
    Thread::start();
}

void BluetoothManager::onStart() {
    Log::write(BM_TAG, "[ Preparing... ]");
    txBuff="";
    rxBuff="";
    rxBuffMutex= PTHREAD_MUTEX_INITIALIZER;
    txBuffMutex= PTHREAD_MUTEX_INITIALIZER;
    accBufferMutex= PTHREAD_MUTEX_INITIALIZER;


    open();
    Log::write(BM_TAG, "[ Started ]");
}

void BluetoothManager::onRun() {
    string tmp;
    string cmd;

    if(isCliConnected()){
        //Test input / read available data
        tmp+= testInput();

        if(tmp.length()>0) {
            pthread_mutex_lock(&rxBuffMutex);
            rxBuff+=tmp;
            if (rxBuff.length() > BM_RX_MAX_LEN) {
                rxBuff = rxBuff.substr(rxBuff.length() - BM_RX_MAX_LEN);
            }
            pthread_mutex_unlock(&rxBuffMutex);
        }

        Thread::pause(50);
    } else {
        connectFirstClient();
    }
}

void BluetoothManager::onStop() {
    close(cli);
    close(soc);
    sdp_close(session);
}

bool BluetoothManager::isCliConnected() {
    pthread_mutex_lock(&accBufferMutex);

    if(cli<=0 || soc==0) {
        pthread_mutex_unlock(&accBufferMutex);
        return false;
    }

    static struct sockaddr socadr={0};
    static socklen_t soclen=sizeof(socadr);
    bool ret= (getpeername(cli,&socadr,&soclen)==0);

    pthread_mutex_unlock(&accBufferMutex);

    return ret;
}

void BluetoothManager::write(string data) {
    data+="\n";

    pthread_mutex_lock(&txBuffMutex);
    send(cli, data.c_str(), data.length(), 0);
    pthread_mutex_unlock(&txBuffMutex);
}

string BluetoothManager::read() {
    string ret;

    pthread_mutex_lock(&rxBuffMutex);
    size_t end_pos= rxBuff.find_first_of('\n');
    if(end_pos!=string::npos){
        ret= rxBuff.substr(0, end_pos);
        rxBuff= rxBuff.substr(end_pos+1);
    } else {
        ret="";
    }
    pthread_mutex_unlock(&rxBuffMutex);

    return ret;
}

sdp_session_t *BluetoothManager::register_service(uint8_t ch) {
    uint32_t svc_uuid_int[]= { 0x00001101  , 0x00001000, 0x80000080, 0x5f9b34fb };
    const char *service_name = "dkulpaclibs Bluetooth Manager";
    const char *service_dsc = "Simple bluetooth manager with RF COMM profile";
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

    sdp_list_t *l2cap_list =        NULL,
            *rfcomm_list =          NULL,
            *root_list =            NULL,
            *proto_list =           NULL,
            *access_proto_list =    NULL,
            *svc_class_list =       NULL,
            *profile_list =         NULL;

        sdp_data_t *channel =       NULL;
    sdp_profile_desc_t profile;
    sdp_record_t record = { 0 };
    sdp_session_t *session = 0;

    // set the general service ID
    Log::write(BM_TAG, "Set the general service ID");
    sdp_uuid128_create(&svc_uuid, &svc_uuid_int);
    sdp_set_service_id(&record, svc_uuid);

    char str[256] = "";
    sdp_uuid2strn(&svc_uuid, str, 256);

    // set the service class
    Log::write(BM_TAG, "Set the service class");
    sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
    svc_class_list = sdp_list_append(0, &svc_class_uuid);
    sdp_set_service_classes(&record, svc_class_list);

    // set the Bluetooth profile information
    Log::write(BM_TAG, "Set the Bt profile info");
    sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
    profile.version = 0x0100;
    profile_list = sdp_list_append(0, &profile);
    sdp_set_profile_descs(&record, profile_list);

    // make the service record publicly browsable
    Log::write(BM_TAG, "Make the service record publicly browsable");
    sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
    root_list = sdp_list_append(0, &root_uuid);
    sdp_set_browse_groups(&record, root_list);

    // set l2cap information
    Log::write(BM_TAG, "Set l2cap info");
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    l2cap_list = sdp_list_append(0, &l2cap_uuid);
    proto_list = sdp_list_append(0, l2cap_list);

    // register the RFCOMM chanenel for RFCOMM sockets
    Log::write(BM_TAG, "Register the RFCOMM channel for RFCOMM socket");
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    channel = sdp_data_alloc(SDP_UINT8, &ch);
    rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(rfcomm_list, channel);
    sdp_list_append(proto_list, rfcomm_list);

    access_proto_list = sdp_list_append(0, proto_list);
    sdp_set_access_protos(&record, access_proto_list);

    // set the name, provider, and description
    Log::write(BM_TAG, "Set the name, provider and descriptor");
    sdp_set_info_attr(&record, service_name, service_prov, service_dsc);

    // connect to the local SDP server, register the service record,
    // and disconnect
    Log::write(BM_TAG, "Connect to the local SDP server, register the service record and disconnect");
    session = sdp_connect(anyAddr, localAddr, SDP_RETRY_IF_BUSY);
    sdp_record_register(session, &record, 0);

    // cleanup
    Log::write(BM_TAG, "Cleanup");
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
    remAddr = { 0 };    //Remote address

    //Create socket
    Log::write(BM_TAG, "Create socket");
    soc = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    Log::write(BM_TAG, "Set non blocking");
    fcntl(soc, F_SETFL, O_NONBLOCK);

    //Find free port
    Log::write(BM_TAG, "Find free port");
    int status;
    uint8_t ch;
    locAddr.rc_family = AF_BLUETOOTH;
    locAddr.rc_bdaddr = {0};
    for( ch = 1; ch <= 30; ch++ ) {
        locAddr.rc_channel = ch;
        status = bind(soc, (struct sockaddr *)&locAddr, sizeof( locAddr ) );
        if( status == 0 ) break;
    }

    //Register SPP service
    Log::write(BM_TAG, "Register SPP service");
    session= register_service(ch);

    //Put socket into listening mode, set timeout val
    Log::write(BM_TAG, "Put socket into listening mode");
    listen(soc, 1);
    ba2str(&locAddr.rc_bdaddr, buff);
}

void BluetoothManager::connectFirstClient(){
    char buf[20];

    socklen_t opt = sizeof(remAddr);
    cli = accept(soc, (struct sockaddr *)&remAddr, &opt);
    ba2str( &remAddr.rc_bdaddr, buf );

    if(cli>=0){
        Log::write(BM_TAG, "Client connected with cli= %d", cli);
        Log::write(BM_TAG, "Client's name %s", buf);

        pthread_mutex_lock(&rxBuffMutex);
        rxBuff="";
        pthread_mutex_unlock(&rxBuffMutex);

        pthread_mutex_lock(&txBuffMutex);
        txBuff="";
        pthread_mutex_unlock(&txBuffMutex);
    }
}


string BluetoothManager::testInput() {
    int bytes_read;
    char buf[256];

    bytes_read = ::read(cli, buf, 256);

    if( bytes_read > 0 ) {
        buf[bytes_read]=0;
        return string(buf);
    }

    return "";
}

void BluetoothManager::closeConnection() {
    //If client is open but not connected
    if(cli)
        close(cli);
}
