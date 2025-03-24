#include <stdint.h>
#include <stdio.h>

#define POOL_SIZE 1024
uint8_t memory_pool[POOL_SIZE];

typedef struct {
    uint32_t size;
} t_mem_info;
t_mem_info mem_info;

// Not necessary, but it's need for efficient.
void *head;

#define CONVERT_TO_POINTER(in)  (uint32_t *)(in)
#define CONVERT_TO_DATA(in)     (*(uint32_t *)(in))
#define ADD_POINTER(in, val)    (uint32_t *)((uint8_t *)(in) + (uint8_t)(val))

#define ALLOC   0xFFFF
#define FREE    0xFFEE

void init_memory_pool(void) {
    memory_pool[0] = memory_pool[POOL_SIZE - 1];
    memory_pool[1] = FREE;
    mem_info.size = POOL_SIZE - sizeof(uint32_t) * 2;
    head = memory_pool;
}

void* sm_mem_pool(size_t size) {
    uint32_t *curr = NULL;
    uint32_t *next = NULL;
    if (mem_info.size > size + sizeof(uint32_t) * 2) {
        next = ADD_POINTER(head, size + sizeof(uint32_t) * 2);
        *next = CONVERT_TO_DATA(head);
        *(next + 1) = FREE;
        curr = CONVERT_TO_POINTER(head);
        *curr = CONVERT_TO_DATA(next);
        *(curr + 1) = ALLOC;
        head = (void *)next;
        return curr + 2;
    }
    return NULL;
}

int main() {
    init_memory_pool();

    uint32_t* ptr1 = (uint32_t *)sm_mem_pool(100);
    if (ptr1 == NULL)
        return 1;
    uint32_t* ptr2 = (uint32_t *)sm_mem_pool(100);
    if (ptr2 == NULL)
        return 1;
    uint32_t* ptr3 = (uint32_t *)sm_mem_pool(100);
    if (ptr3 == NULL)
        return 1;

    *ptr1 = 123456789;
    printf("test value1: %d\n", *ptr1);
    *ptr2 = 987654321;
    printf("test value2: %d\n", *ptr2);
    *ptr3 = 456789123;
    printf("test value3: %d\n", *ptr3);
    return 0;
}
