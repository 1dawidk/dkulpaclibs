#ifndef MORTFIRMWARE_BLUETOOTHMANAGER_H
#define MORTFIRMWARE_BLUETOOTHMANAGER_H

#define BM_TAG "Bluetooth"

#include <dkulpaclibs/misc/Thread.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>


using namespace std;

typedef struct sockaddr_rc sockaddr_rc_t;

class BluetoothManager: public Thread {
public:
    explicit BluetoothManager();

    bool isCliConnected();
    void write(const char* data, int len);

    void onStop() override;

protected:
    void onRun() override;

private:

    sdp_session_t* session;
    int soc;
    int cli;
    sockaddr_rc_t loc_addr;
    sockaddr_rc_t rem_addr;

    string cmdBuf;

    fd_set set;
    struct timeval timeout;

    sdp_session_t *register_service(uint8_t ch);
    void open();
    void waitForClient();
    string readCliBuff();
    string retrieveCmd(string msg);
};


#endif //MORTFIRMWARE_BLUETOOTHMANAGER_H
