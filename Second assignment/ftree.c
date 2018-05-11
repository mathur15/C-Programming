#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "ftree.h"
#include "hash.h"
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/wait.h>

//performing the copying process using a helper function to simplify code

void write_data(FILE *source, FILE *destination){
	char *c;
	while(fread(c, sizeof(char), 1, source) != 0 ){
		int error = fwrite(&c, sizeof(char), 1, destination);
		if(error == 0){
			fprintf(stderr,"Character could not be written");					
		}
	}
}

int copy_ftree(const char* src, const char* dest){
				int count =0;
				struct stat info_src;
				struct dirent *dp;
				if (lstat(src, &info_src) == -1){
						perror("Might be an invalid source");
						exit(-1);
    			}
    			
    			//creating a pathname in the destination with src as the file
				char *dest_pathname = malloc(sizeof(dest) + sizeof(basename((char *)src)) + 2);
				strcat(dest_pathname,dest);
				strcat(dest_pathname,"/");
				strcat(dest_pathname,basename((char*)src));
				
				//case 1: where the source is regular file - base case
				if(S_ISREG(info_src.st_mode)){
						
 						//checking if the file exists in the destination
						//creating file pointers for the same file in the source and destination
						FILE *f_src = fopen(src, "rb");
						FILE *f_dest = fopen(dest_pathname, "rb");
						if(f_dest == NULL){
								//file does not exist in the destination
							   //fclose(f_dest);
							   
								f_dest = fopen(dest_pathname,"wb");
								write_data(f_src,f_dest);
						}
						
						else if(f_dest != NULL){
								//in this a a file exists at the source and the destination
								//comparing of sizes and hashes in this phase- will require another struct stat for the file in the destination
								struct stat file_dest;
								if(lstat(dest_pathname, &file_dest) == -1)
									{
											perror("lstat");
									}
								//condition for copying
								const char* value_1 = hash(f_src);
								const char* value_2 = hash(f_dest);
								if(file_dest.st_size != info_src.st_size ||((file_dest.st_size == info_src.st_size) && (strcmp(value_1,value_2) != 0))){
										write_data(f_src, f_dest);
								}
						}
				}
				if(S_ISDIR(info_src.st_mode)){
							//create a pathname for the source directory in the destination to check if the exists or not in the destination
							//checking if the directory exists. CREATE ONE if it does not exist
							//traverse through the SOURCE directory using struct dirent
							//recursion if a regular file is reached
							//fork if the dirent is a directory
							
							
							//DIR *src_dir = opendir(src);
							//do error checking here
							
							mkdir(dest_pathname,(info_src.st_mode & 0777));
							DIR *src_dir = opendir(src);
							
							while((dp = readdir(src_dir)) != NULL){
								   
									if((*dp).d_name[0] != '.'){
										char *src_path = malloc(sizeof(src) + sizeof((*dp).d_name) + 2);
								   	strcat(src_path,src);
								   	strcat(src_path,"/");
								   	strcat(src_path,(*dp).d_name);
											if((*dp).d_type == DT_REG){//regular file
												//keep recursing
												copy_ftree(src_path, dest_pathname);			
											}
											else if((*dp).d_type == DT_DIR){
												int result = fork();
												if(result == 0){
														//Parent process	
 														int status, pid;
														if ((pid = wait(&status)) == -1) {
															fprintf(stderr, "no child to wait for");
															exit(-1);
													}
											count += WEXITSTATUS(status);
											}
											else{ //Child Process
													int e_status = copy_ftree(src_path,dest_pathname);
													exit(e_status);
											}
										}
									}
								}
					}
					return 1+count;
				}
				
				   

            
            
            
                
