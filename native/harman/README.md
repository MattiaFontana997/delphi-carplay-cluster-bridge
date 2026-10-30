# Harman host-only RGI projection

This is an **experimental PC-side adapter**, NOT a patch for the Discover Pro and NOT a cluster transport driver.

`include/dcc_harman.h` exposes two small interfaces:

- `dcc_harman_family(train)`: labels software-train **prefixes** `MHI2_` and `MHI2Q_` separately. This does not determine ABI, device or map capability, even if the part number appears to match.
- `dcc_harman_snapshot(guidance, output)`: takes a **copy** of the generic normalized guidance state and the currently resolved maneuver. Distances are raw meters, ETA is raw, maneuver type remains an unconverted Apple numeric value, and absent data remain absent. No BAP/CAN/MOST output is generated.

It is deliberately a *thin* adapter: true Harman-specific API calls cannot be implemented responsibly until a matching `MHI2` or `MHI2Q` firmware and real iAP2 receiver path have been independently documented.

## Test on Windows/Linux

Requires a C99 compiler and CMake. From the repository root:

```sh
cmake -S native/rgi -B build/rgi
cmake --build build/rgi --config Release
ctest --test-dir build/rgi -C Release --output-on-failure
```

Expected test names include `offline_protocol_core`, `offline_identify_preparation`, `offline_guidance_state` and `offline_harman_projection`. These tests use synthetic inputs exclusively. Passing them does not prove real iPhone reception, compatibility with `5NA 035 020 D`, or navigation/CarPlay output on the VW instrument cluster.

No stock firmware, proprietary code or signing material belongs in this repository.


## Analog-cluster prototype (centre display, no map video)

The new `include/dcc_harman_analog.h`, `src/dcc_harman_analog.c` and `tests/harman_analog_test.c` implement a read-only projection for a traditional instrument cluster.

The projection:

- carries the raw CarPlay maneuver, optional text, current-road/destination, ETA and numeric distances;
- shows a next-turn distance **only when the referenced maneuver has been received**;
- clears old turn-distance presence if a new maneuver index arrives without an updated distance;
- resets route data on session reset, and leaves all output unchanged when the input is invalid;
- always excludes cluster map/video output;
- exposes three explicit, externally verified prerequisite flags: working OEM arrows on the cluster, actual RGI reception and identified VW BAP output. These flags only record evidence; they do not transmit or validate BAP commands.

A raw Apple maneuver number is **not** a VW BAP maneuver code. Mapping it and sending data to the Golf 7 analog cluster are still open development tasks. The part number `5G1 920 741 D` does not prove a BAP feature by itself.

Run the synthetic `offline_harman_analog_cluster` test with the same CMake/CTest commands above. It is entirely offline: **do not install it in the car**.

### Existing external research

- `edzenis/mib2high-carplay-altscreen-rgi`: has vehicle-tested MHI2 iAP2 RGI to Audi factory HUD via `CombiBAPServiceNavi` on K2161. No evidence that the Audi Java/BAP contract or native hook offsets match VW MHI2.
- `MacsInSpace/mib-cluster-map`: supports VW MHI2 digital-cluster video in some firmware versions, **not** a ready-made analog VW CarPlay RGI bridge.
- `waterfalldev/mib2q-carplay-enhanced-nonfpk`: Audi **MHI2Q**, not an MHI2 patch or a Volkswagen compatibility certificate.

Research findings may guide our original implementation; vendor-specific or GPL code must not be copied wholesale into the MIT branch without a deliberate license review.
