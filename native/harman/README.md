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
