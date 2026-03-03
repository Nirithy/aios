#include "smp.h"
#include "tty.h"

cpu_core_t cpus[MAX_CPUS];
int num_cpus_online = 0;

void smp_init() {
    print_string("Initializing SMP (Symmetric Multiprocessing)...\n");

    // In a real OS, we would parse ACPI MADT to find Local APICs
    // and send INIT-SIPI-SIPI sequences to wake up APs.
    // For this prototype, we'll simulate discovering 4 cores.

    num_cpus_online = 4;

    for (int i = 0; i < num_cpus_online; i++) {
        cpus[i].id = i;
        cpus[i].is_online = 1;
        cpus[i].current_task_id = 0; // 0 means idle

        print_string("CPU Core ");
        print_hex(i);
        print_string(" is online.\n");
    }

    print_string("SMP Init Complete. Online cores: ");
    print_hex(num_cpus_online);
    print_string("\n");
}

int smp_get_current_core() {
    // In a real OS, we'd read the Local APIC ID.
    // Here we just return 0 (BSP) as a stub since we are not fully
    // implementing true parallel execution environments with local APIC yet.
    return 0;
}
