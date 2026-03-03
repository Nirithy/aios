#include "tty.h"

// We need outb/inb for serial output
static inline void serial_outb(unsigned short port, unsigned char val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned char serial_inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static void write_serial(char a) {
    while ((serial_inb(0x3F8 + 5) & 0x20) == 0);
    serial_outb(0x3F8, a);
}

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
        write_serial(str[i]);
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
