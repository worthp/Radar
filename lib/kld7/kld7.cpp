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
    
    distance = 0;
    speed = 0;
    angle = 0;
    magnitude = 0;
    char tdat[8];
    radarConnection->readBytes(hdr, sizeof(hdr)); //TODO: sanity check
    
    sprintf(s, "[\"%c%c%c%c\",\"%x%x%x%x\"]",
                hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6], hdr[7]);
    setStatus(s);

    if (hdr[4] > 0) { // target data
        radarConnection->readBytes(tdat, sizeof(tdat));
        sprintf(s, "\"%x%x %x%x %x%x %x%x\"]",
                tdat[0], tdat[1], tdat[2], tdat[3], tdat[4], tdat[5], tdat[6], tdat[7]);
        //setStatus(s);

        distance = tdat[1] << 8 | tdat[0];
        speed = tdat[3] << 8 | tdat[2];
        angle = tdat[5] << 8 | tdat[4];
        magnitude = tdat[7] << 8 | tdat[6];
    }
    
    sprintf(s, "[\"metrics\",\"%d\",\"%d\",\"%d\",\"%d\"]", distance, speed, angle, magnitude);
    setStatus(s);
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

