# Delphi MIB2 Standard development branch

**Target:** Volkswagen Delphi MIB2 Standard, software train family `MST2_*D`, with `MST2_EU_VW_P0879D` the original offline research sample.

This is the **Delphi-only** development track. The experimental C99 iAP2 Route Guidance core and normalized guidance state are inherited from the shared offline branch.

## Current evidence

- P0879D iAP2 Identify builder is documented; the stock driver has no observed RGI component builder.
- An original receive-dispatch tree routes incoming `0x5201`, `0x5202` and `0x5204` to its default handler. Advertising message IDs alone cannot produce a working receiver.
- Delphi HMI/BAP guidance APIs are **candidates** for output; no live binding or bus delivery is proven.
- MHI2/MHI2Q firmware patches, code offsets, loaders and capabilities are **not** interchangeable with Delphi.

## Development order

1. Verify a real decrypted iAP2 Identify and the driver sender boundary on an authorized bench unit.
2. Implement a coherent RGI Identify declaration and a separate native receive adapter; forward unrelated messages to the original dispatcher.
3. Connect accepted decrypted RGI frames to `dcc_session_init` / `dcc_feed` / `dcc_guidance_callback` and verify real navigation data while retaining privacy.
4. Only then investigate OEM HMI/BAP output and navigation ownership arbitration on compatible clusters.
5. Evaluate map video as **a separate feature**, not an assumed consequence of turn-by-turn support.

## Validation

Current source has synthetic host tests only. No tested firmware writer, installer, on-device receiver, iPhone negotiation, HMI API binding or vehicle integration is included. Never publish firmware, raw device dumps, security tokens or VINs.
