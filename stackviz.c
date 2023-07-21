#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>

typedef struct STACK_FRAME//data structure to hold the contents of the stack frame of the profiled function
{
    void *this_fn;
    void *call_site;
    void *profiledaddress;
    void *beginningaddress;
    void *endingaddress;
    

    struct timespec startTime;
    struct timespec endTime;

    double runningtime;
	

    pid_t startprocess_id;
    pid_t startparentprocess_id;

    pid_t endprocess_id;
    pid_t endparentprocess_id;

    unsigned int * initial;
    unsigned int * final;

    
    struct STACK_FRAME *next;
    
}STACK_FRAME;



/*
*Accepts a binary file from the command line
*Reads from the binary file which contains a list of structures for the profiled functions
*Prints each of the structures
*Has three command line sorting options
*@param argc: int variable for number of arguments
*@param argv: array variable for command line argument
*/
int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        printf("Provide the binary file name\n");
        return 1;
    }  
    
	FILE *fp;

    fp = fopen(argv[2], "rb");//argument 2 in this program is the binary file

    struct STACK_FRAME currentNode;//declare structure to hold the values that it will receive while reading the binary file
	
  
    int counter  = 0;//index to count the number of profiled functions that are printed


    if(strncmp(argv[1], "-s",1) == 0 )//statement to sort by the start time of the calls
      {

               while( fread(&currentNode, sizeof(struct STACK_FRAME), 1, fp))
                 {
                        printf("\n");//Just to make the output easier to read
          
                        counter++;
                        
		                fprintf(stderr, "(%d)\n", counter);//printing the index counter
                        
                        //print statements to print out the data of the current stack frame being read from the file
                        fprintf(stderr,"start process: %d, start parent process: %d\n", currentNode.startprocess_id, currentNode.startparentprocess_id);
		        fprintf(stderr, "finish process: %d, finish parent process: %d\n", currentNode.endprocess_id, currentNode.endparentprocess_id);

		        fprintf(stderr,"function:%p, caller:%p, frame pointer:%p\n", currentNode.this_fn, currentNode.call_site, currentNode.profiledaddress);


                        fprintf(stderr,"stack frame:%p-%p,  running time: %f (%ld.%.9ld - %ld.%.9ld)\n", currentNode.beginningaddress, 
		        currentNode.endingaddress,currentNode.runningtime,currentNode.startTime.tv_sec, currentNode.startTime.tv_nsec,
		        currentNode.endTime.tv_sec, currentNode.endTime.tv_nsec);

    	                fprintf(stderr,"address range                  initial    final\n");
                        
                        //loop to print out the address range and its contents
    	                while(currentNode.beginningaddress > currentNode.endingaddress)
			    {
		                 //calculating the address range for the stack frame
		                 currentNode.beginningaddress -= 1;
    	                         currentNode.beginningaddress -= 3;

		                 printf("%p-%p: \n", currentNode.beginningaddress, currentNode.beginningaddress);
                                 
                            }	
            
            
             
		                printf("\n");
                    }

        }

    else  if(strncmp(argv[1], "-f",1) == 0 )//statement to sort by finish time
      {
                //same as last if statement reads from the file and prints out the data of the stack frame
               while( fread(&currentNode, sizeof(struct STACK_FRAME), 1, fp))
                 {
                        printf("\n");
          
                        counter++;
      
		        fprintf(stderr, "(%d)\n", counter);

                        fprintf(stderr,"start process: %d, start parent process: %d\n", currentNode.startprocess_id, currentNode.startparentprocess_id);
		        fprintf(stderr, "finish process: %d, finish parent process: %d\n", currentNode.endprocess_id, currentNode.endparentprocess_id);

		        fprintf(stderr,"function:%p, caller:%p, frame pointer:%p\n", currentNode.this_fn, currentNode.call_site, currentNode.profiledaddress);


                        fprintf(stderr,"stack frame:%p-%p,  running time: %f (%ld.%.9ld - %ld.%.9ld)\n", currentNode.beginningaddress, 
		                currentNode.endingaddress,currentNode.runningtime,currentNode.startTime.tv_sec, currentNode.startTime.tv_nsec,
		                currentNode.endTime.tv_sec, currentNode.endTime.tv_nsec);
   	 	
    	                fprintf(stderr,"address range                  initial    final\n");

             
    	                while(currentNode.beginningaddress > currentNode.endingaddress)
			   {
		
		                 currentNode.beginningaddress-=1;
    	                         currentNode.beginningaddress -= 3;

		                 printf("%p-%p: \n", currentNode.beginningaddress, currentNode.beginningaddress);
                                 
                            }	
            
            
             
		                printf("\n");
                 }


      }
       
     else  if(strncmp(argv[1], "-p",1) == 0 )//statement for option to sort by finish pid and start time
      {
               //same as last if statement reads from the file and prints out the data of the stack frame
               while( fread(&currentNode, sizeof(struct STACK_FRAME), 1, fp))
                 {
                        printf("\n");
          
                        counter++;
      
		        fprintf(stderr, "(%d)\n", counter);

                        fprintf(stderr,"start process: %d, start parent process: %d\n", currentNode.startprocess_id, currentNode.startparentprocess_id);
		        fprintf(stderr, "finish process: %d, finish parent process: %d\n", currentNode.endprocess_id, currentNode.endparentprocess_id);

		        fprintf(stderr,"function:%p, caller:%p, frame pointer:%p\n", currentNode.this_fn, currentNode.call_site, currentNode.profiledaddress);


                        fprintf(stderr,"stack frame:%p-%p,  running time: %f (%ld.%.9ld - %ld.%.9ld)\n", currentNode.beginningaddress, 
		        currentNode.endingaddress,currentNode.runningtime,currentNode.startTime.tv_sec, currentNode.startTime.tv_nsec,
		        currentNode.endTime.tv_sec, currentNode.endTime.tv_nsec);
   	 	                
    	                fprintf(stderr,"address range                  initial    final\n");
                        

                        
    	                while(currentNode.beginningaddress > currentNode.endingaddress)
			    {
		                         
		                 currentNode.beginningaddress -= 1;
    	                         currentNode.beginningaddress -= 3;

		                 printf("%p-%p: \n", currentNode.beginningaddress, currentNode.beginningaddress);
                    
                            }	
                            
                            
            
            
             
		                printf("\n");
                 }


      }


	fclose(fp);
       
}
