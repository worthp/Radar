#include <Arduino.h>
#include <kld7.h>


void KLD7::setSerialConnection (HardwareSerial *connection) {
    radarConnection = connection;
}

KLD7::RESPONSE KLD7::init() {
    byte buf[] = {'I', 'N', 'I', 'T',
                    0x04, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00};
    
    char s[128];
    for (int i = 0; i < 3; i++) {

        //radarConnection->write(buf, sizeof(buf));
        // clean whatever may be in the rx buffer. sometimes kld7 puts 
        // responses before we talk. probably some line noise or something
        // but it's not in the spec
        char junk[1];
        while ((radarConnection->readBytes(junk, 1)) == 1) {
        }
    }

    radarConnection->write(buf, sizeof(buf));
    char hdr[8];
    char response[1];
    radarConnection->readBytes(hdr, sizeof(hdr)); // TODO: do some sanity check
    radarConnection->readBytes(response, sizeof(response));

    sprintf(s, "[\"%c%c%c%c\",\"%x%x%x%x\", \"%x\"]",
                hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6], hdr[7], response[0]);
    setStatus(s);

    return OK;
}

KLD7::RESPONSE KLD7::getRadarParameters() {
    return OK;
}

KLD7::RESPONSE KLD7::getNextFrameData() {
    byte buf[] = {'G', 'N', 'F', 'D',
                    0x04, 0x00, 0x00, 0x00,
                    0x08, 0x00, 0x00, 0x00};
    radarConnection->write(buf, sizeof(buf));

    char hdr[8];
    char response[1];
    radarConnection->readBytes(hdr, sizeof(hdr));
    radarConnection->readBytes(response, sizeof(response));

    char s[128];
    
    char tdat[8];
    radarConnection->readBytes(hdr, sizeof(hdr)); //TODO: sanity check
    
    sprintf(s, "[\"%c%c%c%c\",\"%x%x%x%x\"]",
                hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6], hdr[7]);
    setStatus(s);

    if (hdr[4] > 0) { // target data
        radarConnection->readBytes(tdat, sizeof(tdat));
        sprintf(s, "\"%x%x %x%x %x%x %x%x\"]",
                tdat[0], tdat[1], tdat[2], tdat[3], tdat[4], tdat[5], tdat[6], tdat[7]);

        // metric units. speed,angle,magnitude are scaled x 100
        distance = tdat[1] << 8 | tdat[0];
        speed = (tdat[3] << 8 | tdat[2]);
        angle = (tdat[5] << 8 | tdat[4]);
        magnitude = (tdat[7] << 8 | tdat[6]);
        
        addTDATReading(distance, speed, angle, magnitude);
        stats.nonZeroTDATCount += 1;
        stats.lastNonZeroReadingTime = millis();
    } else {
        // no need to waste space on zeroes
        stats.zeroTDATCount += 1;
        stats.lastZeroReadingTime = millis();
    }
    
    return OK;
}

void KLD7::setStatus(String s) {
    setStatus(s.c_str());
}
void KLD7::setStatus(const char* s) {
    if (status.length() > 2048) {
        status.clear();
        status.concat(s);
    } else {
        status.concat(",");status.concat(s);
    }
}

String KLD7::getStatus() {
    String s = "";
    s.concat("{");
    s.concat("\"time\":\"0900\",");
    s.concat("\"data\":[");
    s.concat(status);
    s.concat("]");
    s.concat("}");
    return s;
}

void KLD7::addTDATReading(uint16_t distance, int16_t speed, int16_t angle, uint16_t magnitude)
{
    if (tDataWriteIndex == sizeof(tData)) {
        tDataWriteIndex = 0;
    }
    
    tData[tDataWriteIndex].time = millis();
    tData[tDataWriteIndex].distance = distance;
    tData[tDataWriteIndex].speed = speed;
    tData[tDataWriteIndex].angle = angle;
    tData[tDataWriteIndex].magnitude = magnitude;
    
    stats.minDistance = min(distance, stats.minDistance);
    stats.maxDistance = max(distance, stats.maxDistance);
    stats.minSpeed = min(speed, stats.minSpeed);
    stats.maxSpeed = max(speed, stats.maxSpeed);
    stats.minAngle = min(angle, stats.minAngle);
    stats.maxAngle = max(angle, stats.maxAngle);
    stats.minMagnitude = min(magnitude, stats.minMagnitude);
    stats.maxMagnitude = max(magnitude, stats.maxMagnitude);
}
