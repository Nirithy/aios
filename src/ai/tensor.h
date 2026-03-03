#ifndef TENSOR_H
#define TENSOR_H

typedef struct {
    int rows;
    int cols;
    int *data;
} tensor_t;

// Create a tensor with the given dimensions
tensor_t* tensor_create(int rows, int cols);

// Add two tensors element-wise
tensor_t* tensor_add(tensor_t *a, tensor_t *b);

// Multiply two tensors (matrix multiplication)
tensor_t* tensor_matmul(tensor_t *a, tensor_t *b);

#endif
