#include "dcc_guidance.h"
#include <string.h>

static DccResult read_number(const DccMessage *m, unsigned id, unsigned width,
                             uint64_t *out, int *found) {
    const DccField *f = dcc_find(m, DCC_ROOT, (uint16_t)id, 0);
    if (!f) { *found=0; return DCC_OK; }
    if (f->length!=width) return DCC_FIELD;
    *found=1;
    return dcc_unsigned(m,f,out);
}
static DccResult u8(const DccMessage *m,unsigned id,DccGuidanceU8 *dst) {
    int found;uint64_t v=0;DccResult r=read_number(m,id,1,&v,&found);
    if(r==DCC_OK&&found){dst->present=1;dst->value=(uint8_t)v;}
    return r;
}
static DccResult u16(const DccMessage *m,unsigned id,DccGuidanceU16 *dst) {
    int found;uint64_t v=0;DccResult r=read_number(m,id,2,&v,&found);
    if(r==DCC_OK&&found){dst->present=1;dst->value=(uint16_t)v;}
    return r;
}
static DccResult u32(const DccMessage *m,unsigned id,DccGuidanceU32 *dst) {
    int found;uint64_t v=0;DccResult r=read_number(m,id,4,&v,&found);
    if(r==DCC_OK&&found){dst->present=1;dst->value=(uint32_t)v;}
    return r;
}
static DccResult u64(const DccMessage *m,unsigned id,DccGuidanceU64 *dst) {
    int found;uint64_t v=0;DccResult r=read_number(m,id,8,&v,&found);
    if(r==DCC_OK&&found){dst->present=1;dst->value=v;}
    return r;
}
static DccResult txt(const DccMessage *m,unsigned id,DccGuidanceText *dst) {
    const DccField *f=dcc_find(m,DCC_ROOT,(uint16_t)id,0);size_t written=0;
    DccResult r;
    if(!f)return DCC_OK;
    r=dcc_copy_text(m,f,dst->value,sizeof(dst->value),&written);
    if(r==DCC_OK)dst->present=1;
    return r;
}
static uint16_t be16(const uint8_t *p) {
    return (uint16_t)(((uint16_t)p[0]<<8)|p[1]);
}
static DccResult route(const DccMessage *m,DccGuidance *g) {
    const DccField *f;DccResult r;
#define APPLY(call) do { r=(call);if(r!=DCC_OK)return r; } while(0)
    APPLY(u8(m,1,&g->route_state));
    APPLY(u8(m,20,&g->source_supports_rg));
    APPLY(u8(m,15,&g->visible_in_app));
    APPLY(u16(m,14,&g->maneuver_count));
    APPLY(u32(m,7,&g->distance_to_destination_m));
    APPLY(u32(m,10,&g->distance_to_maneuver_m));
    APPLY(u64(m,5,&g->eta_raw));
    APPLY(u64(m,6,&g->remaining_time_raw));
    APPLY(txt(m,3,&g->current_road));
    APPLY(txt(m,4,&g->destination));
    f=dcc_find(m,DCC_ROOT,13,0);
    if(f){
        /* Index list is optional; zero-length list is an explicit no-current
         * indication, not a synthetic end-of-route or cache reset. */
        if(f->length%2 || f->offset>m->length || f->length>m->length-f->offset)
            return DCC_FIELD;
        g->maneuver_list_seen=1;
        if(f->length) {
            g->current_index.present=1;
            g->current_index.value=be16(m->bytes+f->offset);
        }else g->current_index.present=0;
    }
#undef APPLY
    return DCC_OK;
}
static DccGuidanceManeuver *slot(DccGuidance *g,uint16_t index) {
    size_t i,chosen=0;uint64_t oldest=UINT64_MAX;
    for(i=0;i<DCC_GUIDANCE_MANEUVER_SLOTS;i++)
        if(g->maneuvers[i].occupied&&g->maneuvers[i].index==index)
            return &g->maneuvers[i];
    for(i=0;i<DCC_GUIDANCE_MANEUVER_SLOTS;i++) {
        if(!g->maneuvers[i].occupied){chosen=i;break;}
        if(g->maneuvers[i].last_seen<oldest){oldest=g->maneuvers[i].last_seen;chosen=i;}
    }
    memset(&g->maneuvers[chosen],0,sizeof(g->maneuvers[chosen]));
    g->maneuvers[chosen].occupied=1;
    g->maneuvers[chosen].index=index;
    return &g->maneuvers[chosen];
}
static DccResult maneuver(const DccMessage *m,DccGuidance *g) {
    DccGuidanceU16 index={0,0};DccGuidanceManeuver *s;DccResult r;
    r=u16(m,1,&index);if(r!=DCC_OK)return r;
    if(!index.present)return DCC_FIELD;
    if(g->maneuver_sequence==UINT64_MAX)return DCC_LIMIT;
    s=slot(g,index.value);
    r=u8(m,3,&s->maneuver_type);if(r!=DCC_OK)return r;
    r=u32(m,5,&s->step_distance_m);if(r!=DCC_OK)return r;
    r=txt(m,2,&s->description);if(r!=DCC_OK)return r;
    r=txt(m,4,&s->after_road);if(r!=DCC_OK)return r;
    s->last_seen=++g->maneuver_sequence;
    return DCC_OK;
}
void dcc_guidance_init(DccGuidance *g) { if(g)memset(g,0,sizeof(*g)); }
DccResult dcc_guidance_apply(DccGuidance *g,DccEvent ev,uint64_t epoch,
                             const DccMessage *m) {
    DccGuidance next;DccResult r;
    if(!g)return DCC_ARGUMENT;
    if(ev==DCC_EVENT_RESET) {
        if(m)return DCC_ARGUMENT;
        memset(g,0,sizeof(*g));g->epoch=epoch;g->session_seen=1;return DCC_OK;
    }
    if(ev!=DCC_EVENT_MESSAGE||!m||!m->bytes)return DCC_ARGUMENT;
    if(!g->session_seen||g->epoch!=epoch)return DCC_STALE;
    if(g->revision==UINT64_MAX)return DCC_LIMIT;
    next=*g;
    switch(m->id) {
        case 0x5201:r=route(m,&next);break;
        case 0x5202:r=maneuver(m,&next);break;
        case 0x5204:
            r=u16(m,1,&next.last_lane_index);
            if(r==DCC_OK){if(next.lane_frames_seen==UINT64_MAX)return DCC_LIMIT;
                ++next.lane_frames_seen;}
            break;
        default:return DCC_UNSUPPORTED;
    }
    if(r!=DCC_OK)return r;
    ++next.revision;*g=next;return DCC_OK;
}
void dcc_guidance_callback(void *ctx,DccEvent ev,uint64_t epoch,const DccMessage *m) {
    DccGuidance *g=(DccGuidance *)ctx;DccResult r;
    if(!g)return;
    r=dcc_guidance_apply(g,ev,epoch,m);
    g->last_result=r;
}
const DccGuidanceManeuver *dcc_guidance_current(const DccGuidance *g) {
    size_t i;
    if(!g||!g->current_index.present)return NULL;
    for(i=0;i<DCC_GUIDANCE_MANEUVER_SLOTS;i++)
        if(g->maneuvers[i].occupied&&g->maneuvers[i].index==g->current_index.value)
            return &g->maneuvers[i];
    return NULL;
}
