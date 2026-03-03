# AI OS Project Plan

## Phase 1: Prototype (Completed)
- [x] Create project structure and documentation.
- [x] Set up a basic build system (`Makefile`, linker script).
- [x] Implement a multiboot-compliant basic bootloader in Assembly.
- [x] Implement a minimal C kernel that outputs text to VGA.
- [x] Create initial architectural stubs for AI-specific OS components (`ai_scheduler`, `gpu_manager`).
- [x] Implement actual multiboot memory map parsing in C.
- [x] Implement physical and virtual memory management optimized for large tensor allocations.
- [x] Implement multi-core processing support (SMP) for parallel scheduling.
- [x] Build a robust PCI/PCIe driver for discovering GPUs/NPUs.
- [x] Integrate a minimalistic math/tensor library in kernel-space.
- [x] Implement the AI task scheduler with compute-intensity awareness.
- [x] Create a basic user space for executing AI workloads.

## Phase 2: Transition to True OS Architecture
- [ ] **Transition to 64-bit (x86_64 or ARM64)**
  - Rewrite bootloader to set up Long Mode (PAE, 4-level paging, GDT).
  - Port existing 32-bit C code to 64-bit pointers and registers.
- [ ] **Advanced Memory Management**
  - Implement a true Virtual Memory Manager (VMM) with page tables for hardware isolation.
  - Add a kernel heap allocator (kmalloc/kfree) replacing the bump allocator.
- [ ] **Hardware SMP & Interrupts**
  - Parse ACPI/MADT tables to discover true hardware CPU topologies.
  - Implement APIC (Advanced Programmable Interrupt Controller) and IOAPIC.
  - Send INIT-SIPI-SIPI to wake up Application Processors (APs).
  - Implement IDT (Interrupt Descriptor Table) and Hardware Interrupts.
- [ ] **FPU & SIMD Support**
  - Initialize hardware FPU, SSE, AVX, or AMX instructions on all cores.
  - Enable saving/restoring FPU context during task switching.
- [ ] **True User Mode (Ring 3)**
  - Implement TSS (Task State Segment) for Ring 3 -> Ring 0 transitions.
  - Implement true system call boundaries using `sysenter`/`sysexit` or `syscall`/`sysret`.
  - Implement hardware context switching (saving/restoring full CPU state).
- [ ] **Filesystem and Storage (NVMe)**
  - Implement an NVMe driver for high-throughput model loading.
  - Build a basic read-only filesystem (e.g., ext2 or a custom AI-FS) to load models from disk.
- [ ] **Application Execution**
  - Implement an ELF loader to execute user-space binaries directly from disk.
- [ ] **Real GPU/NPU Drivers**
  - Expand the PCI driver to map GPU MMIO regions.
  - Write a basic compute driver for a target GPU/NPU to offload tensor operations.
