#include <HardwareSerial.h>
#include <Arduino.h>
#include "uip.h"
#include "arduino_debug.h"

void arduino_debug_init() {
    #if ARDUINO_DEBUG
    Serial1.begin(38400);
    #endif
}

void arduino_debug(char* s) {
    #if ARDUINO_DEBUG
    Serial1.println(s);
    delay(20);
    #endif
}

void arduino_debug_hex(int b) {
  #if ARDUINO_DEBUG
    Serial1.println(b, HEX);
    #endif
}

void arduino_debug_dec(long b) {
  #if ARDUINO_DEBUG
    Serial1.println(b, DEC);
    #endif
}
    
void arduino_debug_address(uip_ip6addr_t* address) {
  #if ARDUINO_DEBUG
    int i;
    for (i=0; i<16; i++) {    
      if (address->u8[i]<0xf) 
        Serial1.print("0");   
      Serial1.print(address->u8[i], HEX);
      if (((i%2) == 1) && (i != 15))
        Serial1.print(":");
    }    
    Serial1.println();  
    #endif
}

void arduino_debug_lladdr(uip_lladdr_t* address){
  #if ARDUINO_DEBUG
    int i;
    Serial1.print(address->addr[0], HEX);
    for (i=1; i< UIP_LLADDR_LEN; i++) {  
      Serial1.print(":");
      Serial1.print(address->addr[i], HEX);
    }
    Serial1.println();  
    #endif
}

    
    
