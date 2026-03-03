// Stub for an AI-centric Process Scheduler
#include "ai_scheduler.h"
#include "../../kernel/tty.h"
#include "../../kernel/smp.h"

#define MAX_TASKS 16

ai_task_t task_queue[MAX_TASKS];
int current_task_id = 0;

void ai_scheduler_init() {
    print_string("Initializing AI Scheduler...\n");
    for (int i = 0; i < MAX_TASKS; i++) {
        task_queue[i].is_active = 0;
    }
}

void schedule_inference_task(void *model, void *input_data, int compute_intensity) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!task_queue[i].is_active) {
            task_queue[i].task_id = ++current_task_id;
            task_queue[i].compute_intensity = compute_intensity;
            task_queue[i].model_ptr = model;
            task_queue[i].input_data_ptr = input_data;
            task_queue[i].is_active = 1;
            task_queue[i].assigned_core = -1; // Not yet assigned

            print_string("Scheduled Inference Task ID ");
            print_hex(task_queue[i].task_id);
            print_string(" with Compute Intensity: ");
            print_hex(compute_intensity);
            print_string("\n");
            return;
        }
    }
    print_string("Failed to schedule task: Queue full.\n");
}

void ai_scheduler_tick() {
    // Basic SMP scheduling loop: try to assign available tasks to idle cores
    for (int core_id = 0; core_id < num_cpus_online; core_id++) {
        if (cpus[core_id].current_task_id == 0) {
            // Find highest intensity unassigned task
            int highest_intensity_idx = -1;
            int max_intensity = -1;

            for (int i = 0; i < MAX_TASKS; i++) {
                if (task_queue[i].is_active && task_queue[i].assigned_core == -1) {
                    if (task_queue[i].compute_intensity > max_intensity) {
                        max_intensity = task_queue[i].compute_intensity;
                        highest_intensity_idx = i;
                    }
                }
            }

            if (highest_intensity_idx != -1) {
                // Assign task to this core
                task_queue[highest_intensity_idx].assigned_core = core_id;
                cpus[core_id].current_task_id = task_queue[highest_intensity_idx].task_id;

                print_string("Core ");
                print_hex(core_id);
                print_string(" Executing AI Task ID ");
                print_hex(task_queue[highest_intensity_idx].task_id);
                print_string(" (Intensity: ");
                print_hex(task_queue[highest_intensity_idx].compute_intensity);
                print_string(")\n");

                // Simulate execution completion (immediately frees up the core/task in this stub)
                task_queue[highest_intensity_idx].is_active = 0;
                cpus[core_id].current_task_id = 0;
            }
        }
    }
}