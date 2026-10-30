# Initial contributor backlog

## Research status in 0.0.3

- Completed: reproducible archive inventory and initial static evidence ledger.
- Identified: HMI/BAP guidance-output candidates and selected native-to-HMI update calls. Runtime integration remains open.
- Identified: unsupported stream-111 branch in the examined original AirPlay setup dispatch.
- Completed: SHA-256 comparison of selected P0879D/P0891D components.
- Still open: actual CarPlay RGI ingress, event translation, ownership arbitration, video implementation and hardware capability validation.

Next priority: trace iAP2 feature negotiation/message dispatch to establish a real RGI source. Do not substitute a synthetic arrow demo for this missing link.

## 1. Map the CarPlay input interface

Inspect original local CarPlay/CarPlayServices components and relevant HMI interfaces. Deliver a sanitized evidence table of actual route-guidance callbacks/data structures, including what remains unknown. Do not treat a generic screen or media callback as an RGI callback.

## 2. Map OEM guidance to the cluster

Trace the native navigation-to-ClusterServices path. Identify maneuver/distance/ETA interfaces, their consumers and per-cluster transport. Deliver a call/data-flow description with reproducible offline evidence. No bus traffic or device writes.

## 3. Investigate the video path

Establish separately: auxiliary-screen negotiation, stream availability, decoder path, and cluster-video output. Document differences from MHI2Q and VcMOSTRenderMqb rather than copying platform-specific offsets or startup commands.

## 4. Define event model and PC replay

After real input/output formats are understood, design a platform-independent event model and fixtures. Test unknown maneuvers, unit conversion, missing fields, stale data, reroute, disconnect, end of guidance and source arbitration. Label synthetic versus captured data.

## 5. Build an evidence-based compatibility matrix

Record head-unit firmware, cluster part/software version, guidance capability, video capability, evidence type and limitations. Initial status for device compatibility is unknown. Do not list claimed support solely from part-number similarity.

## 6. Audit NavIgnore compatibility and attribution

Identify the installed Delphi variant and its author from original package/source credits. Compare overridden HMI classes and navigation ownership behavior with the future RGI/video bridge. Record whether standalone NavIgnore is needed, redundant or conflicting for each supported architecture. See [NavIgnore notes](navignore.md).

## Current milestone

Reproducible read-only inventory and an evidence ledger. All live hardware deployment work is outside this milestone. A future hardware phase needs a separately agreed scope; this repository does not provide one implicitly.
