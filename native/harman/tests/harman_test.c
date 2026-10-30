#include "dcc_harman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(x) do {if(!(x)){fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#x);exit(1);}}while(0)
static void w16(uint8_t *p,unsigned v){p[0]=(uint8_t)(v>>8);p[1]=(uint8_t)v;}
static size_t make(uint8_t *b,unsigned id){b[0]=b[1]=0x40;w16(b+4,id);return 6;}
static void add(uint8_t *b,size_t *len,unsigned id,const uint8_t *v,size_t n){
    w16(b+*len,(unsigned)(4+n));w16(b+*len+2,id);
    if(n)memcpy(b+*len+4,v,n);
    *len+=4+n;
}
static void send(DccGuidance *g,uint8_t *b,size_t n){
    DccMessage msg;w16(b+2,(unsigned)n);
    CHECK(dcc_decode(b,n,&msg)==DCC_OK);
    CHECK(dcc_guidance_apply(g,DCC_EVENT_MESSAGE,g->epoch,&msg)==DCC_OK);
}
static void test_family(void){
    CHECK(dcc_harman_family("MHI2_ER_VWG13_K4525_MU1367")==DCC_HARMAN_MHI2);
    CHECK(dcc_harman_family("MHI2Q_ER_AUG22_P5092")==DCC_HARMAN_MHI2Q);
    CHECK(dcc_harman_family("MST2_EU_VW_P0879D")==DCC_HARMAN_UNKNOWN);
    CHECK(dcc_harman_family("")==DCC_HARMAN_UNKNOWN);
    CHECK(dcc_harman_family(NULL)==DCC_HARMAN_UNKNOWN);
    CHECK(dcc_harman_family("MHI2XX_foo")==DCC_HARMAN_UNKNOWN);
    puts("PASS distinct Harman train families and unknown Delphi classification");
}
static void test_snapshot(void){
    DccGuidance g;DccHarmanSnapshot snap, unchanged;
    uint8_t b[160], idx[2]={0,4},dist[4]={0,0,0,0},kind=3;
    size_t n;
    dcc_guidance_init(&g);
    memset(&snap,0xa5,sizeof(snap));unchanged=snap;
    CHECK(dcc_harman_snapshot(&g,&snap)==DCC_STALE);
    CHECK(!memcmp(&snap,&unchanged,sizeof(snap)));
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,7,NULL)==DCC_OK);
    n=make(b,0x5201);
    add(b,&n,10,dist,sizeof(dist));
    add(b,&n,13,idx,sizeof(idx));
    add(b,&n,3,(const uint8_t *)"Via Roma",8);
    send(&g,b,n);
    CHECK(dcc_harman_snapshot(&g,&snap)==DCC_OK);
    CHECK(snap.epoch==7&&snap.revision==1);
    CHECK(snap.distance_to_maneuver_m.present&&snap.distance_to_maneuver_m.value==0);
    CHECK(!snap.eta_raw.present&&!snap.current_maneuver_known);
    CHECK(snap.current_road.present&&!strcmp(snap.current_road.value,"Via Roma"));
    n=make(b,0x5202);
    add(b,&n,1,idx,sizeof(idx));add(b,&n,3,&kind,1);
    add(b,&n,2,(const uint8_t *)"Keep right",10);
    send(&g,b,n);
    CHECK(dcc_harman_snapshot(&g,&snap)==DCC_OK);
    CHECK(snap.current_maneuver_known&&snap.maneuver_type_raw.present);
    CHECK(snap.maneuver_type_raw.value==3);
    CHECK(!strcmp(snap.maneuver_description.value,"Keep right"));
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,8,NULL)==DCC_OK);
    CHECK(dcc_harman_snapshot(&g,&snap)==DCC_OK);
    CHECK(!snap.current_road.present&&!snap.current_maneuver_known);
    CHECK(!snap.distance_to_maneuver_m.present&&snap.epoch==8);
    puts("PASS synthetic route/maneuver projection; zero, unknown and reset");
}
int main(void){test_family();test_snapshot();puts("ALL HARMAN HOST TESTS PASSED (no device tests)");return 0;}
