#define QUEUEMAX 1000

struct Test {
	int test_id;
	int result;
};

enum Result {
	SUCCESS,
	FAILURE,
	ERROR
};

struct Test_Queue {
	int queue_length;
};

// Main API function - this is the core abstraction for parallel test execution
int run_test_in_parallel(struct Test tests[QUEUEMAX], struct Test_Queue test_queue);

// Sequential execution for comparison
int run_test_sequentially(struct Test tests[QUEUEMAX], struct Test_Queue test_queue);

// Worker function called by each thread (internal use)
void* run_test(void* arg);
