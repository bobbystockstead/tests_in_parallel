#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_sample(void) {
    TEST_ASSERT_EQUAL_INT(1, 1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sample);
    return UNITY_END();
}
