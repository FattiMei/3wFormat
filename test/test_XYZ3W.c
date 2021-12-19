#include <stdlib.h>
#include "unity.h"
#include "XYZ3W.h"
#include "memory_view.h"

void setUp(){
}


void tearDown(){
}


void test_header_packing(){
	unsigned char buf[HEADER_LEN];
	struct memory_view header = pack_header(buf, 0, 0);

	TEST_ASSERT_TRUE(mv_equals(header, (struct memory_view){HEADER_V5_TEMPLATE, HEADER_LEN}));
}


void test_header_unpacking(){
	unsigned char buf[HEADER_LEN];
	const unsigned int PRINT_INFO = 23598, CRC32 = 73897;

	struct header_info info = unpack_header(pack_header(buf, PRINT_INFO, CRC32));

	TEST_ASSERT_EQUAL_HEX32(PRINT_INFO, info.print_info_len);
	TEST_ASSERT_EQUAL_HEX32(CRC32, info.crc32);
}


void test_header_validation(){
	unsigned char buf[HEADER_LEN];

	TEST_ASSERT_TRUE(validate_header((struct memory_view){HEADER_V5_TEMPLATE, HEADER_LEN}));
	TEST_ASSERT_TRUE(validate_header(pack_header(buf, 512, 1024)));
}


int main(){
	UNITY_BEGIN();


	RUN_TEST(test_header_packing);
	RUN_TEST(test_header_unpacking);
	RUN_TEST(test_header_validation);


	return UNITY_END();
}
