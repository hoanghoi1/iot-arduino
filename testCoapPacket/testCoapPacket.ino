#include "CoApPacket.h"

CoApPacket packet;
CoApPacket packet1;

void setup() {
    Serial.begin(9600);
    delay(1000);
    packet.init();
    packet1.init();
}

void loop() {
    //    Serial.println(packet.getConfig(), BIN);
    //    delay(1000);

    //    packet.setType(1);
    //    Serial.println(packet.getConfig(), BIN);
    //    delay(1000);

    //    Serial.println(packet.getMessageId(), DEC);
    //    delay(1000);

    //    Serial.println(packet1.getMessageId(), DEC);
    //    delay(1000);

    //    packet.setMultiplePath("multiple/path");
    //    delay(1000);
    //    packet.setMultipleQuery("a=b&b=c");
    //    Serial.println("Path:");
    //    packet.printPaths();
    //    delay(1000);
    //    Serial.println("Queries:");
    //    packet.printQueries();
    //    delay(1000);


    //    String path = "multiple/path22";
    //    packet.setMultiplePath(path);
    //    delay(1000);
    //    Serial.println("Path:");
    //    packet.printPaths();
    //    delay(1000);

    //    String query = "ab=bc&bn=ry";
    //    packet.setMultipleQuery(query);
    //    delay(1000);
    //    Serial.println("Queries:");
    //    packet.printQueries();
    //    delay(1000);
    //    Serial.println();

    //    packet.setUri("abc//sed?ab=c&bc=ui");
    //    delay(1000);
    //    Serial.println("Path:");
    //    packet.printPaths();
    //    delay(1000);
    //    Serial.println("Queries:");
    //    packet.printQueries();
    //    delay(1000);
    //    packet.printUri();

    //    packet.setPayload("payload payload");
    //    delay(1000);
    //    packet.printPayload();
    //    delay(1000);

    //    packet.setUri("abc//sed?ab=c&bc=ui");
    //    delay(1000);
    //    Serial.println("Path:");
    //    Serial.print(packet.getPathsString());
    //    Serial.println();
    //    delay(1000);
    //    Serial.println("Queries:");
    //    Serial.print(packet.getQueriesString());
    //    Serial.println();
    //    delay(1000);
    //    Serial.println("Uri:");
    //    Serial.print(packet.getUri());
    //    Serial.println();
    //    delay(1000);
    //    Serial.println();

    //    packet.setUri("abdaidaifaifaifiafic/def/sed?ab=c&bc=ui");
    //    delay(1000);
    //    packet.makePacket();
    //    packet.printPacketBuffer();
    //    Serial.println();
    //    byte * packetBuff = packet.getPacket();
    //    uint16_t len = packet.getPacketLength();
    //    uint16_t i;
    //    for (i = 0; i < len; i++) {
    //        Serial.print(packetBuff[i], HEX);
    //    }
    //    Serial.println();
    //    delay(1000);
    //    while (1) {
    //    }
    
    
}
