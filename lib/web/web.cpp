#include "web.h"

String Web::homePage(KLD7 radar) {
    String p ;
    p.concat(htmlHead);
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

    p.concat(htmlHead);
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

String Web::menu()
{
    String s;
    s.concat("<table><tr>");
    s.concat("<td><a href='/'>Home</a></td>");
    s.concat("<td><a href='/stats/'>Stats</a></td>");
    s.concat("</tr></table>");

    return s;
}
