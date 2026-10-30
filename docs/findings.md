# Evidence ledger — 0.0.3

Recorded 2026-10-30. Input identified by the contributor as original `MST2_EU_VW_P0879D`, `STD2Nav_EU_VW`.

| Observation | Evidence class | What it does not establish |
| --- | --- | --- |
| EXTBIN_APPS is a GZIP-compressed TAR with 2355 entries | Offline container analysis | Installability or firmware authenticity |
| CarPlay, CarPlayServices, ClusterServices, Most exist | TAR inventory | A functional link between those services |
| Selected executables are ELF32, little endian, machine 40 (ARM), reference ldqnx.so.2 | Header/string inspection | Exact QNX release or an ABI-compatible build environment |
| ClusterServices has NavGuidance, NavigationInfo, GfxStreamSink, maneuver-distance and ETA references | Static diagnostic strings | CarPlay RGI support, availability on every cluster, or callable APIs |
| libScreenStream.so refers to H.264 decoding | Static diagnostic strings | An auxiliary CarPlay screen, independent map video, or output to the cluster |

Original EXTBIN_APPS SHA-256: `4241058e0d69dfdfaf1b0d7569b5cd42cf8db9c57037d3d6da6e3fbf91d6346c`.

A previous skin update was reported to end with **Aborted**. The cause is not established. Skin-container validation is separate from this project's CarPlay feasibility work. No inference of working signatures, safe flashing or hardware compatibility follows from the observations above.

## Extended static research

Full details: [Italian research report](research/2026-10-30-delphi-carplay.it.md), [structured evidence](research/evidenze_carplay_delphi.json).

| Finding | Evidence | Practical limit |
| --- | --- | --- |
| HMI/BAP output candidate identified | ASLNavBAPAPI signatures and selected bytecode calls from HsmTargetBap; implementation updates HMI collectors/lists | No live event injection, bus transmission or cluster compatibility demonstrated |
| Explicit RGI callbacks absent from the examined Java CarPlay interfaces | 13 declared DSICarplay and 15 declared DSICarplayListener methods; signatures cross-checked against the original dsiifc.jar | Does not prove RGI is absent elsewhere in native code |
| The examined AirPlay session setup dispatch handles stream types 100, 101 and 110; type 111 takes the unsupported branch | Targeted ARM disassembly of the original libAirPlay.so | Not a complete audit of every code path; accepting another type alone would not implement an auxiliary display |
| AirPlay libraries, iAP2 driver/config and dsiifc.jar are byte-identical in supplied P0879D/P0891D samples | SHA-256 comparison | Surrounding executables differ; overall firmware behavior is not established as identical |
| Native-navigation ownership interacts with CarPlay mode changes | Selected NavigationHandler.updateGuidanceActive bytecode references | Installed NavIgnore variant has not been analyzed |

The IFS/JXE research conversion produced 19,403 MIBHMI and 5,332 Delphi classes with no skipped classes reported. Resulting ZIP CRCs passed; conversion was not validated by a full JVM verifier. Counts describe converted material, not exhaustive semantic analysis of every class. Proprietary binaries and classes remain local and are not distributed.

## Unknowns

- The source and format of actual CarPlay route-guidance events on this Delphi build.
- Whether the CarPlay implementation can advertise/receive a separate cluster map stream.
- Runtime suitability of the HMI/BAP output candidate and the transport/capabilities of each cluster profile.
- Cluster video path, resource ownership and interaction with OEM navigation.
- Exact capabilities of the contributor's installed cluster and its software version.

Preliminary string searches found no explicit RGI/AltScreen identifier in selected components. This is not proof of absence. No device-level tests have been performed for this project.
