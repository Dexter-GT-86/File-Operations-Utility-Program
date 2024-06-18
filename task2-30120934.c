//Writen by Yujie Hu (30120934)	
//create date 01/10/2021
//last modified time 08/10/2021
#include <stdio.h>//standrad input and putput
#include <stdbool.h>// using boolean
#include <stdlib.h>// allocate memory 

typedef enum state{
   READY,RUNNING,EXIT
} process_state_t;

typedef struct pcbs{
   char *process_name;  
   int entryTime;
   int serviceTime;
   int remainingTime;
   int order;   //used to identify the process in the copied array
   int deadline;
   int waitingTime;
   int turnaroundTime;
   int metdeadline;
   int first;  //record if the process is first time in the system
   
   process_state_t state;
}pcb_t;
int getsize(FILE *fp){//get the num of process
   int num =0;
   char buf[1024];
   if(fp == NULL){                 //check file able to open
       perror("Can not open file");
       return -1;
   }
   while(fgets(buf,1024,fp)){
     num++;
   }  //count lines
   fclose(fp);
   return num;
}
int getnext(struct pcbs pcb_array[],int time,int size,int end){//get the process that needs to run.
  struct pcbs list[size];
  for(int i=0;i<size;i++){// copy the pcb_array
     list[i] = pcb_array[i];
  }
  int min=0;
  bool sure = true;
  int ret=0;
  struct pcbs temp;
  for(int i=0;i<size;i++){   //sort the process in  ascending order  
       min=i;
       for(int j=i;j<size;j++){
         if(list[j].remainingTime<list[min].remainingTime){ //sort the process by the remaining time;
            temp =list[min];
            list[min]=list[j];
            list[j] = temp;
         }
       }
    }
  while(sure){ //check if the return process is not finish and already entried in the system
     if(ret<size-1){
       if(list[ret].remainingTime==0||list[ret].entryTime>time){//if the process is finished or have not entry yet.
         ret+=1;
       }
       else{
         sure=false;
       }
     }
     else{
       sure = false;
     }
  }

  return list[ret].order;//return the num of the next process 
}

void createoutput(struct pcbs pcb_array[],int size){
  FILE *fp;
  fp = fopen("results-task2.txt","w+");//open or create output file
  for(int i=0;i<size;i++){
    fprintf(fp,"%s %d %d %d\n",pcb_array[i].process_name,pcb_array[i].waitingTime,pcb_array[i].turnaroundTime,pcb_array[i].metdeadline); //wirte to the file;
  }
  fclose(fp);
}
void SRTN(struct pcbs pcb_array[],int size){ //shortest remaining time first
   int end = 0;
   int time = 0;
   int count= 0;
   int min;

   for(int i=0;i<size;i++){//generate remainingtime,first and order
      pcb_array[i].remainingTime = pcb_array[i].serviceTime;
      pcb_array[i].first = 0;
      pcb_array[i].order = i;
   }
   while(end<size){//simulate time goes by 
     for(int i=0;i<size;i++){
       if(time==pcb_array[i].entryTime){//get the entry Time of process
          printf("Time %d :%s has entered the system.\n",time,pcb_array[i].process_name);
          pcb_array[i].state= READY;//set the state 
        }
     }
     min = getnext(pcb_array,time,size,end);//get the next process that needs in the running stage
     printf("Time %d :%s is in running stage.\n",time,pcb_array[min].process_name);
     pcb_array[min].state= RUNNING;//set the state 
     pcb_array[min].first+=1;
     if(pcb_array[min].first==1){//chcek if it is first time in running.
       pcb_array[min].waitingTime = time - pcb_array[min].entryTime;
     }
     if(pcb_array[min].remainingTime<=3){//check if the remaining time smaller than quantum of 3
       while(pcb_array[min].remainingTime>0){ 
        time+=1;
        pcb_array[min].remainingTime-=1;
          for(int i=0;i<size;i++){
           if(time==pcb_array[i].entryTime){//get the entry Time of process
            printf("Time %d :%s has entered the system.\n",time,pcb_array[i].process_name);
            pcb_array[i].state= READY;
           }
         }
         if(pcb_array[min].remainingTime==0){//checl whether the process is finished or not
          pcb_array[min].state = EXIT;//set the state 
          printf("Time %d :%s has finished execution.\n",time,pcb_array[min].process_name);
          pcb_array[min].turnaroundTime =time-pcb_array[min].entryTime;//get the tuenaround time
          if(pcb_array[min].deadline>=pcb_array[min].turnaroundTime){//checl deadline meet or not
            pcb_array[min].metdeadline = 1;
          }
          else{
            pcb_array[min].metdeadline = 0;
          }
         }
       }
       end++;
     }
     else{
      while(count<3){//set quantum of three
        pcb_array[min].remainingTime-=1;
        count+=1;
        for(int i=0;i<size;i++){
         if(time==pcb_array[i].entryTime){//get the entry Time of process
          printf("Time %d :%s has entered the system.\n",time,pcb_array[i].process_name);
          pcb_array[i].state= READY;
        }
       }
       time+=1;
      }
      count=0;
      pcb_array[min].state= READY;
      printf("Time %d :%s has changed to the ready stage.\n",time,pcb_array[min].process_name);
     }
   }
   createoutput(pcb_array,size);//create the output file
   

}

int main(int argc, char *argv[])
{  
   FILE *fp =NULL;
   int num=0;
   if(argc == 1){//identify there is a sourcefile or not
      fp = fopen("processes.txt","r");
   }
   else{
      fp = fopen(argv[1],"r");
   }
   
   if(fp == NULL){                 //check file able to open
       perror("Can not open file");
       return -1;
   }
   int size = getsize(fp);//get the number of process
   if(argc == 1){//identify there is a sourcefile or not
      fp = fopen("processes.txt","r");
   }
   else{
      fp = fopen(argv[1],"r");
   }
   
   if(fp == NULL){                 //check file able to open
       perror("Can not open file");
       return -1;
   }
   struct pcbs *pcb_array=(struct pcbs*)malloc(size*sizeof(struct pcbs));
   do{                             //get the data in text file and store it to array.
       if(feof(fp)){
          break;
       }
       pcb_array[num].process_name =(char *)malloc(11*sizeof(char));//arrage space 
       fscanf(fp,"%s %d %d %d",pcb_array[num].process_name,&pcb_array[num].entryTime,&pcb_array[num].serviceTime,&pcb_array[num].deadline);//read the process and store it in the pcb array.
       num++;

   }while(1);
   
   fclose(fp);
   SRTN(pcb_array,size);//arrage processes
    for(int i=0;i<size;i++){//free space
      free(pcb_array[i].process_name);
   }
   free(pcb_array);
  
   return 0;
}
