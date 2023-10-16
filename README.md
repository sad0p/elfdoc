# elfdoc
Proof of concept heuristic detection for common ELF infection algorithms.

# Detection capability

- Entry point modification.
- Embedded payloads.
- Mangled Section Header.
- PT_NOTE infection.

# Build (requires cmake)

`mkdir build; cd build; cmake .. ; make`

# Run

Single file 

`elfdoc <path-to-file>`

Directory scan bash for-loop.

``` for file in .; do elfdoc $file ; done ```
