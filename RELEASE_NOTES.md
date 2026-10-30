# 0.1.0-experimental

First experimental source release of the offline C99 RGI core and Identify transformer, alongside the existing firmware analyzer and static research.

The code can prepare and validate protocol buffers on a PC. It does not yet connect to the live Delphi sender/receiver or forward guidance to the vehicle cluster. No firmware, proprietary library, installer or radio binary is included. All protocol fixtures are synthetic; no successful iPhone/Delphi negotiation is claimed.

Identify construction tests cover the nine-field RGI component, advertised message directions, preservation of other TLVs, missing/partial lists, idempotence, conflicts and atomic rejection. Core tests cover framing, fields, malformed input, sessions, disconnect and expiry. Both suites include structured mutations; the core also includes 20,000 deterministic malformed-input mutations.

Next milestones: native Identify binding, real RGI receive adapter, then HMI/BAP integration and ownership arbitration. Hardware validation is a separate step.
