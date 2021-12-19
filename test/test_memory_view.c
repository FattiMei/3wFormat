#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "memory_view.h"


void setUp(){
}


void tearDown(){
}


void test_mv_slice(){
	struct memory_view a = mv_from_string("0123456789ABCDEF");

	TEST_ASSERT_TRUE(mv_equals(mv_slice(a, 3, 7), mv_from_string("3456")));
	TEST_ASSERT_TRUE(mv_equals(mv_slice(a, 3, 70), mv_from_string("3456789ABCDEF")));

	TEST_ASSERT_TRUE(mv_equals(mv_slice_from_offset_to_end(a, 3), mv_from_string("3456789ABCDEF")));

	TEST_ASSERT_TRUE(mv_equals(mv_slice_from_start_to_offset(a, 5), mv_from_string("01234")));
}


void test_mv_index(){
	TEST_ASSERT_EQUAL_UINT32(6, mv_index(mv_from_string("0123456789"), '6'));
	TEST_ASSERT_EQUAL_UINT32(-1, mv_index(mv_from_string("0123456789"), 'G'));
}


void test_mv_index_slice(){
	TEST_ASSERT_EQUAL_UINT32(9, mv_index_slice(mv_from_string("0123456789"), '9', 4, 10));
	TEST_ASSERT_EQUAL_UINT32(-1, mv_index_slice(mv_from_string("0123456789"), '0', 1, 4));
}


void test_mv_count(){
	const char *s = "Apelle figlio d'Apollo fece una palla di pelle di pollo";
	TEST_ASSERT_EQUAL_UINT32(11, mv_count(mv_from_string(s), 'l'));
}


void test_mv_equals(){
	TEST_ASSERT_TRUE(mv_equals(mv_from_string("Some bytes of memory..."),
				   mv_from_string("Some bytes of memory...")));

	TEST_ASSERT_FALSE(mv_equals(mv_from_string("Some bytes of memory..."),
				    mv_from_string("Some other bytes of memory...")));

	TEST_ASSERT_FALSE(mv_equals(mv_from_string("Ciao, Matteo"),
				    mv_from_string("Ciao, Mattia")));
}


void test_mv_startswith(){
	TEST_ASSERT_TRUE(mv_startswith(	mv_from_string("+39 000000000"),
					mv_from_string("+39")));

	TEST_ASSERT_FALSE(mv_startswith(mv_from_string("Lord Byron"),
				       	mv_from_string("Lady")));
}


void test_mv_endswith(){
	TEST_ASSERT_TRUE(mv_endswith(mv_from_string("filename.txt"),
				     mv_from_string(".txt")));

	TEST_ASSERT_FALSE(mv_endswith(mv_from_string("filename.txt"),
				      mv_from_string(".md")));
}


int main(){
	UNITY_BEGIN();


	RUN_TEST(test_mv_slice);
	RUN_TEST(test_mv_equals);
	RUN_TEST(test_mv_index);
	RUN_TEST(test_mv_index_slice);
	RUN_TEST(test_mv_count);
	RUN_TEST(test_mv_startswith);
	RUN_TEST(test_mv_endswith);


	return UNITY_END();
}
