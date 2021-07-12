// refs https://www.youtube.com/watch?v=n2AAhiujAqs&list=WL&index=12&t=267s&ab_channel=drdelhart
#define BLOCK_SIZE 256
#define NAME_LEN 16

#include<time.h>
#include <stdbool.h>

struct superblock {
        int num_inode;
        int num_block;
        int num_files;
        //int size_blocks;
};
struct files {
        int inode_idx;
        char name[NAME_LEN];
};
struct inode {
        int ptr_count; // counts the number of files pointing
        int size;
        time_t creation_time;
        char user_name[NAME_LEN];
        int access_code;
        int first_block;
        //char name[NAME_LEN];
};

struct disk_block {
        int next_block_num;// -1 means empty, -2 means end of file, >=0 value means occupied and points to another block
        char data[BLOCK_SIZE];
};

void create_fs (); // initialize new filesystem

void mount_fs (); // load a file system

void sync_fs (); // write the file system

void print_fs (); // Print the file system info

int next_available_block(); // Returns the position of the disk_block array

int occupy_blocks(int, int); // Occupy the blocks, the file will take

//int find_inode(char *); // Find the inode of the file

void read_fs(); // Read a file from the created file system

void delete_inode(); // delete a file and free the blocks occupied
void free_block(int);

void put(); // Copy a file from os based file system

void rename_file(); // Rename a file in the file system

void transfer_file(); // copy the data from a fs into OS based file system

char* detect_username();

void change_access();

int find_empty_file_inode();

int find_file(char *);

void link();