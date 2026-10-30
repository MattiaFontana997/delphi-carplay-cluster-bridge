# Changelog

## 0.1.0-experimental — 2026-10-30

- First public-source candidate of the C99 RGI protocol/session core and Identify preparation module, with synthetic host tests.
- Added bounded decoding, UTF-8/numeric accessors, lane validation, session-token isolation, framing and inactivity expiry.
- Added atomic/idempotent Identify transformation with conflict checks and message-ID-list preservation.
- Subscription start/stop encoders include the same caller-selected component ID; corrected the stop encoder from the earlier private draft.
- Extended CI to build and test the C module on a host. No live driver/HMI bindings, QNX ABI validation, phone acceptance, video integration or installer provided.

## 0.0.4 — 2026-10-30

- Added bounded iAP2 identification/receive-dispatch reconstruction and destination-information findings.
- Documented configurable advertised message-ID extensions and the raw-send API linkage, with runtime limitations.
- Evaluated the reconstructed control-dispatch comparison tree for all 65,536 16-bit IDs; incoming RGI IDs reach its default branch.
- Added sanitized structured evidence. No vendor binaries, live receiver, firmware patches or installers added; offline analyzer/test code unchanged.

## 0.0.3 — 2026-10-30

- Added static research report and structured evidence without proprietary binaries or stock classes.
- Documented the ASLNavBAPAPI/HMI output candidate and selected bytecode links.
- Documented the missing explicit RGI callbacks in the examined Java CarPlay interfaces.
- Recorded the stream-type dispatch limitation in original AirPlay code and selected P0879D/P0891D component comparisons.
- Updated architecture and contributor priorities; real RGI ingress remains open.
- Offline analyzer/test code unchanged. No device-facing bridge, patch or installer added. This is a research documentation release.

## 0.0.2 — 2026-10-30

- Added NavIgnore scope, historical attribution and compatibility caveats.

## 0.0.1 — 2026-10-30

- Initial offline analyzer, synthetic tests, documentation and contribution templates.
