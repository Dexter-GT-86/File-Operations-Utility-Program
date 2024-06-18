//Hengyu Zhang 31278299
//start date: 8.20 2021
//finish date:  8.27 2021
/*description of my program:
	THis program is for operating the file in low io condition.
	task 1 is to print the first 10 lines of sample.txt on terminal
	task 2 is to chose the file we want to print first 10 lines from the file we choose
	task 3 is to make the copy of file and it holds the first 10 lines of the original file
	task 4 is to choose the length of the lines we want to print or copy
	task 5 is to print or copy the last 10 lines of the file
	Above all, I complete all these five tasks each. But when I see the 5.6 I find it need to 
	operate the combination of these arguments("-L" "-n" "-d"). I have to admit that my pogram
	can only operate comment with the argc less than 4.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h> 		/* change to <sys/fcntl.h> for System V */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void printLine(char* sourceFile, int writePlace, int length);
void printReverseLine(char* sourceFile, int writePlace, int length);



int main(int argc, char *argv[])
{
    char* sourceFile = "sample.txt";//make the source file at first be "sample.txt"
    
    char* warning1 = "fail to open file\n"; //print to on the terminal when fail to open the file
	char* warning2 = "fail to read file\n"; //print to on the terminal when fail to read the file
    
    if (argc == 1) {
   	    //printLine(sourceFile, 1, 10);
   	    printLine(sourceFile, 1, 10);
        return(0);      //// we should return 0 as the task require
		}
		
	

	if (argc == 2) { //the numberof command line argument is less than two, so  need to judge argv[1] is legal or not
		if(strcmp(argv[1] , "-L") == 0){ // this is the case when the command is &./fileutil -L
			//printReverseLine(sourceFile, 1, 10);
   	    	printReverseLine(sourceFile, 1, 10);
        	return(0);      //// we should return 0 as the task require
		
		}else{
			int inputFile;
			if (( inputFile = open(argv[1], O_WRONLY)) < 0) {                   
		  		write(1,warning1,strlen(warning1));
		 	 	exit(1); // we should exit 1 as the task require
		  	}
		   sourceFile = argv[1]; // make the source file be replaced by argv[1]
		   printLine(sourceFile, 1, 10);
		   return(0);      //// we should return 0 as the task require
       }
       }
    
	
	
	
	if (argc >= 3) { //the numberof command line argument is more than three
		int inputFile;
		int outputFile;
		
	  	if (( inputFile = open(argv[1], O_WRONLY)) < 0) {                    
	  		write(1,warning1,strlen(warning1));
	 	 	exit(1); // we should exit 1 as the task require
		}	
		sourceFile = argv[1]; // make the source file be replaced by argv[1]
		
		
		for(int i =2; i<argc; i++){
			  
			  
			  if(strcmp(argv[i] , "-d") == 0 ){   //task 3
			  	char* warning3 = "fail to make file\n"; //print to on the terminal when fail to make the file
				char str1[20];  //the char array to hold the strcpy
				strcpy(str1, argv[i+1]); //store the directory path 
				char str2[20]; //the char array to hold the strcpy
				strcpy(str2, sourceFile); // store the name of sorcefile that you want to copy with
				
			  	strcat(str1,str2);   //combine str1 and str2 and store them in str1
			  	
			  	char* warning5 = "destination already has the file \n"; //print to on the terminal when destination already has the file
				if (( outputFile = open(str1, O_WRONLY)) >= 0) {                   
					  write(1,warning5,strlen(warning5));
					  exit(2); // we should exit 1 as the task require
				} 
				
			  	if(( outputFile = open(str1, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0){
			  		write(1,warning3,strlen(warning3));
			  		exit(2);  // we should exit 2 as the task require
			  	}
			  	printLine(sourceFile, outputFile,10);
			  	write(1, "Copy successful\n", 17);
			  	return(0);      //// we should return 0 as the task require
			  
			  }else if(strcmp(argv[i] , "-n") == 0){//task 4
			    //judge argv[i+1] is a num? using atio
			    int lineLength;
			    int val;
			    char str[20];
			    strcpy(str, argv[i+1]);
			    val = atoi(str);
			    char*  warning4 = "invalid argument, no line numbers specified after the -n argument\n"; //print to on the terminal when fail to make the file
			    if(val == 0){
			    	write(1,warning4,strlen(warning4));
			    	exit(2);  // we should exit 2 as the task require
			    }
			    
			    lineLength = val;
			    
			  	printLine(sourceFile, outputFile,lineLength);
			    //write(1, "Copy successful\n", 17);
			    return(0);      //// we should return 0 as the task require  
			   
			  }else if(strcmp(argv[i] , "-L") == 0){
			  //when I write to this, I find it's not a good idea to judge the argv[i] at different length, beacuse there will be so many cases when the argc is 6 or 7, so may be it woould be better just to judge the  "-L" "-n" "-d" and to choose which operation we need to use.
			  }
			  
			  
			}
			
		
			
    }
    
	
	
	
	
	
	
	
 }

/*
this function has three input which are source file that the function read from, write place to judge it should print on the terminal or make the copy of the file,
int length to decide the line we want to read from
*/
void printLine(char* sourceFile, int writePlace, int length){
		char* warning1 = "fail to open file\n"; //print to on the terminal when fail to open the file
		char* warning2 = "fail to read file\n"; //print to on the terminal when fail to read the file
		
	 	int countCharacter;  // count the number of character
        int countLine =0;    //count the number of line
       
     
        int fd = open(sourceFile,O_RDONLY);  //fd is the file descriptor
        if (fd == -1) {    //-1 means fail to open
          write(1,warning1,strlen(warning1));  //write the warning on terminal
          exit(1);       // we should exit 1 as the task require
          }
         
       
        char buffer1[1024];     // I make this buffer1 with the size 1024 to store all characters in the file
        int m = read(fd, &buffer1, 1024);    // read it, and m is the number of characters in the file
        if (m == -1) {      //-1 means fail to read
            write(1,warning2,strlen(warning2));     //write the warning on terminal
            exit(1);  // we should exit 1 as the task require
        }
         
         
        for(countCharacter=0; countCharacter< m; countCharacter++){   // this for loop keep runing until the whole characters are been judged
            char buffer[1];    //this buffer  just need to store one character each  time
            lseek(fd, countCharacter, SEEK_SET);   // move the file descriptor to the next character in each circulation
           
            int n = read(fd, &buffer, sizeof(buffer));     // read only one character
            if (n == -1) {     //-1 means fail to read
                  write(1,warning2,strlen(warning2));      //write the warning on terminal
                  exit(1);   // we should exit 1 as the task require
              }
     
            if( buffer[countCharacter] == '\n') { // whether there is a '\n' (next line)
               
                countLine += 1;    // make the count of line increce 1
               
                if (countLine == length){  // leave when we have 10 lines
                break;
                }   
            }
            write(writePlace, &buffer, sizeof(buffer));   // write this character
           
        }
        close(fd);     // close the file before leave
        


}

//this function can print the last 10 line of a file
/*
this function has three input which are source file that the function read from, write place to judge it should print on the terminal or make the copy of the file,
int length to decide the line we want to read from
*/
void printReverseLine(char* sourceFile, int writePlace, int length){//task 5
		char* warning1 = "fail to open file\n"; //print to on the terminal when fail to open the file
		char* warning2 = "fail to read file\n"; //print to on the terminal when fail to read the file
		
	 	int countCharacter;  // count the number of character
        int countLine =0;    //count the number of line
        
        int countCharInEachLine[10];// a int array to hold the number in a line in last 10 lines
        int countCharInOneLine = 0; // to count the number of character in one line
       
     
        int fd = open(sourceFile,O_RDONLY);  //fd is the file descriptor
        if (fd == -1) {    //-1 means fail to open
          write(1,warning1,strlen(warning1));  //write the warning on terminal
          exit(1);       // we should exit 1 as the task require
          }
         
       
        char buffer1[1024];     // I make this buffer1 with the size 1024 to store all characters in the file
        int m = read(fd, &buffer1, 1024);    // read it, and m is the number of characters in the file
        if (m == -1) {      //-1 means fail to read
            write(1,warning2,strlen(warning2));     //write the warning on terminal
            exit(1);  // we should exit 1 as the task require
        }
         
        int addNum = m; //let addnum = the length of the file
        for(countCharacter = m-1; countCharacter>= 0; countCharacter--){   // this for loop keep runing until the whole characters are been judged
            char buffer[1];    //this buffer  just need to store one character each  time
            
            addNum --; // to get the last place of the file descrptor
            countCharInOneLine ++;
            lseek(fd, countCharacter, SEEK_SET);   // move the file descriptor to the next character in each circulation
           
            int n = read(fd, &buffer, sizeof(buffer));     // read only one character
            if (n == -1) {     //-1 means fail to read
                  write(1,warning2,strlen(warning2));      //write the warning on terminal
                  exit(1);   // we should exit 1 as the task require
              }
     
            if( buffer[countCharacter] == '\n') { // whether there is a '\n' (next line)
               
                countCharInEachLine[countLine] = countCharInOneLine;
                countCharInOneLine = 0;
                countLine += 1;    // make the count of line increce 1
                
                if (countLine == length){  // leave when we have 10 lines
                break;
                }   
            }
            
            
            //write(writePlace, &buffer, sizeof(buffer));   // write this character
           
        }
        
        
        for(int i =length-1; i >= 0; i--){// it should another loop inside because  the first loop if for operate each line
        	char buffer[1];
        	
        	
        	for(int j = 0; j < countCharInEachLine[i]; j ++){// this for loop while print characters in a line until the loop over
        	lseek(fd, addNum + j, SEEK_SET);
        	int n = read(fd, &buffer, sizeof(buffer));     // read only one character
        	write(writePlace, &buffer, sizeof(buffer));   // write this character
        	}
        	addNum += countCharInEachLine[i];
        }
        
        close(fd);     // close the file before leave
        


}





