#include <stdio.h>
#include "ftree.h"

int copy_ftree(const char* src, const char* dest){
    //traverse through the destination directory
    //check if the particular element is an ele
    struct stat info_src;
    struct stat info_dest;
    struct dirent *dp;
    if (lstat(src, &info_src)) == -1){
        perror("Might be an invalid source");
    }
    DIR *dir_ptr = opendir(dest);
    if(dir_ptr == NULL){
        perror("opendir");
    }
    while((dp = readdir(dir_ptr)) != NULL){
        if (lstat(dp.d_name, &info_dest) == -1){
             perror("Invalid destination directory");
        }
        //if(S_ISREG(info_dest.st_mode)){
        //1. Base case where the source is a regular file
        /*
             -The file exists - check the pathname of the destination to see if the file exists.
             //               -if the file exists, then check sizes,hash values
                              -
             -The file does not exist and completely new

        */
            if(S_ISREG(info_src.st_mode){
                //The file might exist
                FILE *f1_src = fopen(src, "rb");
                if(f1_src == NULL){
                    fprintf(stderr, "Error: could not open file\n");
                    return 1;
                }
                if(strcmp(dp.d_name, src) == 0){
                    FILE *f2_dest = fopen(dp.d_name, "wb")
                    //check the size-
                    if(info_src.st_size == info_dest.st_size){
                    	//compute hash
                    	const char* value_1 = hash(f1_src);
                    	const char* value_2 = hash(f2_src);
                    	//if the hash is not equal
                    	if (strcmp(value_1,value_2) == 0){
                        	int close = fclose(f2_dest);
                        	if(close !=0){
                            	fprintf(fprintf(stderr, "Error: fclose failed\n");
                            	exit (1);
                        }

                    }

                    
                    else{
                        //read from f1 using fread
                        //write to f2 using fwrite but with append function/ compare bit by bit to make changes in the middle
                        const *char data;
                        while(fread(data, sizeof(char), 1, f1_src) != 0){
                        	int error = fwrite(data, sizeof(char), 1, f1_dest);
                        	if(error != 1){
                        		fprintf(stderr, "Error: Could not write to file");
                        	}
                        }



                    }

        }
        
    }

    }

}


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage:\n\tfcopy SRC DEST\n");
        return 0;
    }
    int ret = copy_ftree(argv[1], argv[2]);
    if (ret < 0) {
        printf("Errors encountered during copy\n");
        ret = -ret;
    } else {
        printf("Copy completed successfully\n");
    }
    printf("%d processes used\n", ret);
    
    return 0;
}

