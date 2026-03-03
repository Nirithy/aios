#ifndef GPU_MANAGER_H
#define GPU_MANAGER_H

// Initialize the GPU/NPU Resource Manager
void gpu_manager_init(void);

// Allocate memory on the GPU (Stub)
int allocate_gpu_memory(int tensor_size);

#endif
