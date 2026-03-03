#include "multiboot.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

unsigned short *vga_buffer;
int current_vga_row = 0;
int current_vga_col = 0;

void clear_screen() {
    vga_buffer = (unsigned short *)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (unsigned short)' ' | (0x0F << 8); // White on black
    }
    current_vga_row = 0;
    current_vga_col = 0;
}

void print_string(const char *str) {
    vga_buffer = (unsigned short *)VGA_ADDRESS;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            current_vga_row++;
            current_vga_col = 0;
        } else {
            vga_buffer[current_vga_row * VGA_WIDTH + current_vga_col] = (unsigned short)str[i] | (0x0F << 8);
            current_vga_col++;
            if (current_vga_col >= VGA_WIDTH) {
                current_vga_col = 0;
                current_vga_row++;
            }
        }
        i++;
    }
}

void print_hex(unsigned long val) {
    char buffer[9];
    buffer[8] = '\0';
    for (int i = 7; i >= 0; i--) {
        unsigned char digit = val & 0xF;
        if (digit < 10) {
            buffer[i] = '0' + digit;
        } else {
            buffer[i] = 'A' + (digit - 10);
        }
        val >>= 4;
    }
    print_string("0x");
    print_string(buffer);
}

void kernel_main(unsigned long magic, unsigned long addr) {
    clear_screen();
    print_string("Hello AI OS - Booting the Future of Computing\n");

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_string("Invalid magic number: ");
        print_hex(magic);
        print_string("\n");
        return;
    }

    multiboot_info_t *mbi = (multiboot_info_t *)addr;

    if (mbi->flags & (1 << 6)) {
        print_string("Memory map:\n");
        memory_map_t *mmap = (memory_map_t *)mbi->mmap_addr;
        while ((unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length) {
            print_string("Base: ");
            print_hex(mmap->base_addr_high);
            print_hex(mmap->base_addr_low);
            print_string(" Length: ");
            print_hex(mmap->length_high);
            print_hex(mmap->length_low);
            print_string(" Type: ");
            print_hex(mmap->type);
            print_string("\n");

            mmap = (memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size));
        }
    } else {
        print_string("Memory map not provided by bootloader\n");
    }

    // In a real OS, we'd initialize the AI scheduler and hardware here.
    // ai_scheduler_init();
    // gpu_manager_init();
}
