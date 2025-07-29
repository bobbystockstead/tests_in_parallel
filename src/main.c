#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parallel.h"

int main(void) {
    printf("=== Parallel Test Runner Demo ===\n");
    printf("Demonstrating OS-level parallel test execution abstraction\n\n");
    
    // Initialize random seed for test simulation
    srand(time(NULL));
    
    // Create a test suite
    struct Test tests[QUEUEMAX];
    struct Test_Queue queue;
    
    // Set up 12 tests to demonstrate parallelization
    queue.queue_length = 400;
    
    for (int i = 0; i < queue.queue_length; i++) {
        tests[i].test_id = i + 1;
        tests[i].result = ERROR; // Initialize to ERROR state
    }
    
    printf("Created test suite with %d tests\n", queue.queue_length);
    printf("Each test will be executed in parallel using optimal thread count\n\n");
    
    // This is the main API call - engineers only need to call this function
    // All parallelization, thread management, and optimization is handled internally
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int result = run_test_in_parallel(tests, queue);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    printf("\n=== Final Results ===\n");
    printf("Total execution time: %.2f seconds\n", execution_time);
    printf("Overall result: %s\n", result == 0 ? "SUCCESS" : "FAILURE");
    
    // printf("\nIndividual test results:\n");
    // for (int i = 0; i < queue.queue_length; i++) {
    //     printf("Test %d: %s\n", 
    //            tests[i].test_id,
    //            tests[i].result == SUCCESS ? "PASS" :
    //            tests[i].result == FAILURE ? "FAIL" : "ERROR");
    // }
    
    // printf("\n=== Abstraction Benefits ===\n");
    // printf("✓ Engineers don't need to understand threading\n");
    // printf("✓ Optimal thread count calculated automatically\n");
    // printf("✓ Load balancing handled internally\n");
    // printf("✓ Cross-platform compatibility (Linux/Windows)\n");
    // printf("✓ Simple API: just pass tests array and queue\n");
    
    return 0;
}
