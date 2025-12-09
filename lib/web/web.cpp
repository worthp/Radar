#include "web.h"

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
    char buf[256];
    String p ;
    
    unsigned long now = millis();

    p.concat(htmlHead(3));
    p.concat("<body>");
    p.concat(menu());

    /** Activity  */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Activity Summary</th></tr>");
    p.concat("<tr><th scope='col'>Data Class</th><th scope='col'>Count</th><th scope='col'>Elapsed Time</th></tr>");
    p.concat("</thead>");

    p.concat("<tbody>");

    sprintf(buf, "<tr><th scope='row'>No Target Acquired</th><td>%d</td><td>%lu secs ago</td></tr>",
                    radar.stats.zeroTDATCount,
                    (radar.stats.lastZeroReadingTime == 0) ? 0 : (now - radar.stats.lastZeroReadingTime) / 1000);
    p.concat(buf);

    sprintf(buf, "<tr><th scope='row'>Target Acquired</th><td>%d</td><td>%lu secs ago</td></tr>",
                    radar.stats.nonZeroTDATCount,
                    (radar.stats.lastNonZeroReadingTime == 0) ? 0 : (now - radar.stats.lastNonZeroReadingTime) / 1000);
    p.concat(buf);
    
    p.concat("</tbody>");

    p.concat("</table>");
    
    /** Last Target Reading */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='4' scope='col'>Last Target Reading</th></tr>");
    p.concat("<tr><th scope='col'>Distance (cm)</th><th scope='col'>Speed (km/h)</th><th scope='col'>Angle (deg)</th><th scope='col'>Magnitude (dB)</th></tr>");
    p.concat("</thead>");
    p.concat("<tbody>");
    
    sprintf(buf, "<tr><td>%d</td><td>%d</td><td>%d</td><td>%d</td></tr>",
                    radar.distance, radar.speed, radar.angle, radar.magnitude);
    p.concat(buf);
    
    p.concat("</tbody>");
    p.concat("</table>");
    
    /** min max stats */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Measurement History</th></tr>");
    p.concat("<tr><th width='20%' scope='col'>Measure</th><th scope='col'>Min</th><th scope='col'>Max</th></tr>");
    p.concat("</thead>");
    
    p.concat("<tbody>");
    sprintf(buf, "<tr><th scope='row'>Distance</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minDistance, radar.stats.maxDistance);
    p.concat(buf);
    
    sprintf(buf, "<tr><th scope='row'>Speed</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minSpeed, radar.stats.maxSpeed);
    p.concat(buf);
    
    sprintf(buf, "<tr><th scope='row'>Angle</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minAngle, radar.stats.maxAngle);
    p.concat(buf);
    
    sprintf(buf, "<tr><th scope='row'>Magnitude</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minMagnitude, radar.stats.maxMagnitude);
    p.concat(buf);

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
    String s;
    s.concat("<table><tr>");
    s.concat("<td><a href='/'>Home</a></td>");
    s.concat("<td><a href='/radarsettings/'>Radar Settings</a></td>");
    s.concat("<td><a href='/stats/'>Stats</a></td>");
    s.concat("<td><a href='/logs/'>Logs</a></td>");
    s.concat("</tr></table>");

    return s;
}
/**
 * 
 */
String Web::radarSettingsPage(KLD7 radar) {
    char buf[256];
    String p ;

    p.concat(htmlHead(36000));
    p.concat("<body>");
    p.concat(menu());

    /** Activity  */
    p.concat("<br/><table>");
    p.concat("<thead>");
    p.concat("<tr><th colspan='3' scope='col'>Settings</th></tr>");
    p.concat("<tr><th scope='col'>Parameter</th><th>Value</th></tr>");
    p.concat("</thead>");

    p.concat("<tbody>");

    sprintf(buf, "<tr><th scope='row'>Software Version</th><td>%s</td></tr>", radar.software_version); p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>base_frequency</th><td>%d</tr>",  radar.base_frequency); p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>maximum_speed</th><td>%d</tr>",  radar.maximum_speed);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>maximum_range</th><td>%d</tr>",  radar.maximum_range);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>threshold_offset</th><td>%d</tr>",  radar.threshold_offset);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>tracking_filter_type</th><td>%d</td></tr>", radar.tracking_filter_type);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>vibration_suppression</th><td>%d</td></tr>", radar.vibration_suppression);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>minimum_detection_distance</th><td>%d</td></tr>", radar.minimum_detection_distance);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>maximum_detection_distance</th><td>%d</td></tr>", radar.maximum_detection_distance);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>minimum_detection_angle</th><td>%d</td></tr>", radar.minimum_detection_angle);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>maximum_detection_angle</th><td>%d</td></tr>", radar.maximum_detection_angle);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>minimum_detection_speed</th><td>%d</td></tr>", radar.minimum_detection_speed);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>maximum_detection_speed</th><td>%d</td></tr>", radar.maximum_detection_speed);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>detection_direction</th><td>%d</td></tr>", radar.detection_direction);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>range_threshold</th><td>%d</td></tr>", radar.range_threshold);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>angle_threshold</th><td>%d</td></tr>", radar.angle_threshold);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>speed_threshold</th><td>%d</td></tr>", radar.speed_threshold);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>digital_output_1</th><td>%d</td></tr>", radar.digital_output_1);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>digital_output_2</th><td>%d</td></tr>", radar.digital_output_2);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>digital_output_3</th><td>%d</td></tr>", radar.digital_output_3);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>hold_time</th><td>%d</td></tr>", radar.hold_time);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>micro_detection_retrigger</th><td>%d</td></tr>", radar.micro_detection_retrigger);  p.concat(buf);
    sprintf(buf, "<tr><th scope='row'>micro_detection_sensitivity</th><td>%d</td></tr>", radar.micro_detection_sensitivity);  p.concat(buf);
    
    p.concat("</tbody>");
    p.concat("</table>");
    p.concat("<div><a href='/updateradarsettings/'>Update Settings</a></div>") ;
    p.concat("</body></html>");
    return p;
}

String Web::htmlHead(int refreshInterval) {

    char buf[1024];
    // no need to reformat
    if (refreshInterval == lastRefreshInterval) {
        return formattedHtmlHead;
    }
    sprintf(buf, htmlHeadFormat.c_str(), refreshInterval);
    formattedHtmlHead.clear();
    formattedHtmlHead.concat(buf);
    lastRefreshInterval = refreshInterval;
    
    return formattedHtmlHead;
}

/**
 * 
 */
void Web::updateRadarParameters(KLD7 radar) {
    radar.setRadarParameters();
}