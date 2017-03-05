/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CoApPacket.cpp
 * Author: Hoang Hoi
 * 
 * Created on Ngày 04 tháng 3 năm 2017, 23:01
 */

#include <limits.h>

#include "CoApPacket.h"
#include "CoAp_conf.h"

unsigned int CoApPacket::messageID = 0;

boolean CoApPacket::init() {
    version = COAP_VERSION;
    type = COAP_NON_CONFIRMABLE;
    optionCount = 0;
    method = COAP_GET;
    queryIndex = 0;
    pathIndex = 0;
    index = 0;
    return true;
}

// Construct function

CoApPacket::CoApPacket() {
    init();
}

CoApPacket::CoApPacket(const CoApPacket& orig) {
}

CoApPacket::~CoApPacket() {
}

// Encode function

boolean CoApPacket::pushHeader() {
    index = 0;
    messageID++;
    packetBuffer[index++] = version << 6 | type << 4 | optionCount;
    packetBuffer[index++] = method;
    packetBuffer[index++] = highByte(messageID); // Message ID high byte
    packetBuffer[index++] = lowByte(messageID); // Message ID low byte
    return true;
}

boolean CoApPacket::pushFooter() {
    if (payload.length() > 0) {
        return true;
    }

    if (pathIndex == 0 && queryIndex == 0) {
        packetBuffer[index++] = 0xd1;
        packetBuffer[index++] = 0x0a;
    } else if (pathIndex == 0) {
        packetBuffer[index++] = 0xc1;
    } else {
        packetBuffer[index++] = 0x81;
    }

    packetBuffer[index++] = 0x02;
}

boolean CoApPacket::pushPaths() {
    char i;
    if (pathIndex == 0) {
        return true;
    }

    encode(COAP_URI_PATH_FIRST, paths[0]);
    for (i = 1; i < pathIndex; i++) {
        encode(COAP_URI_FOLLOW, paths[i]);
    }

    return true;
}

boolean CoApPacket::pushQueries() {
    char i;
    if (queryIndex == 0) {
        return true;
    }

    if (pathIndex == 0) {
        // No path.
        encode(COAP_URI_QUERY_FIRST_NO_PATH, queries[0]);
    } else {
        encode(COAP_URI_QUERY_FIRST, queries[0]);
    }

    for (i = 1; i < queryIndex; i++) {
        encode(COAP_URI_FOLLOW, queries[i]);
    }

    return true;
}

boolean CoApPacket::pushPayload() {
    char i;
    char len = payload.length();
    if (len == 0) {
        return true;
    }

    packetBuffer[index++] = COAP_PRE_PAYLOAD; // Payload Marker (0xFF)
    //payload
    for (i = 0; i < len; i++) {
        packetBuffer[i + index] = payload.charAt(i);
    }

    index += i;
}

boolean CoApPacket::encode(byte code, String &entity) {
    byte len = entity.length();
    byte i;
    if (len <= 12)
        packetBuffer[index++] = code | len;
    else {
        // lenght is greater than 12
        packetBuffer[index++] = code | COAP_URI_LONG;
        packetBuffer[index++] = len - 13; // the Option Value and indicates the Option Length minus 13.
    }

    if (pathIndex == 0 && code == COAP_URI_QUERY_FIRST_NO_PATH) {
        packetBuffer[index++] = 0x02;
    }

    for (i = 0; i < len; i++) {
        packetBuffer[i + index] = entity.charAt(i);
    }
    index += i;
}

// Set function

boolean CoApPacket::setVersion(byte version) {
    CoApPacket::version = version;
    return true;
}

boolean CoApPacket::setType(byte type) {
    CoApPacket::type = type;
    return true;
}

boolean CoApPacket::setMethod(byte method) {
    CoApPacket::method = method;
    return true;
}

boolean CoApPacket::setUri(String &uri) {
    char i = uri.indexOf('?');
    if (i != -1) {
        String pathString = uri.substring(0, i);
        String queryString = uri.substring(i + 1);
        CoApPacket::setMultiplePath(pathString);
        CoApPacket::setMultipleQuery(queryString);
    } else {
        CoApPacket::setMultiplePath(uri);
    }
    return true;
}

boolean CoApPacket::setUri(const char uri[]) {
    String uriString(uri);
    return CoApPacket::setUri(uriString);
}

boolean CoApPacket::setPath(String path) {
    if (pathIndex >= MAX_PATH) {
        return false;
    }

    paths[pathIndex] = path;
    pathIndex++;
    return true;
}

boolean CoApPacket::setMultiplePath(String &paths) {
    char i = 0;
    char j = paths.indexOf('/', i);
    char len = paths.length();

    while (i < len && j != -1) {
        setPath(paths.substring(i, j));
        i = j + 1;
        j = paths.indexOf('/', i);
    }

    if (i < len) {
        setPath(paths.substring(i));
    }

    return true;
}

boolean CoApPacket::setMultiplePath(const char paths[]) {
    String pathString(paths);
    return CoApPacket::setMultiplePath(pathString);
}

boolean CoApPacket::setQuery(String query) {
    if (queryIndex >= MAX_QUERY) {
        return false;
    }

    CoApPacket::queries[queryIndex] = query;
    queryIndex++;
    return true;
}

boolean CoApPacket::setMultipleQuery(String &querys) {
    char i = 0;
    char j = querys.indexOf('&', i);
    char len = querys.length();

    while (i < len && j != -1) {
        setQuery(querys.substring(i, j));
        i = j + 1;
        j = querys.indexOf('&', i);
    }

    if (i < len) {
        setQuery(querys.substring(i));
    }

    return true;
}

boolean CoApPacket::setMultipleQuery(const char queries[]) {
    String queriesString(queries);
    return CoApPacket::setMultipleQuery(queriesString);
}

boolean CoApPacket::setPayload(String &payload) {
    CoApPacket::payload = payload;
    return true;
}

boolean CoApPacket::setPayload(const char payload[]) {
    String payloadString(payload);
    return CoApPacket::setPayload(payloadString);
}

// Get function

byte CoApPacket::getConfig() {
    return version << 6 | type << 4 | optionCount;
}

byte CoApPacket::getMethod() {
    return method;
}

unsigned int CoApPacket::getMessageId() {
    return messageID;
}

String CoApPacket::getPathsString() {
    String pathString;
    char i;
    for (i = 0; i < pathIndex; i++) {
        pathString.concat(paths[i]);
        if (i < pathIndex - 1) {
            pathString.concat("/");
        }
    }

    return pathString;
}

String CoApPacket::getQueriesString() {
    String queriesString;
    char i;
    for (i = 0; i < queryIndex; i++) {
        queriesString.concat(queries[i]);
        if (i < queryIndex - 1) {
            queriesString.concat("&");
        }
    }

    return queriesString;
}

String CoApPacket::getUri() {
    String uri = getPathsString();
    String queriesString = getQueriesString();
    uri.concat("?");
    uri.concat(queriesString);
    return uri;
}

boolean CoApPacket::makePacket() {
    pushHeader();
    pushPaths();
    pushQueries();
    pushPayload();
    pushFooter();
    return true;
}

byte * CoApPacket::getPacket() {
    return packetBuffer;
}

uint16_t CoApPacket::getPacketLength() {
    return index;
}

// Print function

boolean CoApPacket::printPaths() {
    char i;
    for (i = 0; i < pathIndex; i++) {
        Serial.print("Path ");
        Serial.print(i, DEC);
        Serial.print(": ");
        Serial.println(CoApPacket::paths[i]);
    }
}

boolean CoApPacket::printQueries() {
    char i;
    for (i = 0; i < queryIndex; i++) {
        Serial.print("Query ");
        Serial.print(i, DEC);
        Serial.print(": ");
        Serial.println(CoApPacket::queries[i]);
    }
}

boolean CoApPacket::printUri() {
    char i;

    Serial.print("URI: ");
    for (i = 0; i < pathIndex; i++) {
        Serial.print(CoApPacket::paths[i]);
        if (i < pathIndex - 1) {
            Serial.print("/");
        }
    }

    Serial.print("?");
    for (i = 0; i < queryIndex; i++) {
        Serial.print(CoApPacket::queries[i]);
        if (i < queryIndex - 1) {
            Serial.print("&");
        }
    }

    Serial.println();
}

boolean CoApPacket::printPayload() {
    Serial.println(payload);
}

boolean CoApPacket::printPacketBuffer() {
    byte i;
    for (i = 0; i < index; i++) {
        if (packetBuffer[i] < 0x10) {
            Serial.print("0");
        }

        Serial.print(packetBuffer[i], HEX);
        Serial.print(" ");
    }
}
