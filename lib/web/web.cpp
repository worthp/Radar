#include "web.h"


String Web::homePage(KLD7 radar) {
    String p ;
    p.concat(htmlHead);
    p.concat("<html><body>");
    p.concat(menu());
    p.concat("<h2>NPM Speed Tracker</h2>");
    p.concat("</body></html>");
    return p;
}

String Web::statsPage(KLD7 radar) {
    char buf[256];
    String p ;
    p.concat(htmlHead);
    p.concat("<html><body>");
    p.concat(menu());
    p.concat("<table>");

    sprintf(buf, 
        "<tr><td><b>Zero Count</b></td><td>%d</td></tr>",
        radar.stats.zeroTDATCount);
    p.concat(buf);

    sprintf(buf, 
        "<tr><td><b>Non-Zero Count</b></td><td>%d</td</tr>",
        radar.stats.nonZeroTDATCount);
    p.concat(buf);
    p.concat("</table>");
    
    p.concat("<table>");
    p.concat("<thead><tr><th scope='col'>Measure</th><th scope='col'>Min</th><th scope='col'>Max</th></tr></thead>");
    p.concat("<tbody>");
    
    sprintf(buf, "<tr><th scope='row'>Distance</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minDistance, radar.stats.maxDistance);
    p.concat(buf);
    
    sprintf(buf, "<tr><th scope='row'>Speed</th><td>%d</td><td>%d</td></tr>",
                    radar.stats.minSpeed, radar.stats.maxSpeed);
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
