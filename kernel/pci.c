#include "pci.h"
#include "io.h"
#include "tty.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

unsigned int pci_config_read_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset) {
    unsigned int address;
    unsigned int lbus  = (unsigned int)bus;
    unsigned int lslot = (unsigned int)slot;
    unsigned int lfunc = (unsigned int)func;
    unsigned int loffset = (unsigned int)(offset & 0xFC);

    address = (unsigned int)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | loffset | ((unsigned int)0x80000000));

    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

unsigned short pci_get_vendor_id(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned int r0 = pci_config_read_dword(bus, slot, func, 0);
    return (unsigned short)(r0 & 0xFFFF);
}

unsigned short pci_get_device_id(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned int r0 = pci_config_read_dword(bus, slot, func, 0);
    return (unsigned short)((r0 >> 16) & 0xFFFF);
}

unsigned char pci_get_class_id(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned int r2 = pci_config_read_dword(bus, slot, func, 0x08);
    return (unsigned char)((r2 >> 24) & 0xFF);
}

unsigned char pci_get_subclass_id(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned int r2 = pci_config_read_dword(bus, slot, func, 0x08);
    return (unsigned char)((r2 >> 16) & 0xFF);
}

unsigned char pci_get_header_type(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned int r3 = pci_config_read_dword(bus, slot, func, 0x0C);
    return (unsigned char)((r3 >> 16) & 0xFF);
}

void pci_check_function(unsigned char bus, unsigned char slot, unsigned char func) {
    unsigned short vendor = pci_get_vendor_id(bus, slot, func);
    if (vendor == 0xFFFF) return; // Device doesn't exist

    unsigned short device = pci_get_device_id(bus, slot, func);
    unsigned char class_id = pci_get_class_id(bus, slot, func);
    unsigned char subclass_id = pci_get_subclass_id(bus, slot, func);

    print_string("PCI Device: Bus ");
    print_hex(bus);
    print_string(" Slot ");
    print_hex(slot);
    print_string(" Func ");
    print_hex(func);
    print_string("\n  Vendor: ");
    print_hex(vendor);
    print_string(" Device: ");
    print_hex(device);
    print_string(" Class: ");
    print_hex(class_id);
    print_string(" Subclass: ");
    print_hex(subclass_id);
    print_string("\n");

    // Potential AI Accelerators:
    // Class 0x03 is Display Controller (VGA, 3D, etc)
    // Class 0x12 is Processing Accelerator (e.g. some TPUs/NPUs)
    // Class 0x0B is Processors (Coprocessor)
    if (class_id == 0x03) {
        print_string("  -> Found Display/GPU Controller!\n");
    } else if (class_id == 0x12 || class_id == 0x0B) {
        print_string("  -> Found AI/Coprocessor Accelerator!\n");
    }
}

void pci_check_device(unsigned char bus, unsigned char slot) {
    unsigned short vendor = pci_get_vendor_id(bus, slot, 0);
    if (vendor == 0xFFFF) return; // Device doesn't exist

    pci_check_function(bus, slot, 0);

    unsigned char header_type = pci_get_header_type(bus, slot, 0);
    if ((header_type & 0x80) != 0) {
        // It is a multi-function device, so check remaining functions
        for (unsigned char func = 1; func < 8; func++) {
            if (pci_get_vendor_id(bus, slot, func) != 0xFFFF) {
                pci_check_function(bus, slot, func);
            }
        }
    }
}

void pci_check_bus(unsigned char bus) {
    for (unsigned char slot = 0; slot < 32; slot++) {
        pci_check_device(bus, slot);
    }
}

void pci_init(void) {
    print_string("Initializing PCI Subsystem...\n");
    // Brute-force scanning all buses (simplest approach for minimal OS)
    for (unsigned short bus = 0; bus < 256; bus++) {
        pci_check_bus((unsigned char)bus);
    }
    print_string("PCI Scan Complete.\n");
}
