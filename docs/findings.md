# Initial evidence ledger

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

## Unknowns

- The source and format of actual CarPlay route-guidance events on this Delphi build.
- Whether the CarPlay implementation can advertise/receive a separate cluster map stream.
- Native guidance output APIs and the transport used by each cluster profile.
- Cluster video path, resource ownership and interaction with OEM navigation.
- Exact capabilities of the contributor's installed cluster and its software version.

Preliminary string searches found no explicit RGI/AltScreen identifier in selected components. This is not proof of absence. No device-level tests have been performed for this project.
