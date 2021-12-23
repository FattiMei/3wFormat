#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "../src/memory_view.h"


void setUp(){
}


void tearDown(){
}


void test_mv_slice(){
	struct memory_view a = mv_from_cstring("0123456789ABCDEF");

	TEST_ASSERT_TRUE(mv_equals(mv_slice(a, 3, 7), mv_from_cstring("3456")));
	TEST_ASSERT_TRUE(mv_equals(mv_slice(a, 3, 70), mv_from_cstring("3456789ABCDEF")));

	TEST_ASSERT_TRUE(mv_equals(mv_drop_first(a, 3), mv_from_cstring("3456789ABCDEF")));

	TEST_ASSERT_TRUE(mv_equals(mv_take_first(a, 5), mv_from_cstring("01234")));
}


void test_mv_index(){
	size_t i = 0;

	TEST_ASSERT_TRUE(mv_index(mv_from_cstring("0123456789"), '6', NULL));
	TEST_ASSERT_FALSE(mv_index(mv_from_cstring("0123456789"), 'G', NULL));
	
	TEST_ASSERT_TRUE(mv_index(mv_from_cstring("0123456789"), '6', &i));
	TEST_ASSERT_EQUAL(6, i);
}


void test_mv_count(){
	const char *s = "Apelle figlio d'Apollo fece una palla di pelle di pollo";
	TEST_ASSERT_EQUAL_UINT32(11, mv_count(mv_from_cstring(s), 'l'));
}


void test_mv_equals(){
	TEST_ASSERT_TRUE(mv_equals(mv_from_cstring("Some bytes of memory..."),
				   mv_from_cstring("Some bytes of memory...")));

	TEST_ASSERT_FALSE(mv_equals(mv_from_cstring("Some bytes of memory..."),
				    mv_from_cstring("Some other bytes of memory...")));

	TEST_ASSERT_FALSE(mv_equals(mv_from_cstring("Ciao, Matteo"),
				    mv_from_cstring("Ciao, Mattia")));

	TEST_ASSERT_FALSE(mv_equals(mv_from_cstring("Hello, world!"),
				    MV_NULL));
}


void test_mv_startswith(){
	TEST_ASSERT_TRUE(mv_startswith(	mv_from_cstring("+39 000000000"),
					mv_from_cstring("+39")));

	TEST_ASSERT_FALSE(mv_startswith(mv_from_cstring("Lord Byron"),
				       	mv_from_cstring("Lady")));
}


void test_mv_endswith(){
	TEST_ASSERT_TRUE(mv_endswith(mv_from_cstring("filename.txt"),
				     mv_from_cstring(".txt")));

	TEST_ASSERT_FALSE(mv_endswith(mv_from_cstring("filename.txt"),
				      mv_from_cstring(".md")));
}


int main(){
	UNITY_BEGIN();


	RUN_TEST(test_mv_slice);
	RUN_TEST(test_mv_equals);
	RUN_TEST(test_mv_index);
	RUN_TEST(test_mv_count);
	RUN_TEST(test_mv_startswith);
	RUN_TEST(test_mv_endswith);


	return UNITY_END();
}
