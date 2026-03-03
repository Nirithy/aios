# AI OS

This is a prototype bare-metal operating system specifically designed for AI workloads.

## Vision
The AI OS aims to provide:
- Native hardware acceleration management (GPUs, TPUs, NPUs).
- Tensor-optimized memory management.
- An AI-centric process scheduler that understands compute/memory intensity of deep learning tasks.

## Getting Started
To build the project, run:
```bash
make
```

To run the OS in QEMU:
```bash
make run
```
