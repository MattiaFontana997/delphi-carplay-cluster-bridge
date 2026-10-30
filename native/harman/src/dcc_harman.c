#include "dcc_harman.h"
#include <string.h>

DccHarmanFamily dcc_harman_family(const char *train) {
    if(!train)return DCC_HARMAN_UNKNOWN;
    if(strncmp(train,"MHI2Q_",6)==0)return DCC_HARMAN_MHI2Q;
    if(strncmp(train,"MHI2_",5)==0)return DCC_HARMAN_MHI2;
    return DCC_HARMAN_UNKNOWN;
}
DccResult dcc_harman_snapshot(const DccGuidance *g,DccHarmanSnapshot *out) {
    DccHarmanSnapshot value;
    const DccGuidanceManeuver *current;
    if(!g||!out)return DCC_ARGUMENT;
    if(!g->session_seen)return DCC_STALE;
    memset(&value,0,sizeof(value));
    value.epoch=g->epoch;value.revision=g->revision;
    value.route_state=g->route_state;
    value.distance_to_maneuver_m=g->distance_to_maneuver_m;
    value.distance_to_destination_m=g->distance_to_destination_m;
    value.eta_raw=g->eta_raw;
    value.current_road=g->current_road;
    value.destination=g->destination;
    current=dcc_guidance_current(g);
    if(current) {
        value.current_maneuver_known=1;
        value.maneuver_type_raw=current->maneuver_type;
        value.maneuver_description=current->description;
    }
    *out=value;
    return DCC_OK;
}
