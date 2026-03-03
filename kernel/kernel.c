#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

unsigned short *vga_buffer;

void clear_screen() {
    vga_buffer = (unsigned short *)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (unsigned short)' ' | (0x0F << 8); // White on black
    }
}

void print_string(const char *str) {
    vga_buffer = (unsigned short *)VGA_ADDRESS;
    int i = 0;
    while (str[i] != '\0') {
        vga_buffer[i] = (unsigned short)str[i] | (0x0F << 8); // White text
        i++;
    }
}

void kernel_main() {
    clear_screen();
    print_string("Hello AI OS - Booting the Future of Computing");

    // In a real OS, we'd initialize the AI scheduler and hardware here.
    // ai_scheduler_init();
    // gpu_manager_init();
}
