#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parallel.h"

struct TestResult {
    double seq_time;
    double par_time;
    double speedup;
};

struct TestResult run_test_suite(int num_tests) {
    struct TestResult result;
    struct Test tests[QUEUEMAX];
    struct Test_Queue queue;

    queue.queue_length = num_tests;

    for (int i = 0; i < queue.queue_length; i++) {
        tests[i].test_id = i + 1;
        tests[i].result = ERROR;
    }

    // Run tests sequentially
    struct timespec start_seq, end_seq;
    clock_gettime(CLOCK_MONOTONIC, &start_seq);
    run_test_sequentially(tests, queue);
    clock_gettime(CLOCK_MONOTONIC, &end_seq);
    result.seq_time = (end_seq.tv_sec - start_seq.tv_sec) +
        (end_seq.tv_nsec - start_seq.tv_nsec) / 1e9;

    // Run tests in parallel
    struct timespec start_par, end_par;
    clock_gettime(CLOCK_MONOTONIC, &start_par);
    run_test_in_parallel(tests, queue);
    clock_gettime(CLOCK_MONOTONIC, &end_par);
    result.par_time = (end_par.tv_sec - start_par.tv_sec) +
        (end_par.tv_nsec - start_par.tv_nsec) / 1e9;

    result.speedup = result.seq_time / result.par_time;

    return result;
}

int main(void) {
    srand(time(NULL));

    FILE *fp = fopen("results.csv", "w");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file results.csv for writing.\n");
        return 1;
    }

    int test_counts[] = { 10, 25, 50, 100, 500 };
    int num_test_counts = sizeof(test_counts) / sizeof(int);

    // Print CSV header to file
    fprintf(fp, "Test run #");
    for (int i = 0; i < num_test_counts; i++) {
        fprintf(fp, ",%d sequential,%d parallel,%d speedup",
            test_counts[i], test_counts[i], test_counts[i]);
    }
    fprintf(fp, "\n");

    // Run the test suites 10 times
    for (int run = 1; run <= 10; run++) {
        fprintf(fp, "%d", run);
        for (int i = 0; i < num_test_counts; i++) {
            struct TestResult result = run_test_suite(test_counts[i]);
            fprintf(fp, ",%.4f,%.4f,%.2f", result.seq_time, result.par_time, result.speedup);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Test results successfully written to results.csv\n");

    return 0;
}
