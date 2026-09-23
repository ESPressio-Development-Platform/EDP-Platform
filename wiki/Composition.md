# Composition

Each facility is represented as a Platform-domain capability with explicit properties where provider qualification matters. Concrete providers live in EDP-Platform-* repositories. Higher-level domains consume these capabilities through Requirements rather than depending on provider SDKs.

The abstraction layer must never depend upward on higher EDP domains.
