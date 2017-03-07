#include <IPv6Stack.h>
#include <XBeeMACLayer.h>
#include <CoApPacket.h>

#define UDP_PORT 5683
#define SEND_INTERVAL  (10 * 1000)// 10 seconds
#define UDP_MAX_DATA_LEN 100

//This function calculates the RAM memory left

int mem() {
    uint8_t * heapptr, * stackptr;
    stackptr = (uint8_t *) malloc(4);
    heapptr = stackptr;
    free(stackptr);
    stackptr = (uint8_t *) (SP);
    return stackptr - heapptr;
}

XBeeMACLayer macLayer;
IPv6Address addr_dest(0xaa, 0xaa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
IPv6Timer send_timer;
char udp_data[UDP_MAX_DATA_LEN];
int udp_data_length = 0;
IPv6Address sender_address;

void udp_callback(char *data, int datalen, int sender_port, IPv6Address &sender_addr) {
    //Show received dada
    data[datalen] = 0;
    Serial1.println();
    Serial1.print("Data received from ip: ");
    sender_addr.print();
    Serial1.print(" port: ");
    Serial1.print(sender_port);
    Serial1.print(", data: ");
    //Show data
    while (IPv6Stack::udpDataAvailable()) {
        Serial1.print(IPv6Stack::readUdpData());
    }
    Serial1.println();
}

void setup() {
    Serial.begin(38400);
    Serial1.begin(38400);
    delay(1000);
    Serial1.println();
    delay(100);

    // init network-device
    if (!IPv6Stack::initMacLayer(&macLayer)) {
        Serial1.println("CANNOT INITIALIZE XBEE MODULE.. CANNOT CONTINUE");
        while (true) {
        };
    }

    //init IP Stack
    IPv6Stack::initIpStack();
    Serial1.println("IPV6 INITIALIZED");
    delay(100);

    //init UDP
    IPv6Stack::initUdp(UDP_PORT);
    Serial1.println("UDP INITIALIZED");
    delay(100);

    //  //If Border Router, set prefix. If not, set a timer to send data
    send_timer.set(SEND_INTERVAL);
    Serial1.println("SEND TIMER SET");
    delay(50);

    Serial1.println("SETUP FINISHED!");
    delay(100);
}

void loop() {
    //Always need to poll timers in order to make the IPv6 Stack work
    IPv6Stack::pollTimers();
    //If we are not a router (any kind), we also send messages
    if (send_timer.expired()) {
        Serial1.print("Mem left: ");
        Serial1.print(mem());
        Serial1.println();
        send_timer.reset();
        Serial1.println("Sending CoAP data..");
        IPv6Stack::get(addr_dest, "test/sub?a=b&c=d");
        IPv6Address globalAddress;
        globalAddress.print();
    }
    //We always check if we got anything. If we did, process that with the IPv6 Stack
    if (IPv6Stack::receivePacket()) {
        Serial1.println();
        Serial1.println("Receiver data");
        //If we are not configured as border router, check if udp data is available and run the callback with it
        if (IPv6Stack::udpDataAvailable()) {
            Serial1.println("Get length...");
            udp_data_length = IPv6Stack::getUdpDataLength();
            Serial1.println("Get data...");
            IPv6Stack::getUdpData(udp_data);
            Serial1.println("Get address...");
            IPv6Stack::getUdpSenderIpAddress(sender_address);
            Serial1.println("Handle callback...");
            udp_callback(udp_data, udp_data_length, IPv6Stack::getUdpSenderPort(), sender_address);
        }
    }

    delay(100);
}
