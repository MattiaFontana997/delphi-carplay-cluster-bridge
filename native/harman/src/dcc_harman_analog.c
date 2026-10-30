#include "dcc_harman_analog.h"
#include <string.h>

DccResult dcc_harman_analog_project(const DccGuidance *g,
    const DccAnalogEvidence *e, DccAnalogView *out) {
    DccAnalogView value;
    const DccGuidanceManeuver *maneuver;

    if (!g || !e || !out) return DCC_ARGUMENT;
    if (!g->session_seen) return DCC_STALE;

    memset(&value, 0, sizeof(value));
    value.epoch = g->epoch;
    value.revision = g->revision;
    value.prerequisites_observed =
        (uint8_t)(!!e->oem_nav_arrows_observed &&
                  !!e->carplay_rgi_received &&
                  !!e->vw_bap_interface_verified);
    /* Video remains absent regardless of the evidence profile. */
    value.map_video_supported = 0;

    /* A lane-only update (or an unmatched maneuver) must not be mistaken
     * for a route shown on the analogue cluster. */
    if (g->revision != 0 &&
        (g->route_state.present ||
         g->distance_to_destination_m.present ||
         g->distance_to_maneuver_m.present ||
         g->current_index.present ||
         g->current_road.present ||
         g->destination.present)) {
        value.status = DCC_ANALOG_ROUTE_DATA;
        value.raw_route_state = g->route_state;
        value.distance_to_destination_m = g->distance_to_destination_m;
        value.eta_raw = g->eta_raw;
        value.road_name = g->current_road;
        value.destination = g->destination;

        /* Do not pair a distance or arrow with a different maneuver slot.
         * Even when a slot exists, an Apple maneuver number is NOT a
         * Volkswagen BAP symbol and must be mapped separately. */
        maneuver = dcc_guidance_current(g);
        if (maneuver) {
            value.has_current_maneuver = 1;
            value.raw_maneuver_type = maneuver->maneuver_type;
            value.maneuver_description = maneuver->description;
            value.distance_to_turn_m = g->distance_to_maneuver_m;
        }
    }

    *out = value;
    return DCC_OK;
}
