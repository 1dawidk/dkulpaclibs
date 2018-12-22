#ifndef MORTFIRMWARE_BLUETOOTHMANAGER_H
#define MORTFIRMWARE_BLUETOOTHMANAGER_H

#define BM_TAG "BluetoothManager"
#define BM_RX_MAX_LEN       2048

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
#include <dkulpaclibs/misc/Thread.h>

/**
 * USAGE:
 * 1) Create BluetoothManager object
 * 2) Use start() method to run manager
 * 3) For data write use write(string) method - Manager will write data
 *      through socket as soon as possible
 * 4) For data read user read() method - Manager will return data buffered
 *      from socket read buffer
 */

using namespace std;

typedef struct sockaddr_rc sockaddr_rc_t;

class BluetoothManager: public Thread {
public:
    explicit BluetoothManager();

    //Check if any client is connected
    bool isCliConnected();

    //Write / read data through RFComm server
    void write(string data);
    string read(int max_len);

    //Start Bluetooth Manager
    void start() override;

private:
    //Define Thread reqired methods
    void onStart() override;
    void onRun() override;
    void onStop() override;

    //Register new RFComm server
    sdp_session_t *register_service(uint8_t ch);
    //Open Bt device, socket and register server
    void open();

    //Try to connect to first available client
    void connectFirstClient();

    //Test input if there is any data available in read buffer
    string testInput();

    //Close client connection
    void closeConnection();



    //Bt adapter vars
    sdp_session_t* session; //Session
    int soc;                //Socket
    int cli;                //Connected client
    sockaddr_rc_t locAddr; //Local bt adapter's address
    sockaddr_rc_t remAddr; //Connected client address

//    struct timeval timeout;

    string rxBuff;          //Receive data buffer
    string txBuff;          //Transmit data buffer

    fd_set set;

    //Thread
    pthread_mutex_t rxBuffMutex; //Rx buffer mutex
    pthread_mutex_t txBuffMutex; //Tx buffer mutex
};


#endif //MORTFIRMWARE_BLUETOOTHMANAGER_H
