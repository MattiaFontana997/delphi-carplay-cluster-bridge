#include "dcc_guidance.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)){fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#x);exit(1);}}while(0)
static void w16(uint8_t *p,unsigned x){p[0]=(uint8_t)(x>>8);p[1]=(uint8_t)x;}
static size_t newframe(uint8_t *b,unsigned id){b[0]=b[1]=0x40;w16(b+4,id);return 6;}
static void add(uint8_t *b,size_t *n,unsigned id,const void *src,size_t len){
    w16(b+*n,(unsigned)(len+4));w16(b+*n+2,id);
    if(len)memcpy(b+*n+4,src,len);
    *n+=len+4;
}
static void finish(uint8_t *b,size_t n){w16(b+2,(unsigned)n);}
static void submit(DccGuidance *g,uint64_t epoch,uint8_t *bytes,size_t n){
    DccMessage m;finish(bytes,n);CHECK(dcc_decode(bytes,n,&m)==DCC_OK);
    CHECK(dcc_guidance_apply(g,DCC_EVENT_MESSAGE,epoch,&m)==DCC_OK);
}
static void test_route_and_maneuvers(void){
    DccGuidance g;uint8_t b[256],zero=0,one=1,dist[4]={0,0,0,0},index[2]={0,7};
    size_t n;const DccGuidanceManeuver *cur;
    dcc_guidance_init(&g);CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,42,NULL)==DCC_OK);
    n=newframe(b,0x5201);
    add(b,&n,1,&one,1);add(b,&n,10,dist,4);add(b,&n,13,index,2);
    add(b,&n,3,"Via Roma",8);finish(b,n);
    submit(&g,42,b,n);
    CHECK(g.route_state.present&&g.route_state.value==1);
    CHECK(g.distance_to_maneuver_m.present&&g.distance_to_maneuver_m.value==0);
    CHECK(g.current_index.present&&g.current_index.value==7);
    CHECK(g.current_road.present&&!strcmp(g.current_road.value,"Via Roma"));
    CHECK(!g.destination.present&&!g.source_supports_rg.present);
    CHECK(dcc_guidance_current(&g)==NULL);
    n=newframe(b,0x5202);add(b,&n,1,index,2);add(b,&n,3,&one,1);
    add(b,&n,2,"Turn right",10);submit(&g,42,b,n);
    cur=dcc_guidance_current(&g);CHECK(cur&&cur->maneuver_type.value==1);
    CHECK(cur->description.present&&!strcmp(cur->description.value,"Turn right"));
    /* A missing description is not an empty string or an erase operation. */
    n=newframe(b,0x5202);add(b,&n,1,index,2);submit(&g,42,b,n);
    CHECK(!strcmp(dcc_guidance_current(&g)->description.value,"Turn right"));
    n=newframe(b,0x5201);add(b,&n,15,&zero,1);submit(&g,42,b,n);
    CHECK(g.visible_in_app.present&&g.visible_in_app.value==0);
    CHECK(dcc_guidance_current(&g)!=NULL); /* Hidden app != route-ended. */
    n=newframe(b,0x5201);add(b,&n,1,&zero,1);submit(&g,42,b,n);
    CHECK(g.route_state.present&&g.route_state.value==0);
    CHECK(dcc_guidance_current(&g)!=NULL); /* State zero can be transient. */
    n=newframe(b,0x5201);add(b,&n,13,NULL,0);submit(&g,42,b,n);
    CHECK(g.maneuver_list_seen&&!g.current_index.present&&dcc_guidance_current(&g)==NULL);
    puts("PASS route values, missing fields, current maneuver association and transient route states");
}
static void test_stale_and_errors(void){
    DccGuidance g,copy;uint8_t b[512],bad[3]={1,2,3},id[2]={0,1};DccMessage m;size_t n;
    dcc_guidance_init(&g);n=newframe(b,0x5202);add(b,&n,1,id,2);finish(b,n);
    CHECK(dcc_decode(b,n,&m)==DCC_OK);
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_MESSAGE,1,&m)==DCC_STALE);
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,5,NULL)==DCC_OK);
    copy=g;CHECK(dcc_guidance_apply(&g,DCC_EVENT_MESSAGE,4,&m)==DCC_STALE);
    CHECK(!memcmp(&g,&copy,sizeof(g)));
    /* A decoded message is expected, but validate fields independently. */
    n=newframe(b,0x5201);add(b,&n,13,bad,3);finish(b,n);
    memset(&m,0,sizeof(m));m.bytes=b;m.length=(uint16_t)n;m.id=0x5201;
    m.count=1;m.fields[0].id=13;m.fields[0].offset=10;m.fields[0].length=3;m.fields[0].parent=DCC_ROOT;
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_MESSAGE,5,&m)==DCC_FIELD);
    CHECK(!memcmp(&g,&copy,sizeof(g)));
    CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,6,NULL)==DCC_OK);
    CHECK(!g.current_index.present&&!g.current_road.present);
    puts("PASS session epochs, atomic failure and resets");
}
static void test_callback(void){
    DccSession s;DccGuidance g;uint8_t b[256],id[2]={0,2},angle[2]={0xff,0xa6};size_t n;
    uint64_t token;dcc_guidance_init(&g);dcc_session_init(&s,dcc_guidance_callback,&g);
    CHECK(dcc_connect(&s,100,&token)==DCC_OK&&g.session_seen&&g.epoch==token);
    n=newframe(b,0x5204);add(b,&n,1,id,2);add(b,&n,3,"Lane",4);
    finish(b,n);CHECK(dcc_feed(&s,token,101,b,n)==DCC_OK);
    CHECK(g.last_result==DCC_OK&&g.lane_frames_seen==1);
    n=newframe(b,0x5202);add(b,&n,1,id,2);add(b,&n,11,angle,2);
    finish(b,n);CHECK(dcc_feed(&s,token,102,b,n)==DCC_OK);
    CHECK(g.revision==2);
    CHECK(dcc_disconnect(&s,token)==DCC_OK&&g.revision==0&&g.lane_frames_seen==0);
    CHECK(dcc_connect(&s,200,&token)==DCC_OK&&g.epoch==token);
    puts("PASS DccSession callback integration, lane counters and disconnect isolation");
}
static void test_cache_and_limits(void){
    DccGuidance g,before;uint8_t b[512],id[2],order[2],long_text[256];size_t n,i;
    dcc_guidance_init(&g);CHECK(dcc_guidance_apply(&g,DCC_EVENT_RESET,1,NULL)==DCC_OK);
    for(i=0;i<DCC_GUIDANCE_MANEUVER_SLOTS+1;i++){
        id[0]=0;id[1]=(uint8_t)i;
        n=newframe(b,0x5202);add(b,&n,1,id,2);submit(&g,1,b,n);
    }
    CHECK(g.revision==DCC_GUIDANCE_MANEUVER_SLOTS+1);
    order[0]=0;order[1]=0;
    n=newframe(b,0x5201);add(b,&n,13,order,2);submit(&g,1,b,n);
    CHECK(dcc_guidance_current(&g)==NULL); /* The oldest slot (0) was evicted. */
    n=newframe(b,0x5202);add(b,&n,1,order,2);submit(&g,1,b,n);
    CHECK(dcc_guidance_current(&g)!=NULL);
    memset(long_text,'A',sizeof(long_text));
    n=newframe(b,0x5201);add(b,&n,3,long_text,sizeof(long_text));finish(b,n);
    {
        DccMessage m;CHECK(dcc_decode(b,n,&m)==DCC_OK);
        before=g;
        CHECK(dcc_guidance_apply(&g,DCC_EVENT_MESSAGE,1,&m)==DCC_LIMIT);
        CHECK(!memcmp(&g,&before,sizeof(g)));
    }
    puts("PASS bounded LRU maneuver cache and atomic oversized-text rejection");
}
static void test_timeout(void){
    DccSession s;DccGuidance g;uint64_t token;
    dcc_guidance_init(&g);dcc_session_init(&s,dcc_guidance_callback,&g);
    CHECK(dcc_connect(&s,50,&token)==DCC_OK);
    CHECK(dcc_expire(&s,59,10)==DCC_OK&&g.session_seen);
    CHECK(dcc_expire(&s,60,10)==DCC_OK&&!s.connected);
    CHECK(g.revision==0&&!g.current_index.present&&g.last_result==DCC_OK);
    puts("PASS timeout resets route data through session callback");
}
int main(void){test_route_and_maneuvers();test_stale_and_errors();test_callback();test_cache_and_limits();test_timeout();puts("ALL GUIDANCE TESTS PASSED (synthetic host-only)");return 0;}
