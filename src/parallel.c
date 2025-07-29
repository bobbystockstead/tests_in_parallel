#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif
#include "parallel.h"

// Internal structure to track execution statistics
struct ParallelStats {
    int total_tests;
    int passed;
    int failed;
    int errors;
    double execution_time;
};

// Get optimal number of worker threads based on system capabilities
static int get_optimal_thread_count() {
#ifdef _WIN32	
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#else	
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    // Use all cores, but cap at a reasonable maximum to avoid thread overhead
    return (num_cores > 16) ? 16 : num_cores;
#endif
}

// Main parallel execution engine - the core abstraction
int run_test_in_parallel(struct Test tests[QUEUEMAX], struct Test_Queue test_queue) {
    if (test_queue.queue_length <= 0) {
        printf("No tests to run.\n");
        return 0;
    }

    int num_workers = get_optimal_thread_count();
    int total = test_queue.queue_length;
    
    printf("Parallel Test Runner: Executing %d tests using %d worker threads\n", 
           total, num_workers);

#ifndef _WIN32
    int i = 0;
    while (i < total) {
        // Process tests in batches to match available cores
        int batch_size = (total - i) < num_workers ? (total - i) : num_workers;
        pthread_t threads[batch_size];
        
        // Spawn worker threads for current batch
        for (int t = 0; t < batch_size; t++) {
            if (pthread_create(&threads[t], NULL, run_test, (void*)&tests[i + t]) != 0) {
                fprintf(stderr, "Error creating thread for test %d\n", tests[i + t].test_id);
                tests[i + t].result = ERROR;
            }
        }
        
        // Wait for all threads in current batch to complete
        for (int t = 0; t < batch_size; t++) {
            pthread_join(threads[t], NULL);
        }
        
        i += batch_size;
    }
#else
    // Windows implementation using CreateThread
    // TODO: Implement Windows threading support
    printf("Windows parallel execution not yet implemented\n");
    return -1;
#endif

    // Report execution summary
    int passed = 0, failed = 0, errors = 0;
    for (int j = 0; j < total; j++) {
        switch(tests[j].result) {
            case SUCCESS: passed++; break;
            case FAILURE: failed++; break;
            case ERROR: errors++; break;
        }
    }
    
    printf("Execution Complete: %d passed, %d failed, %d errors\n", 
           passed, failed, errors);
    
    return (failed + errors == 0) ? 0 : 1;
}// Test execution function - this is what gets called by each worker thread
void* run_test(void* arg) {
    struct Test* test = (struct Test*)arg;
    
    // Simulate test execution - replace with actual test logic
    //printf("Executing test %d on thread %p\n", test->test_id, (void*)pthread_self());
    
    // TODO: Replace this with actual test execution logic
    // This could involve:
    // - Running external test executables
    // - Calling test functions
    // - Capturing output and return codes
    
    // For now, simulate some work and random results
    usleep(100000 + (rand() % 500000)); // Random delay between 100-600ms
    
    // Simulate test results (replace with real logic)
    int random_result = rand() % 10;
    if (random_result < 7) {
        test->result = SUCCESS;
    } else if (random_result < 9) {
        test->result = FAILURE;
    } else {
        test->result = ERROR;
    }
    
    // printf("Test %d completed with result: %s\n", 
    //        test->test_id, 
    //        test->result == SUCCESS ? "PASS" : 
    //        test->result == FAILURE ? "FAIL" : "ERROR");
    
    return NULL;
}