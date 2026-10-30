#ifndef DCC_HARMAN_H
#define DCC_HARMAN_H

#include "dcc_guidance.h"

/* Host-only Harman adapter. No QNX ABI, Apple hardware receiver, Volkswagen
 * BAP formatting, CAN/MOST transmission or video transport is implemented. */
typedef enum {
    DCC_HARMAN_UNKNOWN=0, DCC_HARMAN_MHI2=1, DCC_HARMAN_MHI2Q=2
} DccHarmanFamily;

/* Family detection is purely a software-train prefix check, NOT a claim of
 * device support or firmware/cluster compatibility. */
DccHarmanFamily dcc_harman_family(const char *software_train);

typedef struct {
    uint64_t epoch, revision;
    DccGuidanceU8 route_state;
    DccGuidanceU32 distance_to_maneuver_m;
    DccGuidanceU32 distance_to_destination_m;
    DccGuidanceU64 eta_raw;
    DccGuidanceText current_road;
    DccGuidanceText destination;
    uint8_t current_maneuver_known;
    DccGuidanceU8 maneuver_type_raw; /* Apple enum, never assumed VW BAP. */
    DccGuidanceText maneuver_description;
} DccHarmanSnapshot;

/* Snapshots are BAP-neutral, read-only copies; unknown fields remain unknown.
 * On error, output is unchanged. Does NOT check a live route or render status. */
DccResult dcc_harman_snapshot(const DccGuidance *, DccHarmanSnapshot *);

#endif
