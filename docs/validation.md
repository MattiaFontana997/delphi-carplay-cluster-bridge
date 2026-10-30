# Validation of the initial scaffold

Date: 2026-10-30.

- Seven synthetic automated tests pass on Node.js v24.22.1 on Windows. CI is configured for Node.js 22 on Ubuntu but has not run remotely yet.
- Tests cover raw/GZIP inventory equivalence, input immutability, omission of local input paths, corrupted TAR checksums, corrupted GZIP trailers, truncation, resource limits, unsupported links, traversal paths and nonzero trailing data.
- The analyzer was run against the contributor's original local P0879D Nav EXTBIN_APPS. It read 2355 entries; each payload SHA-256 matched the earlier separate TAR inventory. The input archive SHA-256 matched the evidence ledger.
- The source distribution was inspected for personal local paths and vendor binary files; none were included. Raw analysis reports are intentionally not bundled.

This validation establishes basic offline parser behavior on fixtures and one real archive. It does not establish robust support for every TAR variant, functional CarPlay RGI, a second map stream, device execution or safe firmware installation.
