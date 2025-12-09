#include <cstdint>
#ifndef __KLD7_H
#define __KLD7_H

class KLD7 {
    
    // Radar Parameter 'structure' commands GRPS/SRPS deals with these elements
    char software_version[19]; // STRING,19,Zero-terminated String,K-LD7_APP-RFB-XXXX
    uint8_t base_frequency; // ,UINT8,1,0 = Low, 1 = Middle, 2 = High,1 = Middle
    uint8_t maximum_speed; //UINT8,1,0 = 12.5km/h, 1 = 25km/h, 2 = 50km/h, 3 = 100km/h,1 = 25km/h
    uint8_t maximum_range; //UINT8,1,0 = 5m, 1 = 10m, 2 = 30m, 3 = 100m,1 = 10m
    uint8_t threshold_offset; // UINT8,1,10-60 dB,30 dB
    uint8_t tracking_filter_type; // UINT8,1,0 = Standard, 1 = Fast detection, 2 = Long visibility,0 = Standard
    uint8_t vibration_suppression; // UINT8,1,0-16, 0 = No suppression, 16 = High suppression,3 = Medium suppression
    uint8_t minimum_detection_distance; //UINT8,1,0 – 100% of range setting,0%
    uint8_t maximum_detection_distance; //UINT8,1,0 – 100% of range setting,50%
    int8_t minimum_detection_angle; //INT8,1,-90° to +90°,-90°
    int8_t maximum_detection_angle; // INT8,1,-90° to +90°,+90°
    uint8_t minimum_detection_speed; // UINT8,1,0 – 100% of speed setting,0%
    uint8_t maximum_detection_speed; // UINT8,1,0 – 100% of speed setting,100%
    uint8_t detection_direction; // UINT8,1,0 = Approaching, 1 = Receding, 2 = Both,2 = Both
    uint8_t range_threshold; // UINT8,1,0 – 100% of range setting,10%
    uint8_t angle_threshold; // INT8,1,-90° to +90°,0°
    uint8_t speed_threshold; // UINT8,1,0 – 100% of speed setting,50%
    uint8_t digital_output_1; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,0 = Direction
    uint8_t digital_output_2; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,1 = Angle
    uint8_t digital_output_3; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,2 = Range
    uint16_t hold_time; // UINT16,2,1 – 7200s (1s – 2h), 120s
    uint8_t micro_detection_retrigger; // UINT8,1,0 = Off, 1 = Retrigger,0 = Off
    uint8_t micro_detection_sensitivity; // UINT8,1,0-9, 0=Min. sensitivity, 9=Max. sensitvity,4 = Medium sensitivity
                                         
    HardwareSerial *radarConnection;
public:
    enum RESPONSE {
        OK,
        UKNOWN_COMMAND,
        INVALID_PARAMETER_VALUE,
        INVALID_RPST_VERSION,
        UART_ERROR,
        SENSOR_BUSY,
        TIMEOUT        
    };
    
    struct tDataEntry{
        long time;
        uint16_t distance;
        uint16_t speed;
        uint16_t angle;
        uint16_t magnitude;
    };
    
    struct radarStats {
        uint16_t zeroTDATCount = 0;
        uint16_t nonZeroTDATCount = 0;
        
        uint32_t lastNonZeroReadingTime = 0;
        uint32_t lastZeroReadingTime = 0;

        uint16_t minDistance = 0;
        uint16_t maxDistance = 0;
        
        int16_t minSpeed = 0;
        int16_t maxSpeed = 0;
        
        int16_t minAngle = 0;
        int16_t maxAngle = 0;

        uint16_t minMagnitude = 0;
        uint16_t maxMagnitude = 0;
    };
    radarStats stats;
    
    int tDataWriteIndex = 0;
    struct tDataEntry tData[10];
    
    String status;

    void setSerialConnection(HardwareSerial *connection);
    
    void setStatus(String s);
    void setStatus(const char* s);
    String getStatus();
    
    // no need to stack allocate on every read
    uint16_t distance;
    int16_t speed;
    int16_t angle;
    uint16_t magnitude;
    void addTDATReading(uint16_t distance,
                        int16_t speed,
                        int16_t angle,
                        uint16_t magnitude);

    RESPONSE init();
    RESPONSE getRadarParameters();
    RESPONSE getNextFrameData();
};
#endif