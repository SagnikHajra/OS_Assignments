#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
/*#include <sys/types.h>
#include <curses.h>
#include <stdlib.h>

#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
*/
int main(){
        time_t t = time(NULL);
        char s[256];
        getcwd(s, 200);
        printf( "\nCurrent Working Directory: %s \n", s);
        printf("It is now: %s\n\n", ctime(&t));

        DIR * d;
        struct dirent * de;
        d = opendir( "." );
        int c = 0;
        // Print the files in cur working dir
        printf("Files:\n");
        int k='n';
        while((de = readdir(d))){
                if(de->d_type && DT_REG){
                        printf("             %d. %s\n", c++, de->d_name);
                }
                if(c%5==0){
                        printf("press n for more else press any key\n:");
                        k = getchar();
                }
                if(k != 'n'){break;}
        }

        // Print the sub-directories in the cwd
        d = opendir(".");
        c=0, k='n';
        printf("Directories:\n");
        while ((de = readdir(d))){
                if ((de->d_type) & DT_DIR){
                        printf("             %d. %s\n", c++, de->d_name);
                }
                if(c%5==0){
                        printf("press n for more else press any key");
                        k = getchar();
                }
                if(k != 'n'){break;}
        }
