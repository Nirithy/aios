#ifndef SMP_H
#define SMP_H

#define MAX_CPUS 16

typedef struct {
    int id;
    int is_online;
    int current_task_id;
} cpu_core_t;

extern cpu_core_t cpus[MAX_CPUS];
extern int num_cpus_online;

// Initialize the SMP subsystem (discover cores)
void smp_init();

// Get the current CPU core ID (stub)
int smp_get_current_core();

#endif
