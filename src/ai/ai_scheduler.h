#ifndef AI_SCHEDULER_H
#define AI_SCHEDULER_H

typedef struct {
    int task_id;
    int compute_intensity; // e.g., FLOPS required, memory bandwidth needed, 1-100 scale
    int is_active;
    void *model_ptr;
    void *input_data_ptr;
    int assigned_core; // Core ID assigned to execute this task
} ai_task_t;

// Initialize the AI scheduler
void ai_scheduler_init(void);

// Schedule a new inference task
void schedule_inference_task(void *model, void *input_data, int compute_intensity);

// Execute the next task in the queue (simple round-robin prioritizing high intensity)
void ai_scheduler_tick(void);

#endif
