#include "web.h"

Web::Web() {
    memset(formattingBuffer, 0, sizeof(formattingBuffer));
}

String Web::homePage(KLD7 radar) {
    String p ;
    p.concat(htmlHead(3600));
    p.concat("<body>");
    p.concat(menu());
    p.concat("<h2>NPM Speed Tracker</h2>");
    p.concat("</body></html>");
    return p;
}

String Web::statsPage(KLD7 radar) {
    String p ;
    
    unsigned long now = millis();

    p.concat(htmlHead(5));
    p.concat("<body>");
    p.concat(menu());

    /** Activity  */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Activity Summary</th></tr>");
    p.concat("<tr><th scope='col'>Data Class</th><th scope='col'>Count</th><th scope='col'>Elapsed Time</th></tr>");
    p.concat("</thead>");

    p.concat("<tbody>");

    unsigned long seconds = (now - radar.stats.startedOn) / 1000;
    sprintf(formattingBuffer, "<tr><th scope='row'>Uptime</th><td colspan='2'>%02lu:%02lu:%02lu</td></tr>",
                (unsigned long)seconds/3600, (unsigned long)(seconds/60)%60, (unsigned long)seconds%60);
    p.concat(formattingBuffer);

    sprintf(formattingBuffer, "<tr><th scope='row'>No Target Acquired</th><td>%d</td><td>%lu secs ago</td></tr>",
                    radar.stats.zeroTDATCount,
                    (radar.stats.lastZeroReadingTime == 0) ? 0 : (now - radar.stats.lastZeroReadingTime) / 1000);
    p.concat(formattingBuffer);

    sprintf(formattingBuffer, "<tr><th scope='row'>Target Acquired</th><td>%d</td><td>%lu secs ago</td></tr>",
                    radar.stats.nonZeroTDATCount,
                    (radar.stats.lastNonZeroReadingTime == 0) ? 0 : (now - radar.stats.lastNonZeroReadingTime) / 1000);
    p.concat(formattingBuffer);
    
    p.concat("</tbody>");

    p.concat("</table>");
    
    /** Last Target Reading */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='4' scope='col'>Last Target Reading</th></tr>");
    p.concat("<tr><th scope='col'>Distance (cm)</th><th scope='col'>Speed (km/h)</th><th scope='col'>Angle (deg)</th><th scope='col'>Magnitude (dB)</th></tr>");
    p.concat("</thead>");
    p.concat("<tbody>");
    
    sprintf(formattingBuffer, "<tr><td>%d</td><td>%2.2f</td><td>%2.2f</td><td>%2.2f</td></tr>",
                    radar.distance, radar.f_speed, radar.f_angle, radar.f_magnitude);
    p.concat(formattingBuffer);
    
    p.concat("</tbody>");
    p.concat("</table>");
    
    /** min max stats */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Measurement History</th></tr>");
    p.concat("<tr><th width='20%' scope='col'>Measure</th><th scope='col'>Min</th><th scope='col'>Max</th></tr>");
    p.concat("</thead>");
    
    p.concat("<tbody>");
    sprintf(formattingBuffer, "<tr><th scope='row'>Distance (cm)</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minDistance, radar.stats.maxDistance);
    p.concat(formattingBuffer);
    
    sprintf(formattingBuffer, "<tr><th scope='row'>Speed (km/h)</th><td>%2.2f</td><td>%2.2f</td></tr>",
                    radar.stats.minSpeed, radar.stats.maxSpeed);
    p.concat(formattingBuffer);
    
    sprintf(formattingBuffer, "<tr><th scope='row'>Angle (deg)</th><td>%2.2f</td><td>%2.2f</td></tr>",
                    radar.stats.minAngle, radar.stats.maxAngle);
    p.concat(formattingBuffer);
    
    sprintf(formattingBuffer, "<tr><th scope='row'>Magnitude (dB)</th><td>%2.2f</td><td>%2.2f</td></tr>",
                    radar.stats.minMagnitude, radar.stats.maxMagnitude);
    p.concat(formattingBuffer);

    p.concat("</tbody>");
    p.concat("</table>");

    p.concat("</body></html>");
    return p;
}
/**
 * 
 */
String Web::logPage(KLD7 radar) {
    String p;

    /** Logs  */
    p.concat(htmlHead(3600));
    p.concat("<body>");
    p.concat(menu());

    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Logs</th></tr>");
    p.concat("</thead>");

    p.concat("<tbody>");

    p.concat("<tr><td text-align='left'><pre>");
    p.concat(radar.getLogs());
    p.concat("</pre></td></tr>");

    p.concat("</tbody>");
    p.concat("</table>");
    p.concat("</body></html>");
    
    return p;
}

String Web::menu()
{
    String s("");
    s.concat("<table>");
    s.concat("<thead><tr>");
    s.concat("<th scope='col'><a href='/'>Home</a></th>");
    s.concat("<th scope='col'><a href='/radarsettings/'>Radar Settings</a></th>");
    s.concat("<th scope='col'><a href='/stats/'>Stats</a></th>");
    s.concat("<th scope='col'><a href='/logs/'>Logs</a></th>");
    s.concat("</tr></thead>");

    s.concat("</table>");

    return s;
}
/**
 * 
 */
String Web::radarSettingsPage(KLD7 radar, char msg[]) {
    String p ;

    p.concat(htmlHead(36000));
    p.concat("<body>");
    p.concat(menu());

    /** Activity  */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Settings</th></tr>");
    p.concat("<tr><th scope='col'>Parameter</th><th scope='col'>Value</th><th scope='col'>Update Value</th></tr>");
    if (strlen(msg) > 0) {
        sprintf(formattingBuffer,"<tr><th scope='col' colspan=3>%s</th></tr>", msg);
        p.concat(formattingBuffer);
    }
    p.concat("</thead>");

    p.concat("<tbody>");

    sprintf(formattingBuffer, "<tr><th scope='row'>Software Version</th><td>%s</td>\
        <td>\
        </td></tr>",  radar.software_version); p.concat(formattingBuffer);
    sprintf(formattingBuffer, "<tr><th scope='row'>base_frequency</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/base_frequency/0'>Low</a>\
        <a href='/updateradarsettings/base_frequency/1'>Middle</a>\
        <a href='/updateradarsettings/base_frequency/2'>High</a>\
        </td></tr>",  radar.base_frequency); p.concat(formattingBuffer);
    //uint8_t base_frequency; // ,UINT8,1,0 = Low, 1 = Middle, 2 = High,1 = Middle
    sprintf(formattingBuffer, "<tr><th scope='row'>maximum_speed</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/maximum_speed/0'>12.5km/h</a>\
        <a href='/updateradarsettings/maximum_speed/1'>25km/h</a>\
        <a href='/updateradarsettings/maximum_speed/2'>50km/h</a>\
        <a href='/updateradarsettings/maximum_speed/3'>100km/h</a>\
        </td>\
        </tr>",
        radar.maximum_speed);  p.concat(formattingBuffer);
    sprintf(formattingBuffer, "<tr><th scope='row'>maximum_range</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/maximum_range/0'>5m</a>\
        <a href='/updateradarsettings/maximum_range/1'>10m</a>\
        <a href='/updateradarsettings/maximum_range/2'>30m</a>\
        <a href='/updateradarsettings/maximum_range/3'>100m</a>\
        </td>\
        </tr>",  radar.maximum_range);  p.concat(formattingBuffer);
    sprintf(formattingBuffer, "<tr><th scope='row'>threshold_offset</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/threshold_offset/10'>10</a>\
        <a href='/updateradarsettings/threshold_offset/20'>20</a>\
        <a href='/updateradarsettings/threshold_offset/30'>30</a>\
        <a href='/updateradarsettings/threshold_offset/40'>40</a>\
        <a href='/updateradarsettings/threshold_offset/50'>50</a>\
        </td>\
        </tr>",  radar.threshold_offset);  p.concat(formattingBuffer);
    //uint8_t threshold_offset; // UINT8,1,10-60 dB,30 dB
    sprintf(formattingBuffer, "<tr><th scope='row'>tracking_filter_type</th><td>%d</td>\
        <td>None\
        </td>\
        </tr>", radar.tracking_filter_type);  p.concat(formattingBuffer);
    //uint8_t tracking_filter_type; // UINT8,1,0 = Standard, 1 = Fast detection, 2 = Long visibility,0 = Standard
    sprintf(formattingBuffer, "<tr><th scope='row'>vibration_suppression</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.vibration_suppression);  p.concat(formattingBuffer);
    //uint8_t vibration_suppression; // UINT8,1,0-16, 0 = No suppression, 16 = High suppression,3 = Medium suppression
    sprintf(formattingBuffer, "<tr><th scope='row'>minimum_detection_distance(%% of max range)</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/minimum_detection_distance/0'>0</a>\
        <a href='/updateradarsettings/minimum_detection_distance/10'>10</a>\
        <a href='/updateradarsettings/minimum_detection_distance/20'>20</a>\
        <a href='/updateradarsettings/minimum_detection_distance/30'>30</a>\
        <a href='/updateradarsettings/minimum_detection_distance/40'>40</a>\
        <a href='/updateradarsettings/minimum_detection_distance/50'>50</a>\
        <a href='/updateradarsettings/minimum_detection_distance/60'>60</a>\
        <a href='/updateradarsettings/minimum_detection_distance/70'>70</a>\
        <a href='/updateradarsettings/minimum_detection_distance/80'>80</a>\
        </td></tr>", radar.minimum_detection_distance);  p.concat(formattingBuffer);
    //uint8_t minimum_detection_distance; //UINT8,1,0 – 100% of range setting,0%
    sprintf(formattingBuffer, "<tr><th scope='row'>maximum_detection_distance(%% of max range)</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/maximum_detection_distance/10'>10</a>\
        <a href='/updateradarsettings/maximum_detection_distance/20'>20</a>\
        <a href='/updateradarsettings/maximum_detection_distance/30'>30</a>\
        <a href='/updateradarsettings/maximum_detection_distance/40'>40</a>\
        <a href='/updateradarsettings/maximum_detection_distance/50'>50</a>\
        <a href='/updateradarsettings/maximum_detection_distance/60'>60</a>\
        <a href='/updateradarsettings/maximum_detection_distance/70'>70</a>\
        <a href='/updateradarsettings/maximum_detection_distance/80'>80</a>\
        </td></tr>", radar.maximum_detection_distance);  p.concat(formattingBuffer);
    //uint8_t maximum_detection_distance; //UINT8,1,0 – 100% of range setting,50%
    sprintf(formattingBuffer, "<tr><th scope='row'>minimum_detection_angle</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.minimum_detection_angle);  p.concat(formattingBuffer);
    //int8_t minimum_detection_angle; //INT8,1,-90° to +90°,-90°
    sprintf(formattingBuffer, "<tr><th scope='row'>maximum_detection_angle</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.maximum_detection_angle);  p.concat(formattingBuffer);
    //int8_t maximum_detection_angle; // INT8,1,-90° to +90°,+90°
    sprintf(formattingBuffer, "<tr><th scope='row'>minimum_detection_speed(%% of max speed)</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/minimum_detection_speed/0'>0</a>\
        <a href='/updateradarsettings/minimum_detection_speed/10'>10</a>\
        <a href='/updateradarsettings/minimum_detection_speed/20'>20</a>\
        <a href='/updateradarsettings/minimum_detection_speed/30'>30</a>\
        <a href='/updateradarsettings/minimum_detection_speed/40'>40</a>\
        <a href='/updateradarsettings/minimum_detection_speed/50'>50</a>\
        <a href='/updateradarsettings/minimum_detection_speed/60'>60</a>\
        <a href='/updateradarsettings/minimum_detection_speed/70'>70</a>\
        <a href='/updateradarsettings/minimum_detection_speed/80'>80</a>\
        </td></tr>", radar.minimum_detection_speed);  p.concat(formattingBuffer);
    //uint8_t minimum_detection_speed; // UINT8,1,0 – 100% of speed setting,0%
    sprintf(formattingBuffer, "<tr><th scope='row'>maximum_detection_speed(%% of max speed)</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/maximum_detection_speed/10'>10</a>\
        <a href='/updateradarsettings/maximum_detection_speed/20'>20</a>\
        <a href='/updateradarsettings/maximum_detection_speed/30'>30</a>\
        <a href='/updateradarsettings/maximum_detection_speed/40'>40</a>\
        <a href='/updateradarsettings/maximum_detection_speed/50'>50</a>\
        <a href='/updateradarsettings/maximum_detection_speed/60'>60</a>\
        <a href='/updateradarsettings/maximum_detection_speed/70'>70</a>\
        <a href='/updateradarsettings/maximum_detection_speed/80'>80</a>\
        </td></tr>", radar.maximum_detection_speed);  p.concat(formattingBuffer);
    //uint8_t maximum_detection_speed; // UINT8,1,0 – 100% of speed setting,100%
    sprintf(formattingBuffer, "<tr><th scope='row'>detection_direction</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.detection_direction);  p.concat(formattingBuffer);
    //uint8_t detection_direction; // UINT8,1,0 = Approaching, 1 = Receding, 2 = Both,2 = Both
    sprintf(formattingBuffer, "<tr><th scope='row'>range_threshold</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/range_threshold/10'>10</a>\
        <a href='/updateradarsettings/range_threshold/20'>20</a>\
        <a href='/updateradarsettings/range_threshold/30'>30</a>\
        <a href='/updateradarsettings/range_threshold/40'>40</a>\
        <a href='/updateradarsettings/range_threshold/50'>50</a>\
        <a href='/updateradarsettings/range_threshold/60'>60</a>\
        <a href='/updateradarsettings/range_threshold/70'>70</a>\
        <a href='/updateradarsettings/range_threshold/80'>80</a>\
        </td></tr>", radar.range_threshold);  p.concat(formattingBuffer);
    //uint8_t range_threshold; // UINT8,1,0 – 100% of range setting,10%
    sprintf(formattingBuffer, "<tr><th scope='row'>angle_threshold</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.angle_threshold);  p.concat(formattingBuffer);
    //uint8_t angle_threshold; // INT8,1,-90° to +90°,0°
    sprintf(formattingBuffer, "<tr><th scope='row'>speed_threshold</th><td>%d</td>\
        <td>\
        <a href='/updateradarsettings/speed_threshold/10'>10</a>\
        <a href='/updateradarsettings/speed_threshold/20'>20</a>\
        <a href='/updateradarsettings/speed_threshold/30'>30</a>\
        <a href='/updateradarsettings/speed_threshold/40'>40</a>\
        <a href='/updateradarsettings/speed_threshold/50'>50</a>\
        <a href='/updateradarsettings/speed_threshold/60'>60</a>\
        <a href='/updateradarsettings/speed_threshold/70'>70</a>\
        <a href='/updateradarsettings/speed_threshold/80'>80</a>\
        </td></tr>", radar.speed_threshold);  p.concat(formattingBuffer);
    //uint8_t speed_threshold; // UINT8,1,0 – 100% of speed setting,50%
    sprintf(formattingBuffer, "<tr><th scope='row'>digital_output_1</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.digital_output_1);  p.concat(formattingBuffer);
    //uint8_t digital_output_1; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,0 = Direction
    sprintf(formattingBuffer, "<tr><th scope='row'>digital_output_2</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.digital_output_2);  p.concat(formattingBuffer);
    //uint8_t digital_output_2; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,1 = Angle
    sprintf(formattingBuffer, "<tr><th scope='row'>digital_output_3</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.digital_output_3);  p.concat(formattingBuffer);
    //uint8_t digital_output_3; // UINT8,1,0 = Direction, 1 = Angle, 2 = Range, 3 = Speed, 4 = Micro detection,2 = Range
    sprintf(formattingBuffer, "<tr><th scope='row'>hold_time</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.hold_time);  p.concat(formattingBuffer);
    //uint16_t hold_time; // UINT16,2,1 – 7200s (1s – 2h), 120s
    sprintf(formattingBuffer, "<tr><th scope='row'>micro_detection_retrigger</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.micro_detection_retrigger);  p.concat(formattingBuffer);
    //uint8_t micro_detection_retrigger; // UINT8,1,0 = Off, 1 = Retrigger,0 = Off
    sprintf(formattingBuffer, "<tr><th scope='row'>micro_detection_sensitivity</th><td>%d</td>\
        <td>None\
        </td></tr>", radar.micro_detection_sensitivity);  p.concat(formattingBuffer);
    //uint8_t micro_detection_sensitivity; // UINT8,1,0-9, 0=Min. sensitivity, 9=Max. sensitvity,4 = Medium sensitivity
    
    p.concat("</tbody>");
    p.concat("</table>");
    
    p.concat("<a href='/updateradarsettings/all/all'>Reset Factory Settings</a>");
    return p;
}

String Web::htmlHead(int refreshInterval) {

    // no need to reformat
    if (refreshInterval == lastRefreshInterval) {
        return formattedHtmlHead;
    }
    sprintf(formattingBuffer, htmlHeadFormat.c_str(), refreshInterval);
    formattedHtmlHead.clear();
    formattedHtmlHead.concat(formattingBuffer);
    lastRefreshInterval = refreshInterval;
    
    return formattedHtmlHead;
}

/**
 * 
 */
void Web::updateRadarParameters(KLD7 radar) {
    radar.setRadarParameters();
}