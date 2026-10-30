# NavIgnore: scope, prerequisite status and credits

Updated 2026-10-30. This is an anticipated dependency for one proposed architecture, not an installation instruction or a verified Delphi compatibility claim.

## Planned role

Reusing the OEM navigation/cluster path while CarPlay navigation is active may require compatible NavIgnore/NavActiveIgnore or equivalent handling of navigation ownership. The original contribution suppresses an interlock between smartphone and internal navigation. This preserves native navigation availability; it does not translate CarPlay route guidance or turn native maps into CarPlay maps.

| Scenario | Requirement status |
| --- | --- |
| Current PC analyzer | No patch required |
| Proposed native navigation running alongside CarPlay | Compatible NavIgnore or equivalent ownership handling is a planned prerequisite, pending validation |
| Future Delphi RGI adapter | Exact need and coexistence remain unknown |
| Future Delphi auxiliary map video | Exact need and coexistence remain unknown; NavIgnore alone is insufficient |

NavIgnore names multiple platform-specific implementations. MHI2 and TechniSat/Preh patches are not automatically Delphi-compatible. A future bridge may implement arbitration itself without a standalone patch.

## Attribution

- **Andrew Leech (`andrewleech`, `alelec`)**: original MIB2 High NavActiveIgnore contribution, documented in [jilleb/mib2-toolbox PR #189](https://github.com/jilleb/mib2-toolbox/pull/189), merged in January 2022, and his [mib2-lsd-patching project](https://gitlab.com/alelec/mib2-lsd-patching). These establish historical authorship on that platform, not authorship of every Delphi adaptation.
- **jilleb / mib2-toolbox**: upstream integration context for the original contribution.
- **Congo & Duke**: environment referenced in [public Delphi usage reports](https://github.com/olli991/mib-std2-pq-zr-toolbox/discussions/415). These reports do not establish individual authorship of the installed Delphi adaptation. Confirm its original source, author statement or package credits before making a specific author claim.
- [harman-f/MHI2_navignore](https://github.com/harman-f/MHI2_navignore) preserves other MHI2 patch families; it is not a Delphi support declaration.

Credits imply neither endorsement nor permission to redistribute. No third-party patch source or binaries are included here.

## Coexistence requires investigation

The MHI2Q [CarPlay RGI deployment documentation](https://github.com/luka-dev/mib2q-carplay-rgi/blob/main/docs/deploy/install.md) reports conflicts with particular NavActiveIgnore JARs that replace CarPlay application-state behavior. This argues against declaring a universal prerequisite; it does not prove the same conflict on Delphi. Do not copy installation/removal commands from another platform.

## Offline research task

Identify the exact Delphi patch provenance/version and the HMI classes it overrides. Compare against the future bridge's classes and navigation ownership flow. Record collisions and unknowns before considering any hardware validation. Keep proprietary code and third-party binaries local. No radio changes are included in this milestone.
