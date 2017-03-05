#include <IPv6Stack.h>
#include <XBeeMACLayer.h>
#include <CoApPacket.h>

#define IS_INTERMEDIATE_ROUTER  (UIP_CONF_ROUTER && 0)// FOR INTERMEDIATE ROUTERS: 1, FOR NODES: 0 -> UIP_CONF_ROUTER MUST BE 1

#define IS_BORDER_ROUTER (UIP_CONF_ROUTER && !IS_INTERMEDIATE_ROUTER)

#define UDP_PORT 5683

#define SEND_INTERVAL		(10 * 1000)// 10 seconds

//This function calculates the RAM memory left
int mem(){
  uint8_t * heapptr, * stackptr;
  stackptr = (uint8_t *)malloc(4);
  heapptr = stackptr;
  free(stackptr);
  stackptr = (uint8_t *)(SP);
  return stackptr - heapptr;
}

//We need a specific object to implement the MACLayer interface (the virtual methods). In this case we use XBee but could be anyone capable of implementing that interface
XBeeMACLayer macLayer;

//We use a buffer in order to put the data that we will send
#define UDP_MAX_DATA_LEN 100
char udp_send[UDP_MAX_DATA_LEN];

//This will be the destination IP address
IPv6Address addr_dest(0xaa, 0xaa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

#if IS_BORDER_ROUTER
  //If we are configured to be a router, this is our prefix. Up to now we hardcode it here. Note: The aaaa::/64 prefix is the only known by the 6loPAN context so it will achieve a better compression.
  IPv6Address prefix(0xaa, 0xaa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
#endif

String coap_uri = "uri_test";       // CoAP Option: URI
String coap_query = "apikey=fd205ed589";// CoAP Option: Query. Emoncms: "apikey=<your_api_key>"
String coap_query_opt = "node=1";   // CoAP Option: Query (Optional)
String coap_payload_string = "";   // CoAP Payload.
CoApPacket coapPacket;

//We use a timer in order to send data if we do not have to respond to a message received.
IPv6Timer send_timer;

char udp_data[UDP_MAX_DATA_LEN];
int udp_data_length = 0;
IPv6Address sender_address;
uint16_t dest_port = UDP_PORT;

//changes the /64 prefix of the given address to the value of the prefix that was given to us by a router (if any). Returns true if we have an address with a prefix like this and false if we did not get a prefix and thus a global address
bool change_addr_prefix(IPv6Address &address){
  IPv6Address globalAddress;
  if (IPv6Stack::getGlobalPreferredAddress(globalAddress)){//we get our global address' prefix, given by our router, to send the response to the sender using global addresses (this will allow us to see routing)
    address.setAddressValue(globalAddress.getAddressValue(0), 0);
    address.setAddressValue(globalAddress.getAddressValue(1), 1);
    address.setAddressValue(globalAddress.getAddressValue(2), 2);
    address.setAddressValue(globalAddress.getAddressValue(3), 3);
    return true;
  }
  return false;
}

//This is what we will run as a result of receiving a UDP message. We first use the serial port to show what we received, wait half of the sending time and send the response to the sender that is, actually, the same message inverted
void udp_callback(char *data, int datalen, int sender_port, IPv6Address &sender_addr){
  Serial.println(mem());
  delay(100);
  //Show received dada
  data[datalen] = 0;
  Serial.println();
  Serial.println();
  Serial.print("Data received from ip: ");
  sender_addr.print();
  Serial.print(" port: ");
  Serial.print(sender_port);
  Serial.print(", data: ");
  //Serial.println(data); //could do it this way too, but we try the other way just to test
  //Show data
  while (IPv6Stack::udpDataAvailable()){
     Serial.print(IPv6Stack::readUdpData());
  }
  Serial.println();
        
    //In case we have been given a prefix by a router and we have validated our global address, change the address of the sender by changing its prefix. It is related to the same router, messages will go through it
  if (!change_addr_prefix(sender_addr)){
  IPv6Address globalAddress;
  globalAddress.print();
    #if IS_INTERMEDIATE_ROUTER // routers do not respond until they have their prefix (Border Routers never respond)
    return;
    #endif
  }
    
  delay(SEND_INTERVAL/2);//take SEND_INTERVAL/2 to respond

  Serial.println("Sending response..");
  Serial.println(); 
  delay(50); 
  int j;
  for(j=0; j<datalen; ++j){
    udp_send[datalen-1-j] = data[j]; 
  }
  addr_dest = sender_addr; //now if our message is not responded, it will be resent from the main loop to the same destination
  dest_port = sender_port;
  IPv6Stack::udpSend(sender_addr, sender_port, udp_send, datalen);
  delay(50);
  send_timer.restart();//each time we receive something we reset this timer so we should not send in the main loop as long as our message is responded
}

void setup(){  
  Serial.begin(9600);
  delay(1000);
  Serial.println();
  delay(100);
  
  // init network-device
  if (!IPv6Stack::initMacLayer(&macLayer)){
    Serial.println("CANNOT INITIALIZE XBEE MODULE.. CANNOT CONTINUE");
    while (true){};
  }
  
  //init IP Stack
  IPv6Stack::initIpStack();  
  Serial.println("IPV6 INITIALIZED");
  delay(100);
  
  //init UDP
  IPv6Stack::initUdp(UDP_PORT);
  Serial.println("UDP INITIALIZED");
  delay(100);
  
  //If Border Router, set prefix. If not, set a timer to send data
  #if !IS_BORDER_ROUTER
      send_timer.set(SEND_INTERVAL);
      Serial.println("SEND TIMER SET");
      delay(50);      
  #else
      //This line is added to specify our prefix    
      IPv6Stack::setPrefix(prefix, 64); 
  #endif /*IS_BORDER_ROUTER*/
  
  Serial.println("SETUP FINISHED!");
  delay(100);
}

void loop(){
  //Always need to poll timers in order to make the IPv6 Stack work
  IPv6Stack::pollTimers();  
  //If we are not a router (any kind), we also send messages
#if !UIP_CONF_ROUTER
  if (send_timer.expired()){
      send_timer.reset();
      Serial.println();
      Serial.print("MEMORY LEFT:");
      Serial.println(mem());
      Serial.println("Sending data..");
      delay(50);
      unsigned char payload[12];
      payload[0] = 0x40;
      payload[1] = 0x01;
      payload[2] = 0xc3;
      payload[3] = 0x00;
      payload[4] = 0xb5;
      payload[5] = 0x6c;
      payload[6] = 0x69;
      payload[7] = 0x6e;
      payload[8] = 0x6b;
      payload[9] = 0x31;
      payload[10] = 0xc1;
      payload[11] = 0x02;
      // char temp[8];

    // sprintf(temp, "%x", payload[0]);
      IPv6Stack::udpSend(addr_dest, dest_port, payload, 12);
      Serial.println("Sending CoAP data..");
      delay(200);
      IPv6Stack::sendCoap(addr_dest, coapPacket);
      
//      IPv6Stack::sendCoAPpacket(addr_dest, coap_uri , coap_query , coap_query_opt , coap_payload_string);
  }
#endif
  //We always check if we got anything. If we did, process that with the IPv6 Stack
  if (IPv6Stack::receivePacket()){
    Serial.println("receiver data");
#if !IS_BORDER_ROUTER
      //If we are not configured as border router, check if udp data is available and run the callback with it
      if (IPv6Stack::udpDataAvailable()){
        udp_data_length = IPv6Stack::getUdpDataLength();
        IPv6Stack::getUdpData(udp_data);
        IPv6Stack::getUdpSenderIpAddress(sender_address);
        udp_callback(udp_data, udp_data_length, IPv6Stack::getUdpSenderPort(), sender_address);
      }
#endif
  }
  delay(100);
}

/*---------------------------------------------------------------------------*/
