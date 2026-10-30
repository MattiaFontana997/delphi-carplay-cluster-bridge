#ifndef DCC_GUIDANCE_H
#define DCC_GUIDANCE_H

#include "dcc_rgi.h"

/* Host-side normalized RGI state. Values are raw protocol quantities, not
 * Volkswagen BAP enums; no cluster, bus, device or firmware access exists. */
#define DCC_GUIDANCE_TEXT_CAP 256u
#define DCC_GUIDANCE_MANEUVER_SLOTS 16u

typedef struct { uint8_t present, value; } DccGuidanceU8;
typedef struct { uint8_t present; uint16_t value; } DccGuidanceU16;
typedef struct { uint8_t present; uint32_t value; } DccGuidanceU32;
typedef struct { uint8_t present; uint64_t value; } DccGuidanceU64;
typedef struct { uint8_t present; char value[DCC_GUIDANCE_TEXT_CAP]; } DccGuidanceText;

typedef struct {
    uint8_t occupied;
    uint16_t index;
    uint64_t last_seen;
    DccGuidanceU8 maneuver_type;   /* Raw Apple enum: not translated to BAP. */
    DccGuidanceU32 step_distance_m;
    DccGuidanceText description, after_road;
} DccGuidanceManeuver;

typedef struct {
    uint64_t epoch, revision, maneuver_sequence;
    uint8_t session_seen, maneuver_list_seen;
    DccResult last_result; /* Diagnostic status of the most recent callback. */
    DccGuidanceU8 route_state, source_supports_rg, visible_in_app;
    DccGuidanceU16 maneuver_count, current_index, last_lane_index;
    DccGuidanceU32 distance_to_destination_m, distance_to_maneuver_m;
    DccGuidanceU64 eta_raw, remaining_time_raw;
    DccGuidanceText current_road, destination;
    uint64_t lane_frames_seen;
    DccGuidanceManeuver maneuvers[DCC_GUIDANCE_MANEUVER_SLOTS];
} DccGuidance;

void dcc_guidance_init(DccGuidance *);
/* A RESET starts a fresh epoch. On MESSAGE, the epoch must match the most
 * recent RESET. Missing fields keep their prior known/unknown status; explicit
 * zero remains distinct from absence. On error, state remains unchanged. */
DccResult dcc_guidance_apply(DccGuidance *, DccEvent, uint64_t epoch,
                             const DccMessage *);
/* Convenience callback for dcc_session_init(). Does not retain borrowed
 * DccMessage buffers. last_result is updated even if apply fails. */
void dcc_guidance_callback(void *, DccEvent, uint64_t, const DccMessage *);
/* NULL when no current index is reported or no data for that index arrived. */
const DccGuidanceManeuver *dcc_guidance_current(const DccGuidance *);

#endif
