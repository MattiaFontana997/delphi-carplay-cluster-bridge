# Offline normalized route state (experimental)

The new `dcc_guidance.h` and `dcc_guidance.c` implement a host-side **read-only state model** for route-guidance messages already parsed by `dcc_rgi`. The module never transmits anything, calls Delphi firmware, converts Apple maneuver codes into Volkswagen BAP enums, or claims support for an instrument cluster.

## Example usage

```c
DccGuidance guidance;
DccSession session;
uint64_t token;
dcc_guidance_init(&guidance);
dcc_session_init(&session, dcc_guidance_callback, &guidance);
dcc_connect(&session, 1000, &token);
/* Feed decrypted iAP2 control-session bytes to dcc_feed() only in an
   independently reviewed host environment. */
```

- `0x5201` updates raw route state, source capability, app visibility, distances, ETA, remaining time, optional road/destination, maneuver count and the **first** maneuver-list index. Absent fields preserve their prior known/unknown status; explicit zero is recorded as zero.
- `0x5202` updates a 16-slot cache keyed by raw maneuver index. The currently referenced maneuver is accessible through `dcc_guidance_current()`. It returns NULL until details for that index are received.
- `0x5204` records an optional lane index and the number of lane frames, **not** translated lane arrows. Actual lane geometry and BAP translation remain future work.
- `DCC_EVENT_RESET` clears every field and cache while establishing the new session epoch. Messages from other epochs are rejected. The callback copies data before the borrowed message buffer expires.
- An explicit `route_state=0`, `visible_in_app=0` or empty maneuver list is not treated as conclusive proof the route ended. This model performs **no navigation ownership arbitration**.
- Errors are atomic, apart from `last_result` in the convenience callback. Oversized text is rejected rather than silently truncated. The oldest maneuver cache entry is evicted when all slots are occupied.

Build and test:

```sh
cmake -S native/rgi -B build/rgi
cmake --build build/rgi
ctest --test-dir build/rgi --output-on-failure
```

All fixtures are synthetic. No real phone captures, Delphi ABI verification, QNX deployment, BAP transmission, cluster tests or firmware flashing are included.
