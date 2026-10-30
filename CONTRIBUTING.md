# Contributing

Start with a small, reproducible offline question. Open a research issue using the template and state the target firmware, observed interface, evidence, uncertainty and proposed check.

- Keep firmware inputs read-only. Never execute a supplied binary as part of analysis.
- Use synthetic fixtures for automated tests. Add tests for real parsing boundaries or failure modes, not merely restatements of implementation.
- Separate observations from hypotheses. Strings and symbol names do not prove active functionality.
- Do not publish stock firmware, libraries, extracted stock classes, signing keys or personal device information. Describe how to reproduce findings using a contributor's own local inputs.
- Keep local inputs and raw reports under `local/` or `reports/`; ignore rules are a convenience, not a review of what is staged.
- No on-device installers, boot changes or bus-transmitting tools are part of the initial scope.
- Run `node --test`. State any validation that could not be performed.
- Original contributions are submitted under MIT. Identify third-party code and its license before proposing reuse; do not assume GPL or unlicensed projects can be copied into MIT code.

The repository is not yet published. Repository ownership, collaborator permissions and publication are separate decisions. No external invitations or messages have been sent.
