# Delphi CarPlay Cluster Bridge

An open research project exploring CarPlay navigation integration with Volkswagen MIB2 Delphi head units. Initial offline target: `MST2_EU_VW_P0879D`.

**Research scaffold, not a working CarPlay patch.** No device installer, firmware writer, on-device agent, RGI decoder or cluster video bridge is included.

[Italiano](README.it.md) · [Findings](docs/findings.md) · [Architecture](docs/architecture.md) · [Roadmap](docs/roadmap.md) · [Contributing](CONTRIBUTING.md) · [Changelog](CHANGELOG.md)

## Goals

- Turn-by-turn indications on compatible analog clusters and Virtual Cockpits.
- A separate CarPlay map display on Virtual Cockpits with verified video/map capability.
- Explicit capability profiles: neither a MOST connection nor a digital instrument panel alone establishes support.

## What exists today

An offline analyzer inventories a local GZIP/TAR `EXTBIN_APPS.bin`, hashes every TAR entry, and inspects selected ELF headers and diagnostic strings. It does not extract files or execute firmware. It reads the entire stream so GZIP integrity errors are surfaced. Reports omit the input's local filesystem path.

Version **0.0.3** adds static research results: an HMI/BAP guidance-output candidate, the limits of the Java CarPlay interfaces, and a video stream-type gate in the original AirPlay library. Selected components were also compared with P0891D. See the [evidence ledger](docs/findings.md), [full Italian research report](docs/research/2026-10-30-delphi-carplay.it.md) and [structured evidence](docs/research/evidenze_carplay_delphi.json).

The CarPlay RGI input remains unidentified. No live bridge or new device-facing functionality is implemented in this release. Offline tools are unchanged from 0.0.2.

## NavIgnore / NavActiveIgnore

For the proposed path that keeps native navigation active alongside CarPlay, the planned prerequisite is **a firmware-compatible Delphi NavIgnore patch, or equivalent navigation-ownership handling**. The need for a particular patch and its compatibility with the future RGI/video bridge remain unverified; this is not a proven universal dependency. The offline analyzer requires no on-device patch.

NavIgnore alone does not forward CarPlay RGI or map video. Credit for the original MIB2 High NavActiveIgnore contribution goes to **Andrew Leech (`andrewleech` / `alelec`)**: [original PR #189](https://github.com/jilleb/mib2-toolbox/pull/189), [mib2-lsd-patching](https://gitlab.com/alelec/mib2-lsd-patching). Public Delphi reports mention the **Congo & Duke** environment, but authorship of the exact Delphi adaptation remains to be established.

See [NavIgnore scope, credits and compatibility](docs/navignore.md). No patch or installer is bundled.

## Run locally

Requires Node.js 22 or later. No packages to install.

```sh
node --test
node tools/analyze-firmware.cjs /path/to/EXTBIN_APPS.bin local/report.json
```

The output directory must already exist and the output file must not exist. Keep proprietary files and generated reports under the ignored `local/` directory. The tool supports the regular-file/directory USTAR layout observed in the supplied samples; extended TAR headers, links and base-256 numeric fields are rejected rather than guessed. Default decompressed limit: 2 GiB; per-entry limit: 512 MiB; 50,000 entries maximum. Diagnostic strings are collected only from selected files up to 16 MiB.

## Contribute

Useful expertise: QNX/ARM, CarPlay session and route guidance interfaces, Delphi HMI/DSI, native cluster guidance, and graphics/MOST video. Start with the evidence tasks in the roadmap. Synthetic PC fixtures are welcome; fabricated protocol adapters presented as real hardware support are not.

Do not upload firmware, proprietary libraries, stock Java classes, keys, credentials, VINs or raw device logs. Review generated reports before sharing. Compatibility reports should distinguish static evidence, simulation, bench validation and vehicle validation.

## References

- [mst2-carplay-vc](https://github.com/xPaiiN/mst2-carplay-vc): its README explicitly excludes Delphi; its documented initial scope is media integration.
- [VcMOSTRenderMqb](https://github.com/OneB1t/VcMOSTRenderMqb): cluster graphics research on other MIB platforms.
- [mib2q-carplay](https://github.com/kamgurgul/mib2q-carplay): RGI/video work targeting MHI2Q, not a drop-in Delphi solution.

No code from those projects is bundled here. MIT applies only to original code/documentation in this repository; external components retain their own terms. There is no affiliation with Volkswagen, Delphi, Apple or QNX.
