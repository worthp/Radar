#include <Arduino.h>
#include <kld7.h>

KLD7::KLD7() {
    
    responseText[OK] = String("OK");
    responseText[UKNOWN_COMMAND] = String("UKNOWN_COMMAND");
    responseText[INVALID_PARAMETER_VALUE] = String("INVALID_PARAMETER_VALUE");
    responseText[INVALID_RPST_VERSION] = String("INVALID_RPST_VERSION");
    responseText[UART_ERROR] = String("UART_ERROR");
    responseText[SENSOR_BUSY] = String("SENSOR_BUSY");
    responseText[TIMEOUT] = String("TIMEOUT");
}

void KLD7::setSerialConnection (HardwareSerial *connection) {
    radarConnection = connection;
}

KLD7::RESPONSE KLD7::init() {
    byte buf[] = {'I', 'N', 'I', 'T',
                    0x04, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00};
    
    RESPONSE r;
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
    r = waitForResponse();
    addLog("KLD7 initialized.");
    getRadarParameters();

    return r;
}

KLD7::RESPONSE KLD7::waitForResponse() {
    char hdr[8];
    char response[1];
    radarConnection->readBytes(hdr, sizeof(hdr)); // TODO: do some sanity check
    radarConnection->readBytes(response, sizeof(response));

    if ((RESPONSE) response[0] != OK) {
        sprintf(logBuffer, "waitForResponse [%c%c%c%c%x%x%x%x%x]",
                    hdr[0], hdr[1], hdr[2], hdr[3], hdr[4], hdr[5], hdr[6], hdr[7], response[0]);
        addLog(logBuffer);
    }
    lastResponseCode = response[0];
    
    return ((RESPONSE) response[0]);
}

/**
 * 
 */
KLD7::RESPONSE KLD7::getRadarParameters() {
    RESPONSE r;
    byte buf[] = {'G', 'R', 'P', 'S',
                    0x04, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00};
    radarConnection->write(buf, sizeof(buf));

    r = waitForResponse();
    char hdr[8];
    
    radarConnection->readBytes(hdr, sizeof(hdr)); //TODO: sanity check
    
    radarConnection->readBytes(srpsBuffer, sizeof(srpsBuffer)); //TODO: sanity check
    //logSRPSBuffer();
        
    memcpy(software_version, srpsBuffer, sizeof(software_version));
    base_frequency = (uint8_t)srpsBuffer[19];
    maximum_speed = (uint8_t)srpsBuffer[20];
    maximum_range = (uint8_t)srpsBuffer[21];
    threshold_offset = (uint8_t)srpsBuffer[22];
    tracking_filter_type = (uint8_t)srpsBuffer[25];
    vibration_suppression = (uint8_t)srpsBuffer[24];
    minimum_detection_distance = (uint8_t)srpsBuffer[25];
    maximum_detection_distance = (uint8_t)srpsBuffer[26];
    
    minimum_detection_angle = (int8_t)srpsBuffer[27];
    maximum_detection_angle = (int8_t)srpsBuffer[28];

    minimum_detection_speed = (uint8_t)srpsBuffer[29];
    maximum_detection_speed = (uint8_t)srpsBuffer[30];
    detection_direction = (uint8_t)srpsBuffer[31];
    range_threshold = (uint8_t)srpsBuffer[32];

    angle_threshold = (int8_t)srpsBuffer[33];

    speed_threshold = (uint8_t)srpsBuffer[34];
    digital_output_1 = (uint8_t)srpsBuffer[35];
    digital_output_2 = (uint8_t)srpsBuffer[36];
    digital_output_3 = (uint8_t)srpsBuffer[37];
    hold_time = (srpsBuffer[39] << 8) | (srpsBuffer[38]);
    micro_detection_retrigger = (uint8_t)srpsBuffer[40];
    micro_detection_sensitivity = (uint8_t)srpsBuffer[41];

    return r;
}

/**
 * 
 */
KLD7::RESPONSE KLD7::updateRadarParameter(String name, String value) {
    RESPONSE r;

    if (name == "base_frequency") {//} = (uint8_t)srpsBuffer[19];
        srpsBuffer[19] = (uint8_t)atoi(value.c_str());
    } else if (name == "maximum_speed") {//} = (uint8_t)srpsBuffer[20];
        srpsBuffer[20] = (uint8_t)atoi(value.c_str());
    } else if (name == "maximum_range") {//} = (uint8_t)srpsBuffer[21];
        srpsBuffer[21] = (uint8_t)atoi(value.c_str());
    } else if (name == "threshold_offset") {//} = (uint8_t)srpsBuffer[22];
        srpsBuffer[22] = (uint8_t)atoi(value.c_str());
    } else if (name == "tracking_filter_type") {//} = (uint8_t)srpsBuffer[23];
        srpsBuffer[23] = (uint8_t)atoi(value.c_str());
    } else if (name == "vibration_suppression") {//} = (uint8_t)srpsBuffer[24];
        srpsBuffer[24] = (uint8_t)atoi(value.c_str());
    } else if (name == "minimum_detection_distance") {//} = (uint8_t)srpsBuffer[25];
        srpsBuffer[25] = (uint8_t)atoi(value.c_str());
    } else if (name == "maximum_detection_distance") {//} = (uint8_t)srpsBuffer[26];
        srpsBuffer[26] = (uint8_t)atoi(value.c_str());
    } else if (name == "minimum_detection_angle") {//} = (int8_t)srpsBuffer[27];
        srpsBuffer[27] = (uint8_t)atoi(value.c_str());
    } else if (name == "maximum_detection_angle") {//} = (int8_t)srpsBuffer[28];
        srpsBuffer[28] = (uint8_t)atoi(value.c_str());
    } else if (name == "minimum_detection_speed") {//} = (uint8_t)srpsBuffer[29];
        srpsBuffer[29] = (uint8_t)atoi(value.c_str());
    } else if (name == "maximum_detection_speed") {//} = (uint8_t)srpsBuffer[30];
        srpsBuffer[30] = (uint8_t)atoi(value.c_str());
    } else if (name == "detection_direction") {//} = (uint8_t)srpsBuffer[31];
        srpsBuffer[31] = (uint8_t)atoi(value.c_str());
    } else if (name == "range_threshold") {//} = (uint8_t)srpsBuffer[32];
        srpsBuffer[32] = (uint8_t)atoi(value.c_str());
    } else if (name == "angle_threshold") {//} = (int8_t)srpsBuffer[33];
        srpsBuffer[33] = (uint8_t)atoi(value.c_str());
    } else if (name == "speed_threshold") {//} = (uint8_t)srpsBuffer[34];
        srpsBuffer[34] = (uint8_t)atoi(value.c_str());
    } else if (name == "digital_output_1") {//} = (uint8_t)srpsBuffer[35];
        srpsBuffer[35] = (uint8_t)atoi(value.c_str());
    } else if (name == "digital_output_2") {//} = (uint8_t)srpsBuffer[36];
        srpsBuffer[36] = (uint8_t)atoi(value.c_str());
    } else if (name == "digital_output_3") {//} = (uint8_t)srpsBuffer[37];
        srpsBuffer[37] = (uint8_t)atoi(value.c_str());
    } else if (name == "xhold_time") {//} = (srpsBuffer[39] << 8) | (srpsBuffer[38]);
        uint16_t v;
        v = (uint16_t) atoi(value.c_str());
        srpsBuffer[38] = (uint8_t) v;
        srpsBuffer[39] = (uint8_t) (v >> 8);
    } else if (name == "micro_detection_retrigger") {//} = (uint8_t)srpsBuffer[40];
        srpsBuffer[40] = (uint8_t)atoi(value.c_str());
    } else if (name == "micro_detection_sensitivity") {//} = (uint8_t)srpsBuffer[41];
        srpsBuffer[41] = (uint8_t)atoi(value.c_str());
    } else {
        sprintf(logBuffer, "parameter name [%s] not recognized.", name.c_str());
        addLog(logBuffer);
        return KLD7::INVALID_PARAMETER_VALUE;
    }

    // send the changes
    r = setRadarParameters();
    getRadarParameters(); // just to make sure we are in sync
    return r;
}

KLD7::RESPONSE KLD7::setRadarParameters() {
    RESPONSE r;
    byte hdr[] = {'S', 'R', 'P', 'S', 0x2A, 0x00, 0x00, 0x00};
    uint8_t sendBuffer[50];
    

    memcpy(sendBuffer, hdr, sizeof(hdr));
    memcpy(&sendBuffer[sizeof(hdr)], srpsBuffer, sizeof(srpsBuffer));
    
    radarConnection->write(sendBuffer, sizeof(sendBuffer));
    
    r = waitForResponse();
    if (r != OK) {
        lastResponseCode = r;
        sprintf(logBuffer, "SRPS failed [%d][%s]", (int) lastResponseCode, responseText[lastResponseCode].c_str());
        addLog( logBuffer);
    }
    
    return r;
}

KLD7::RESPONSE KLD7::getNextFrameData() {
    RESPONSE r;
    byte buf[] = {'G', 'N', 'F', 'D',
                    0x04, 0x00, 0x00, 0x00,
                    0x08, 0x00, 0x00, 0x00};
    radarConnection->write(buf, sizeof(buf));

    r = waitForResponse();
    char hdr[8];
    char tdat[8];
    char s[128];
    
    radarConnection->readBytes(hdr, sizeof(hdr)); //TODO: sanity check

    if (hdr[4] > 0) { // target data
        radarConnection->readBytes(tdat, sizeof(tdat));
        sprintf(s, "TDAT data [%x%x%x%x%x%x%x%x]",
                tdat[0], tdat[1], tdat[2], tdat[3], tdat[4], tdat[5], tdat[6], tdat[7]);
        addLog(s);

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
    
    return r;
}

void KLD7::logSRPSBuffer() {
    sprintf(logBuffer, "SRPS data [%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x]",
                        srpsBuffer[0], srpsBuffer[1], srpsBuffer[2], srpsBuffer[3], srpsBuffer[4],
                        srpsBuffer[5], srpsBuffer[6], srpsBuffer[7], srpsBuffer[8], srpsBuffer[9],
                        srpsBuffer[10], srpsBuffer[11], srpsBuffer[12], srpsBuffer[13], srpsBuffer[14],
                        srpsBuffer[15], srpsBuffer[16], srpsBuffer[17], srpsBuffer[18], srpsBuffer[19],
                        srpsBuffer[20], srpsBuffer[21], srpsBuffer[22], srpsBuffer[23], srpsBuffer[24],
                        srpsBuffer[25], srpsBuffer[26], srpsBuffer[27], srpsBuffer[28], srpsBuffer[29],
                        srpsBuffer[30], srpsBuffer[31], srpsBuffer[32], srpsBuffer[33], srpsBuffer[34],
                        srpsBuffer[35], srpsBuffer[36], srpsBuffer[37], srpsBuffer[38], srpsBuffer[39],
                        srpsBuffer[40], srpsBuffer[41]);
    addLog(logBuffer);
    
}

void KLD7::addLog(String s) {
    addLog(s.c_str());
}

void KLD7::addLog(const char* s) {
    if (logCount == 20) {
        logCount = 0;
    }
    logs[logCount] = s;
    logCount += 1;
}

String KLD7::getLogs() {
    String s ;
    for (int i = 0; i < logCount; i++) {
        s.concat(logs[i]);
        s.concat("\n");
    }
    
    logCount = 0; // not sure if this is good.
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