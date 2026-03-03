CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

AS = nasm
ASFLAGS = -f elf

LD = ld
LDFLAGS = -T linker.ld -m elf_i386

all: ai_os.bin

boot/boot.o: boot/boot.s
	$(AS) $(ASFLAGS) $< -o $@

kernel/tty.o: kernel/tty.c
	$(CC) $(CFLAGS) $< -o $@

kernel/memory.o: kernel/memory.c
	$(CC) $(CFLAGS) $< -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) $< -o $@

kernel/pci.o: kernel/pci.c
	$(CC) $(CFLAGS) $< -o $@

kernel/ai_scheduler.o: src/ai/ai_scheduler.c
	$(CC) $(CFLAGS) $< -o $@

kernel/gpu_manager.o: src/ai/gpu_manager.c
	$(CC) $(CFLAGS) $< -o $@

OBJS = boot/boot.o kernel/tty.o kernel/memory.o kernel/kernel.o kernel/pci.o kernel/ai_scheduler.o kernel/gpu_manager.o

ai_os.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -f $(OBJS) ai_os.bin

run: ai_os.bin
	qemu-system-i386 -kernel ai_os.bin
