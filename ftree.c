#include <stdio.h>
// Add your system includes here.
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "ftree.h"
#include "hash.h"


/*
 * Returns the FTree rooted at the path fname.
 */
struct TreeNode *generate_ftree(const char *fname) {
    // Your implementation here.
    struct stat info;
    struct dirent *dp;
    
    if(lstat(fname, &info) == -1){
    	perror("lstat");
    	exit(1);
    }
    
    //since fname is a path name we need to extract the file name separately
    //Initializing the node pointers in case directory within a directory is encountered
  	 struct TreeNode *current_node = NULL;
    struct TreeNode *prev_node = NULL;

    //Filling up the information for the root
    struct TreeNode *root = malloc(sizeof(struct TreeNode));
    (*root).permissions = (info.st_mode &0777);
    (*root).contents = NULL;
    (*root).next = NULL;
    
    //extracting the filename from the path
    const char *actual_filename = strrchr(fname,'/');
    if(actual_filename == NULL){
    	actual_filename = fname;
    }
    else {
    	actual_filename = actual_filename +1;
    	}
    (*root).fname = malloc(sizeof(actual_filename));
    strcpy((*root).fname,actual_filename);
    
    //check if the root is the directory
    if(S_ISDIR(info.st_mode))
    {
    	DIR *dir_ptr = opendir(fname);//open the directory
    	if(dir_ptr == NULL){
			perror("opendir");
			exit(1);    	
    	}
    	else{
    	while((dp =readdir(dir_ptr)) != NULL){
    		if((*dp).d_name[0] != '.'){
    			//recursively call generate f_tree for every entry in case we hit a directory. 
    	
    			char *entire_path = malloc(sizeof(fname) + sizeof((*dp).d_name) +1);
    			strcpy(entire_path,fname);
    			strcat(entire_path,"/");
    			strcat(entire_path,(*dp).d_name);
  				struct TreeNode *new = generate_ftree(entire_path);
  				free(entire_path);
  				
  				//check if the root is NULL
  				//check if the previous node pointer is null
  				// general cases
  				if((*root).contents == NULL){	
  					current_node = new;  
  					(*root).contents = new;			
  				}
  				else{
  					current_node  = new;
  					(*prev_node).next = current_node;
	
  				}
  				prev_node = current_node;
  				
  			}
    	}
    	closedir(dir_ptr);
    	
	}
  }
    else if(S_ISREG(info.st_mode) || S_ISLNK(info.st_mode)){
    	//dealing with the condition if the file is regular or a link
      //find the hash value USING THE HASH FUNCTION FROM A1
    		FILE *file = fopen(fname, "rb");
    		if(file == NULL){
				perror("fopen"); 
				exit(1);
				   		
    		}
    		char *return_value = hash(file);
    		(*root).hash = malloc(BLOCK_SIZE);
    		strcpy(root ->hash, return_value);
    		fclose(file);
    		
    	}
    	return root;
    }



/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 */
void print_ftree(struct TreeNode *root) {
    // Here's a trick for remembering what depth (in the tree) you're at
    // and printing 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    printf("%*s", depth * 2, "");
    //check if given root is a directory
    //if it is a given directory then recursively call print_ftree ad increment the depth to signify change in level
    //Decrement the value of depth afterward
    //for a general case for if root is a file,
    
    // Your implementation here.
    //first check the general case-just a regular or linked file
    //base case
    
	 if((*root).hash){
    	printf("%s (%o)\n",root->fname,root -> permissions);
    }
	 //recursive call to check an item that might be directory
    else{
    	printf("===== %s (%o) =====\n", root -> fname, root -> permissions);
		if(root -> contents != NULL){
			depth ++;
    		print_ftree(root->contents);
    		depth--;
		}     	
    }
    if(root -> next){
		print_ftree(root -> next);    
    }
}
