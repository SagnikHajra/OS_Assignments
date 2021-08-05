// Please follow readme for details
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


char file_header[300];
int arr_len = 0;
// array of Structure to store the file data for sorting
struct earthquake
{
   char time[50];
   double latitude;
   char other_col[300];
} arr[100000], tmp;

// This func loads the data into struct earthquake arr
void load_file_data(){

        FILE* file_data = fopen("all_month.csv", "r");
        if(file_data == NULL){
                printf("Error: file open error");
                exit(1);
        }

        char data[300], comma[2] = ",";
        int col = 0;
        // Excluding the header. During dump it will be used
        fgets(file_header, 300, file_data);
        // Travarse through rest of the file. Store the first two col in time and latitude fields and the rest into other_col
        while(fgets(data, sizeof(data), file_data)){
                char *token;

                token = strtok(data, comma);
                while(token != NULL){
                        if(col==0)
                                strcpy(arr[arr_len].time, token);
                        else if(col==1)
                                arr[arr_len].latitude = strtod(token, NULL);
                        else if(col==3)
                                strcpy(arr[arr_len].other_col, token);
                        else{
                                strcat(arr[arr_len].other_col, comma);
                                strcat(arr[arr_len].other_col, token);
                        }
                        token = strtok(NULL, comma);
                        ++col;
                }
                        arr_len +=1;
        }
        fclose(file_data);
        printf("File Load complete, %d no of rows loaded\n", arr_len);
}

// Sort the earthquake array using the range specified
void sort(int begin_idx, int end_idx, int process){

        int i,j;
        if(!end_idx || end_idx>arr_len)
                end_idx=arr_len+1;

        int no_rows = end_idx-begin_idx, cnt = 0;

        for(i=begin_idx+1;i<end_idx;++i)
        {
                for(j=begin_idx;j<end_idx-i;j++)
                {
                        if(arr[j+1].latitude<arr[j].latitude)
                        {
                                tmp=arr[j];
                                arr[j]=arr[j+1];
                                arr[j+1]=tmp;
                        }
                }
                ++cnt;
                if(cnt%1000==0 || cnt==no_rows-1){printf("%d lines sorted by %d\n",cnt,process);}
        }

}

// Dump the sorted earthquake array into a file
void file_dump(){

        FILE* output = fopen("sorted_data_file.csv", "w");
        fprintf(output,"%s", file_header);
        int i;
        for(i=0;i<arr_len;i++){
                fprintf(output,"%s,%lf,%s",arr[i].time, arr[i].latitude, arr[i].other_col);
        }
        fclose(output);
        printf("Chgeck sorted_data_file.csv for the sorted data\n");
}



int main(){

        load_file_data();
        sleep(1);
        int n = arr_len/3;
        // create 4 processes. 3 children will sort the earthquake array data and parent process will marge the partially sorted array
        int p1 = fork();
        int p2 = fork();
        int id = getpid();
        if(p1==0 && p2>0){
                printf("Child_1 Process %d starts\n",id );
                printf("Child_1 Process_id(id=%d) picks %d records for sorting\n", id, n);
                sort(0,n,id);
                printf("Child_1 Process_id(id=%d) ends sorting rows...\n", id);
                printf("Child_1 Process %d ends\n", id);
        }
        else if(p1>0 && p2==0){
                printf("Child_2 Process %d starts\n",id );
                printf("Child_2 Process_id(id=%d) picks %d records for sorting\n", id, n);
                sort(n+1,n+n,id);
                printf("Child_2 Process_id(id=%d) ends sorting rows...\n", id);
                printf("Child_2 Process %d ends\n", id);
        }
        else if(p1==0 && p2==0){
                printf("Child_3 Process %d starts\n",id );
                printf("Child_3 Process_id(id=%d) picks %d records for sorting\n", id, n);
                sort(n+n+1,arr_len+1,id);
                printf("Child_3 Process_id(id=%d) ends sorting rows...\n", id);
                printf("Child_3 Process %d ends\n", id);
        }
        else if(p1>0 && p2>0){
                printf("Parent Process %d starts\n", id);
                //printf("Parent Process_id(id=%d) picks %d records for sorting\n", id, n);
                //sort(n+1,arr_len+1,id);
                //printf("Parent Process_id(id=%d) finishes sorting remaining rows\n", id);
                wait(NULL);
                printf("Merge the three sorted parts of array\n");
                sort(0,arr_len+1,id);
                printf("Sorting the whole array is finished\n");
                file_dump();
                printf("Parent Process ends\n");
        }
}
