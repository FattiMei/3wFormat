#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "../src/crc32.h"


void setUp(){
}


void tearDown(){
}


void test_crc32(){
	TEST_ASSERT_EQUAL_UINT32(0, crc32(0, NULL, 0));
}


int main(){
	UNITY_BEGIN();


	RUN_TEST(test_crc32);


	return UNITY_END();
}
