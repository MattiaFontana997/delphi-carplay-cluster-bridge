# Harman Discover Pro development branch

**Target:** Volkswagen Discover Pro Harman MIB2 High / MIB2.5, including the reported head-unit part number `5NA 035 020 D`. The **exact installed software train must be read separately**. Do not assume `MHI2` and `MHI2Q` are interchangeable.

This is the **Harman-only** development track. It shares the host-side C99 iAP2 RGI decoder and state engine with the Delphi track, but its eventual receive, Identify, navigation ownership, cluster transport and map-video adapters require independent evidence.

## Scope and boundaries

- `MHI2_*` and `MHI2Q_*` are independent subvariants for research; a matching vehicle part number does not establish binary, ABI or patch compatibility.
- The shared `native/rgi` parser consumes already decrypted iAP2 control frames. It does not intercept phone traffic or access vehicle hardware.
- A *host-only* Harman projection adapter prepares a raw, BAP-neutral snapshot from parsed navigation state. It **does not send** anything over CAN, MOST, BAP or infotainment APIs.
- No direct transfer of Delphi patches, function addresses, QNX binaries, or MHI2Q-only modifications is authorized.
- Cluster video requires its own supported screen negotiation and transport. Route guidance does not imply video support.

## Development order

1. Classify exact head-unit software train (`MHI2` vs `MHI2Q`) and cluster capability from verified diagnostics.
2. Trace a suitable Identify and RGI reception boundary for that specific variant.
3. Validate real control-session messages against the existing RGI decoder on a separate authorized bench unit.
4. Map accepted guidance into a cluster-specific guidance adapter, with ownership arbitration and graceful disconnect.
5. Investigate secondary CarPlay video only after proving capability on the exact software/hardware pair.

## Testing

`ctest` on this branch tests the parser, guidance state and **Harman host-only projection** using synthetic data. It cannot prove compatibility with any physical MIB. No radio writes, installable firmware or live iPhone traffic are included.


## Explicit analogue-cluster target

The Golf 7.5 instrument part number `5G1 920 741 D` is an **analogue-cluster research target**, not a validated CarPlay target. This track must not require an AID/Virtual Cockpit or assume MOST video for the analogue centre display.

The intended data path is:

```text
iPhone CarPlay
    -> MHI2 iAP2 RGI receive adapter [not implemented]
    -> shared dcc_rgi / dcc_guidance core [offline code]
    -> dcc_harman_analog_project [host-only code + tests]
    -> validated VW-specific navigation BAP adapter [not implemented]
    -> Golf analogue cluster centre display [not validated]
```

### Proof gates before device integration

1. Observe the **OEM** VW-navigation turn arrows and distance on this actual instrument cluster.
2. Identify the head-unit's actual MHI2 train and a safe, inspectable iAP2 receive path. Different MHI2Q hook binaries cannot be used.
3. Confirm that real CarPlay `0x5201` and `0x5202` values can be received and decoded from this firmware, not just replayed from fixtures.
4. Identify the VW-specific HMI/BAP maneuver and distance API and verify source-ownership/restore behavior. Audi `CombiBAPServiceNavi` on K2161 is an architectural reference, not a VW implementation.
5. Only after a reversible bench test, evaluate live-cluster output.

Do not turn Apple numeric maneuver codes directly into Volkswagen BAP enum values. Do not infer live support from an analog panel's part number alone. All firmware binaries, driver offsets and stock Java classes must remain out of the public repo.

### Nearby tested projects

- https://github.com/edzenis/mib2high-carplay-altscreen-rgi — **Audi K2161 MHI2 HUD**, RGI to BAP tested on a specific vehicle; no direct VW compatibility.
- https://github.com/MacsInSpace/mib-cluster-map — VW Harman **digital** cluster map path, not proof for an analog cluster.
- https://github.com/waterfalldev/mib2q-carplay-enhanced-nonfpk — Audi **MHI2Q** non-FPK path; different head unit platform.

The work in `native/harman` is independent C99 code; no other project's GPL source was copied.
