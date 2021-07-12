# include <stdio.h>
#include "fs_struct.h"
#include <stdlib.h>
int main (){
        //printf("Enter the number of blocks:");

        //create_fs ();
        //sync_fs ();
        mount_fs ();
        print_fs (1);

        int c;
        while(1){
                printf("Control: p:all files, d:delete a file, r:read a file, x:put externalFile, g:copy disk file into fs, c: change name of file, q:quit\n");
                scanf("%s",&c);
                switch(c){
                        case 'p': print_fs (1);
                                break;
                        case 'd': delete_inode();
                                break;
                        case 'x': put();
                                break;
                        case 'g': print_fs (1);
                                break;
                        case 'r': read_fs();
                                break;
                        case 'c': rename_file();
                                break;
                        case 't': transfer_file();
                                break;
                        case 'q': printf("Done\n");
                                exit(0);
                }
        }
}
