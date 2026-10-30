#include "dcc_harman_analog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "FAIL %s:%d %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)
static void w16(uint8_t *p,unsigned v) { p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v; }
static size_t frame(uint8_t *b,unsigned id) { b[0]=b[1]=0x40;w16(b+4,id);return 6; }
static void add(uint8_t *b,size_t *n,unsigned id,const void *v,size_t length) {
    w16(b+*n,(unsigned)(length+4));w16(b+*n+2,id);
    if(length)memcpy(b+*n+4,v,length);
    *n+=length+4;
}
static void apply(DccGuidance *g,uint8_t *b,size_t n) {
    DccMessage msg;
    w16(b+2,(unsigned)n);
    CHECK(dcc_decode(b,n,&msg)==DCC_OK);
    CHECK(dcc_guidance_apply(g,DCC_EVENT_MESSAGE,g->epoch,&msg)==DCC_OK);
}
static void analog_test(void) {
    DccGuidance g;
    DccAnalogView view,before;
    DccAnalogEvidence evidence={0,0,0};
    uint8_t bytes[160], index[2]={0,7}, distance[4]={0,0,0,250}, arrow=5;
    size_t n;

    dcc_guidance_init(&g);
    memset(&view,0xa5,sizeof(view));before=view;
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_STALE);
    CHECK(!memcmp(&view,&before,sizeof(view)));
    CHECK(dcc_harman_analog_project(NULL,&evidence,&view)==DCC_ARGUMENT);
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,9,NULL)==DCC_OK);
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_OK);
    CHECK(view.status==DCC_ANALOG_EMPTY&&view.epoch==9);
    CHECK(!view.map_video_supported&&!view.has_current_maneuver);

    n=frame(bytes,0x5201);
    add(bytes,&n,13,index,2);
    add(bytes,&n,10,distance,4);
    add(bytes,&n,3,"Via Roma",8);
    apply(&g,bytes,n);
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_OK);
    CHECK(view.status==DCC_ANALOG_ROUTE_DATA);
    CHECK(view.road_name.present&&!strcmp(view.road_name.value,"Via Roma"));
    CHECK(!view.has_current_maneuver&&!view.distance_to_turn_m.present);
    CHECK(!view.prerequisites_observed);

    n=frame(bytes,0x5202);
    add(bytes,&n,1,index,2);
    add(bytes,&n,3,&arrow,1);
    add(bytes,&n,2,"Turn right",10);
    apply(&g,bytes,n);
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_OK);
    CHECK(view.has_current_maneuver&&view.raw_maneuver_type.present);
    CHECK(view.raw_maneuver_type.value==5);
    CHECK(view.distance_to_turn_m.present&&view.distance_to_turn_m.value==250);
    CHECK(view.maneuver_description.present);
    CHECK(!strcmp(view.maneuver_description.value,"Turn right"));
    /* This output is an intent, not an installed/verified BAP command. */
    evidence.oem_nav_arrows_observed=1;
    evidence.carplay_rgi_received=1;
    evidence.vw_bap_interface_verified=1;
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_OK);
    CHECK(view.prerequisites_observed && !view.map_video_supported);

    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,10,NULL)==DCC_OK);
    CHECK(dcc_harman_analog_project(&g,&evidence,&view)==DCC_OK);
    CHECK(view.status==DCC_ANALOG_EMPTY);
    CHECK(!view.has_current_maneuver&&!view.road_name.present);
    CHECK(!view.raw_maneuver_type.present&&!view.distance_to_turn_m.present);

    puts("PASS analog-only snapshot, late maneuver, evidence flags, reset and atomic rejection");
}
int main(void) {
    analog_test();
    puts("ALL ANALOG PROJECTION TESTS PASSED (synthetic host only)");
    return 0;
}
