#ifndef DCC_HARMAN_ANALOG_H
#define DCC_HARMAN_ANALOG_H

#include "dcc_harman.h"

/* Host-side projection intended for analogue instrument clusters with a
 * centre navigation display. It does not encode/send Volkswagen BAP messages,
 * transmit CAN frames, or imply any MOST/map-video capability. */
typedef enum {
    DCC_ANALOG_EMPTY = 0,
    DCC_ANALOG_ROUTE_DATA = 1
} DccAnalogStatus;

/* Manually recorded evidence, not automatic hardware detection. A part
 * number or MHI2 train alone must never set these bits. */
typedef struct {
    uint8_t oem_nav_arrows_observed;
    uint8_t carplay_rgi_received;
    uint8_t vw_bap_interface_verified;
} DccAnalogEvidence;

typedef struct {
    uint64_t epoch, revision;
    DccAnalogStatus status;
    uint8_t prerequisites_observed;
    uint8_t has_current_maneuver;
    uint8_t map_video_supported; /* Always 0: this is an analogue view. */
    DccGuidanceU8 raw_route_state;
    DccGuidanceU8 raw_maneuver_type; /* No Apple -> VW enum mapping. */
    DccGuidanceU32 distance_to_turn_m;
    DccGuidanceU32 distance_to_destination_m;
    DccGuidanceU64 eta_raw;
    DccGuidanceText road_name, destination, maneuver_description;
} DccAnalogView;

/* A RESET yields EMPTY and clears all fields. All outputs are copied, never
 * borrowed from decoder buffers. Unknown data and unmapped arrow codes stay
 * unknown. On error, output remains byte-for-byte unchanged. */
DccResult dcc_harman_analog_project(const DccGuidance *guidance,
    const DccAnalogEvidence *evidence, DccAnalogView *out);
#endif
