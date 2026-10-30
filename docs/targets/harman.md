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
