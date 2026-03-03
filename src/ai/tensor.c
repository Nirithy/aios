#include "tensor.h"
#include "../../kernel/memory.h"
#include "../../kernel/tty.h"

tensor_t* tensor_create(int rows, int cols) {
    // Allocate space for the tensor structure
    // Since our memory allocator only does 4KB pages currently via pmm_alloc_page
    // or large blocks via allocate_tensor_memory, we use the simple page allocator for small metadata.
    // In a real system, we'd have a kernel heap (kmalloc).

    // As a hack for this minimal kernel, let's use the bump allocator for small objects too.
    // Since allocate_tensor_memory page-aligns everything, it wastes memory for small structs,
    // but works fine for prototyping.

    tensor_t *t = (tensor_t *)allocate_tensor_memory(sizeof(tensor_t));
    if (!t) return 0;

    t->rows = rows;
    t->cols = cols;

    // Allocate space for the data array (using int to avoid FPU for now)
    t->data = (int *)allocate_tensor_memory(rows * cols * sizeof(int));
    if (!t->data) return 0;

    // Initialize to 0
    for (int i = 0; i < rows * cols; i++) {
        t->data[i] = 0;
    }

    return t;
}

tensor_t* tensor_add(tensor_t *a, tensor_t *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) {
        print_string("Tensor Add Error: Dimension mismatch or null tensor.\n");
        return 0;
    }

    tensor_t *result = tensor_create(a->rows, a->cols);
    if (!result) return 0;

    for (int i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }

    return result;
}

tensor_t* tensor_matmul(tensor_t *a, tensor_t *b) {
    if (!a || !b || a->cols != b->rows) {
        print_string("Tensor MatMul Error: Dimension mismatch or null tensor.\n");
        return 0;
    }

    tensor_t *result = tensor_create(a->rows, b->cols);
    if (!result) return 0;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            int sum = 0;
            for (int k = 0; k < a->cols; k++) {
                sum += a->data[i * a->cols + k] * b->data[k * b->cols + j];
            }
            result->data[i * b->cols + j] = sum;
        }
    }

    return result;
}
