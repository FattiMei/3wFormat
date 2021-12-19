#include <stdio.h>
#include <stdlib.h>


#include "XYZ3W.h"
#include "memory_view.h"


long get_file_size(FILE *fp){
	long pos = ftell(fp),
	     result = 0;

	fseek(fp, 0, SEEK_END);
	result = ftell(fp);
	fseek(fp, pos, SEEK_SET);

	return result;
}


int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Invalid command-line arguments\n");
		return -1;
	}
	else{
		FILE *fp = NULL;
		fp = fopen(argv[1], "r");

		if(fp == NULL){
			fprintf(stderr, "Unable to open `%s`\n", argv[1]);
			return -1;
		}
		else{
			long size = get_file_size(fp);
			unsigned char *buf = (unsigned char *) malloc(size);

			if(buf == NULL){
				fprintf(stderr, "Failed memory allocation of %d bytes\n", size);
				return -1;
			}
			else{
				fread(buf, 1, size, fp);
				struct memory_view file_buf = {buf, size};
				struct memory_view mv = convert_3w_to_gcode(buf, decompose_3w_file(file_buf));

				if(mv.data != NULL)
					mv_dump_to_stream(mv, stdout);
				else
					fprintf(stderr, "File does not follow 3w specification V5");

				free(buf);
			}
		}
	}

	return 0;
}
