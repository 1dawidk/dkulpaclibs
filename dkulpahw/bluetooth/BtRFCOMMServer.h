#ifndef MORTFIRMWARE_BLUETOOTHADAPTER_H
#define MORTFIRMWARE_BLUETOOTHADAPTER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <pthread.h>

typedef struct sockaddr_rc sockaddr_rc_t;

class BtRFCOMMServer {

public:
    int Open();
    void WaitForClient();

private:
    int soc;
    int cli;
    sockaddr_rc_t loc_addr;
    sockaddr_rc_t rem_addr;

    int cliWait();
    int waiting;

    pthread_t waitThread;
    static void* threadExec(void *context);
};


#endif //MORTFIRMWARE_BLUETOOTHADAPTER_H
