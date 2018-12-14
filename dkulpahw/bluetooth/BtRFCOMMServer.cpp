#include "BtRFCOMMServer.h"

int BtRFCOMMServer::Open() {
    rem_addr = { 0 };    //Remote address

    //Create socket
    soc = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    //Bind socket to port 1 of the first available local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = {0, 0, 0, 0, 0, 0};
    loc_addr.rc_channel = (uint8_t) 1;
    bind(soc, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    //Put socket into listening mode
    listen(soc, 1);

    return 0;
}

void BtRFCOMMServer::WaitForClient() {
    if(!waiting) {
        waiting = true;
        pthread_create(&waitThread, nullptr, &BtRFCOMMServer::threadExec, this);
    }
}

int BtRFCOMMServer::cliWait() {
    socklen_t opt = sizeof(rem_addr);
    cli = accept(soc, (struct sockaddr *)&rem_addr, &opt);

    return 0;
}

void *BtRFCOMMServer::threadExec(void *context) {
    ((BtRFCOMMServer*)context)->cliWait();
    ((BtRFCOMMServer*)context)->waiting=false;

    return 0;
}