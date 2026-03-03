# AI OS Project Plan

- [x] Create project structure and documentation.
- [x] Set up a basic build system (`Makefile`, linker script).
- [x] Implement a multiboot-compliant basic bootloader in Assembly.
- [x] Implement a minimal C kernel that outputs text to VGA.
- [x] Create initial architectural stubs for AI-specific OS components (`ai_scheduler`, `gpu_manager`).
- [x] Implement actual multiboot memory map parsing in C.
- [ ] Implement physical and virtual memory management optimized for large tensor allocations.
- [ ] Implement multi-core processing support (SMP) for parallel scheduling.
- [ ] Build a robust PCI/PCIe driver for discovering GPUs/NPUs.
- [ ] Integrate a minimalistic math/tensor library in kernel-space.
- [ ] Implement the AI task scheduler with compute-intensity awareness.
- [ ] Create a basic user space for executing AI workloads.
