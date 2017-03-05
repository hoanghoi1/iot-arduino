/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CoApPacket.h
 * Author: Hoang Hoi
 *
 * Created on Ngày 04 tháng 3 năm 2017, 23:01
 */

#ifndef COAPPACKET_H
#define COAPPACKET_H

#include <Arduino.h>

// define coap
#define UIP_IPH_LEN    40   /* Size of IPv6 header */
#define UIP_UDPH_LEN    8    /* Size of UDP header */
#define UIP_IPUDPH_LEN (UIP_UDPH_LEN + UIP_IPH_LEN)    /* Size of IP + UDP header */
#define UIP_BUFSIZE  160 // The size of the uIP packet buffer.
#define UIP_UDP_PAYLOAD_MAX_BUFSIZE (UIP_BUFSIZE - UIP_IPUDPH_LEN)
#define COAP_PACKET_SIZE UIP_UDP_PAYLOAD_MAX_BUFSIZE
#define MAX_QUERY 5
#define MAX_PATH 5

class CoApPacket {
private:
    byte index;
    byte version;
    byte type;
    byte optionCount;
    byte method;
    static unsigned int messageID;
    String paths[MAX_PATH];
    unsigned int pathIndex;
    String queries[MAX_QUERY];
    unsigned int queryIndex;
    String payload;
    byte packetBuffer[COAP_PACKET_SIZE];

    boolean pushHeader();
    boolean pushQueries();
    boolean pushPaths();
    boolean pushPayload();
    boolean encode(byte code, String &entity);
    boolean pushFooter();

public:
    boolean setVersion(byte version);
    boolean setType(byte type);
    boolean setMethod(byte method);

    boolean setPath(String path);
    boolean setMultiplePath(String &paths);
    boolean setMultiplePath(const char paths[]);
    boolean setUri(String &uri);
    boolean setUri(const char paths[]);
    boolean setQuery(String query);
    boolean setMultipleQuery(String &queries);
    boolean setMultipleQuery(const char queries[]);
    boolean setPayload(String &payload);
    boolean setPayload(const char payload[]);

    boolean init();
    CoApPacket();
    CoApPacket(const CoApPacket & orig);
    virtual ~CoApPacket();

    byte getConfig();
    byte getMethod();
    unsigned int getMessageId();
    String getPathsString();
    String getQueriesString();
    String getUri();
    boolean makePacket();
    byte * getPacket();
    uint16_t getPacketLength();

    boolean printPaths();
    boolean printQueries();
    boolean printUri();
    boolean printPayload();
    boolean printPacketBuffer();

};

#endif /* COAPPACKET_H */

