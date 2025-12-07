#include <Arduino.h>
#include <kld7.h>


void KLD7::setSerialConnection (HardwareSerial *connection) {
    radarConnection = connection;
}
KLD7::RESPONSE KLD7::init() {
    radarConnection->println("KDL7.init()");
    test();
    return OK;
}

KLD7::RESPONSE KLD7::getRadarParameters() {
    return OK;
}

KLD7::RESPONSE KLD7::getNextFrameData() {
    radarConnection->printf("distance[%d] speed[%d] angle[%d] magnitude[%d]\n", distance, speed, angle, magnitude);
    return OK;
}

KLD7::RESPONSE KLD7::getResponse() {
    return OK;
}

void KLD7::test() {
    uint8_t header[4] = {0x47, 0x4e, 0x46, 0x44};
    uint8_t length[4] {0x04, 0x00, 0x00, 0x00};
    uint32_t payloadSize = 0;

    uint8_t bu[8] = {0x50, 0x00, 0x97, 0xff, 0x2f, 0x07, 0x15, 0x18};

    distance = 0;
    speed = 0;
    angle = 0;
    magnitude = 0;

    distance = (bu[1] << 8) | (bu[0]);
    speed = (bu[3] << 8) | (bu[2]);
    angle = (bu[5] << 8) | (bu[4]);
    magnitude = (bu[7] << 8) | (bu[6]);

    if (!strncmp((char *) header, "GNFD", 4)) {
        payloadSize = (length[3] << 24) | (length[2] << 16) | (length[1] << 8) | (length[0]);
        if (payloadSize == 4) {
            printf("distance[%d] speed[%d] angle[%d] magnitude[%d]\n", distance, speed, angle, magnitude);
        } else {
            printf("got work to do on payload size\n");
        }
    } else {
        printf("header not recognized [%c%c%c%c]\n",  header[0], header[1], header[2], header[3]);
    }
}

