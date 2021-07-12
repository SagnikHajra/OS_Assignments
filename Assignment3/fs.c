#include "fs_struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
//#define BLOCK_SIZE 256
//#define NAME_LEN 16

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;
struct files *list;

char* detect_username(){
        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        if (pw){
                return pw->pw_name;
        }
        return "";
}

int next_available_block(){
        // return  the 1st empty block else -1
        int i=0;
        for (;i<sb.num_block;i++){
                if (dbs[i].next_block_num == -1){
                        dbs[i].next_block_num = -2;
                        return i;
                }
        }
        return -1;
}

int occupy_blocks(int block, int block_count){
        // check all empty blocks(marked as -1) and occupy as many as needed(block_count)
        // If all blocks are accuired, return -1
        int i=0;
        for(;i<sb.num_block && block_count>0;i++){
                if (dbs[i].next_block_num == -1){
                        dbs[i].next_block_num = -2;
                        dbs[block].next_block_num = i;
                        block_count--;
                        block = i;
                }
        }
        if (block_count>0){
                printf("Not enough block available\n");
                return 0;
        }
        return 1;
}

// create file system
void create_fs(){
        //printf ("Enter the number of blocks:");
        //scanf ("%d",&sb.num_block);
        //printf ("\nEnter the number of filenames:");
        //scanf ("%d",&sb.num_inode);
        sb.num_inode = 5;
        sb.num_block = 50;
        sb.num_files = 10;
        //sb.size_blocks = BLOCK_SIZE;
        print_fs (0);
        printf("\n---------File System is created---------\n");
        int i, nb;
        int ob;
        // create the disk_block array with blank entries
        dbs = malloc (sizeof (struct disk_block) * sb.num_block);
        for (i=0; i<sb.num_block; i++){
                dbs[i].next_block_num = -1;
                //dbs.data = "";
        }
        // Create the struct files array
        list = malloc (sizeof (struct files) * sb.num_files);
        for (i=0; i<sb.num_files; i++){
                list[i].inode_idx = -1;
        }
        // create the inode array with user input entries
        inodes = malloc (sizeof(struct inode) * sb.num_inode);
        for (i=0; i<sb.num_inode; i++){
                //printf ("\nEnter the %d filename(upto %d char): ", i, NAME_LEN);
                //scanf("%s", inodes[i].name);
                //getchar();
                //printf ("Enter the number of blocks needed:");
                //scanf("%d",&inodes[i].size);
                inodes[i].ptr_count = 0;
                inodes[i].first_block = -1;
                inodes[i].access_code = 'w';
                //nb = next_available_block();
                //if (nb == -1){printf("No empty block available\n");break;}
                //inodes[i].first_block = nb;
                //ob = occupy_blocks(inodes[i].first_block, inodes[i].size-1);
                //if (!ob){printf("Not enough block available\n");break;}
                //inodes[i].creation_time = time( NULL );
        }
}

void mount_fs (){

        FILE *file;
        file = fopen("fs_data", "rb");

        fread(&sb, sizeof(struct superblock), 1, file);

        inodes = malloc (sizeof(struct inode) * sb.num_inode);
        fread(inodes, sizeof(struct inode), sb.num_inode, file);

        dbs = malloc (sizeof (struct disk_block) * sb.num_block);
        fread(dbs, sizeof(struct disk_block), sb.num_block, file);

        list = malloc (sizeof (struct files) * sb.num_files);
        fread(list, sizeof(struct files), sb.num_files, file);

        fclose(file);
};


void sync_fs () {
        FILE *file;
        file = fopen("fs_data", "wb");
        // Superblock
        fwrite(&sb, sizeof(struct superblock), 1, file);
        // inodes
        fwrite(inodes, sizeof(struct inode), sb.num_inode, file);
        //disk blocks
        fwrite(dbs, sizeof(struct disk_block), sb.num_block, file);
        // inode pointer
        fwrite(list, sizeof(struct files), sb.num_files, file);

        fclose(file);

}

void print_fs (int level){
        // Print Super block info
        printf("Superblock info\n");
        printf("\tnumber of inodes %d\n", sb.num_inode);
        printf("\tnumber of blocks %d\n", sb.num_block);
        printf("\tnumber of files %d\n", sb.num_files);
        printf("\tsize of blocks %d\n", BLOCK_SIZE);
        if (level>0){
                // Print inode info
                printf("\nFiles info\n");
                int i,j;
                //char time[24];
                printf("\tdate\t\t\t\tsize\tuser\tpermission\tfile name\n");
                for (i=0; i<sb.num_files; i++){
                        if(list[i].inode_idx>-1){
                                j=list[i].inode_idx;
                                //strcpy(time, ctime(&inodes[i].creation_time));
                                printf("\t%.24s\t%d\t%s\t%c\t%s \n", ctime(&inodes[j].creation_time), inodes[j].size*256, inodes[j].user_name, inodes[j].access_code, list[i].name);
                        }
                }
                // Print disk block info
                printf("\nDisk block info\n");
                for (i=0; i<sb.num_block; i++){
                        printf("\tdisk block data %s\n\t next block number %d\n",dbs[i].data, dbs[i].next_block_num);
                }
        }
}

int find_empty_inode(){
        // if the inodes[i].empty flag is flase, return i else -1
        int i;
        for (i=0;i<sb.num_inode;i++){
                if(inodes[i].ptr_count==0){return i;}
        }
        return -1;
}
int find_empty_file_inode(){
// if the list[i].inode_idx ==-1, return i else -1
        int i;
        for (i=0;i<sb.num_files;i++){
                if(list[i].inode_idx==-1){return i;}
        }
        return -1;
}
int find_file(char name[]){
        int i;
        for (i=0;i<sb.num_files;i++){
                if((list[i].inode_idx>-1) && !strcmp(list[i].name, name)){return i;}
        }
        return -1;
}
//int find_inode(char name[]){
//      // if not inodes[i].empty and .name==name, return i else -1
//      int i;
//        for (i=0;i<sb.num_inode;i++){
//              if(!(inodes[i].empty) && !strcmp(inodes[i].name, name)){return i;}
//      }
//      return -1;
//}

void read_fs(){
        // user Input name
        // check if the file exists in the file system or not
        // Read the data from all the blocks the file occupies
        char name[NAME_LEN];
        printf("Enter file name to read:");
        scanf("%s", name);
        int i=find_file(name);
        if(i>-1 &&  (inodes[list[i].inode_idx].access_code == 'w'||inodes[list[i].inode_idx].access_code == 'r')){
                int ind_idx = list[i].inode_idx;
                int j = inodes[ind_idx].first_block;
                while(j<sb.num_block){
                        printf("%s",dbs[j].data);
                        if(dbs[j].next_block_num==-2){return;}
                        j=dbs[j].next_block_num;
                }
        }else{
                printf("No such file found or not accessable");
        }
}

void free_blocks(int start){
        // free all blocks of memory n case of a delete request by .next_block_num=-1
        int i;
        for(i=start; i<sb.num_block; i++){
                if (dbs[i].next_block_num==-2){
                        break;
                }
                dbs[i].next_block_num = -1;
        //      strcpy(dbs[i].data, "");
        }
        dbs[i].next_block_num = -1;
        //strcpy(dbs[i].data, "");
}

void delete_inode(){
        char name[NAME_LEN];
        printf("Enter file name for deletion:");
        scanf("%s", name);
        int i=find_file(name);
        if(i>-1 && inodes[list[i].inode_idx].access_code == 'w'){
                int ind_idx = list[i].inode_idx;
                if(inodes[ind_idx].ptr_count==1){
                        //strcpy(inodes[i].name, "");
                        inodes[ind_idx].creation_time = 0;
                        free_blocks(inodes[ind_idx].first_block);
                        inodes[ind_idx].first_block = -1;
                }
                inodes[ind_idx].ptr_count--;
                strcpy(list[i].name,"");
                list[i].inode_idx = -1;
                sync_fs();
        }else{
                printf("No such file was found or not accessable\n");
        }
}

void put(){
        // take the file namePath as input
        // find next available inode and copy the file name, date etc
        //
        char name[NAME_LEN];
        char data[BLOCK_SIZE+2];
        printf("Enter file name to store Host OS file into the disk:");
        int x = getc(stdin);
        gets(name);
        FILE *file;
        //printf("Step1\n");
        int nb,fni,tmp, empty_file_ptr, num_blocks=0;
        file = fopen(name, "rb");
        if(file==NULL){printf("can't read file");return;}
        empty_file_ptr = find_empty_file_inode();
        if (empty_file_ptr==-1){printf("No empty slot for new file"); fclose(file);return;}
        fni = find_empty_inode();
        if (fni==-1){printf("No empty block for new file"); fclose(file);return;}
        strcpy(list[empty_file_ptr].name, name);
        list[empty_file_ptr].inode_idx = fni;
        inodes[fni].ptr_count++;
        strcpy(inodes[fni].user_name, detect_username());
        //nb = next_available_block();
        //if (nb == -1){
        //      printf("No empty block available\n");
        //      return
        //}
        //inodes[fni].first_block = nb;
        inodes[fni].creation_time = time( NULL );
        // }
        //printf("Step2\n");
        while(fgets(data, BLOCK_SIZE, file)){
                //printf("Step3\n");
                nb = next_available_block();
                printf("nb=%d\nline=%s", nb, data);
                if (nb == -1){
                        printf("No empty block available\n");
                        fclose(file);
                        return;
                }
                //printf("Step2\n");
                if(inodes[fni].first_block==-1){
                        //printf("Step1 %d\n",fni);
                        inodes[fni].first_block = nb;
                }else{
                        dbs[tmp].next_block_num = nb;
                }
                //printf("Step2\n");
                strcpy(dbs[nb].data, data);
                //printf("Step3\n");
                tmp=nb;
                num_blocks++;
        }
        inodes[fni].size = num_blocks;
        sync_fs ();
        fclose(file);
}

void rename_file(){
        char old_name[NAME_LEN], new_name[NAME_LEN];
        printf("Enter old file name: ");
        getchar();
        scanf("%s",old_name);
        //getchar();
        int i=find_file(old_name);
        if(i==-1 || inodes[list[i].inode_idx].access_code!='w'){printf("No such file was found or write protedcted\n");return;}
        printf("Enter new file name: ");
        //getchar();
        scanf("%s",new_name);
        strcpy(list[i].name, new_name);
        inodes[list[i].inode_idx].creation_time = time( NULL );
        sync_fs();
}

void transfer_file(){
        char name[NAME_LEN];
        char dirr[100];
        char filepath[200];
        printf("Enter file name: ");
        getchar();
        gets(name);
        int i=find_file(name),j;
        if(i==-1){printf("No such file was found\n");return;}
        printf("Enter the dir name: ");
        gets(dirr);
        strcpy(filepath,dirr);
        strcat(filepath,"/");
        strcat(filepath,name);
        i=inodes[list[i].inode_idx].first_block;
        FILE *file = fopen(filepath, "wb");
        do{
                fwrite(dbs[i].data, sizeof(dbs[i].data), 1, file);
                printf("%s",dbs[i].data);
                i=dbs[i].next_block_num;
        }while(i!=-2 && i<sb.num_block);
        fclose(file);
}

void change_access(){
        char name[NAME_LEN];
        printf("Enter file name you want to modify: ");
        getchar();
        gets(name);
        int i=find_file(name);
        if(i==-1){printf("No such file was found\n");return;}
        printf("Enter r for read only, w for read-write both: ");
        scanf("%s",&inodes[list[i].inode_idx].access_code);
}
void link(){
        char name[NAME_LEN];
        char new_name[NAME_LEN];
        printf("Enter file name you want to link to(TARGET): ");
        getchar();
        gets(name);
        int i=find_file(name), empty_file_ptr;
        if(i==-1){printf("No such file was found\n");return;}
        printf("Enter file name you want to link with(LINK_NAME) %s: ",name);
        gets(new_name);
        empty_file_ptr = find_empty_file_inode();
        if (empty_file_ptr==-1){printf("No empty slot for new file");return;}
        list[empty_file_ptr].inode_idx = list[i].inode_idx;
        strcpy(list[empty_file_ptr].name, new_name);
        inodes[list[i].inode_idx].ptr_count++;
}
