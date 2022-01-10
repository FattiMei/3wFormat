#include <stdio.h>
#include <string.h>
#include "unity.h"

#include "../src/crc32.h"
#include "../src/memory_view.h"


void setUp(){
}


void tearDown(){
}


void test_crc32(){
	TEST_ASSERT_EQUAL_UINT32(0, 		crc32(0, NULL, 0));
	TEST_ASSERT_EQUAL_UINT32(0x414fa339, 	crc32(0, "The quick brown fox jumps over the lazy dog", 43));
	TEST_ASSERT_EQUAL_UINT32(0x0c877f61, 	crc32(0, "Test vector from febooti.com", 28));
}


int main(){
	UNITY_BEGIN();


	RUN_TEST(test_crc32);


	return UNITY_END();
}
