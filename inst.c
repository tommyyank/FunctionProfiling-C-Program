#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>


pthread_mutex_t lock;//mutex to prevent race condition when writing to the file

pid_t startprocess_id;//global variable for the start process id
pid_t startparentprocess_id;//for the start parent process id

pid_t endprocess_id;//global variable for the finish process id
pid_t endparentprocess_id;//for the finish parent process id

//global variables for the start and end time of the profiled functions
struct timespec startTime;

struct timespec endTime;



//struct Node *head = NULL;//global node to point to the head of the linked list
struct STACK_FRAME *head =  NULL;


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




//declaring the -finstrument-functions that will be used to profile the respective functions
void __cyg_profile_func_enter(void *this_fn,void *call_site)__attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *this_fn,void *call_site)__attribute__((no_instrument_function));






/*
* This function is used to gather the neccessary information at the start of the profiled function
* @param this_fn: pointer to the address of the function being profiled
* @param call_site: pointer to where the profiled function is being called from
*/
void __cyg_profile_func_enter(void *this_fn, void *call_site)
{

    clock_gettime(CLOCK_MONOTONIC, &startTime);//gets start time of the function

    startprocess_id = getpid();//get the process id at the start of the function
    startparentprocess_id = getppid();//get start parent process id

}






/*
* Collects the remaining information about the stack frame
* Inserts the information into the data structure "STACK_FRAME"
* Collects all the information needed in the structure for each function of the program being profiled
* Writes each structure to a binary file
* @param this_fn: pointer to the address of the function being profiled
* @param call_site: pointer to where the profiled function is being called
*/
void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
   //collects remaining data needed for stack frame

   clock_gettime(CLOCK_MONOTONIC, &endTime);//gets the end time of the function
   double runningtime = (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;//calculates the running time of the function
    

   endprocess_id = getpid();//get end process id
   endparentprocess_id = getppid();//get end parent process id
   
  

   STACK_FRAME *currentNode = (struct STACK_FRAME *) malloc(sizeof(struct STACK_FRAME));//declaring and allocating for the structure "STACK_FRAME"

   void *framepointer = __builtin_frame_address(0);
      

   void *profiledaddress = __builtin_frame_address(1);

      
   void *beginningaddress = profiledaddress + 2 * sizeof(void *);//calculates the beginning address of the stack frame
   void *endingaddress = framepointer + 4 * sizeof(void *);//calculates the ending address of the stack frame


   //adding the addresses of the stack frame to the structure
   currentNode->this_fn = this_fn;
   currentNode->call_site = call_site;
   currentNode->profiledaddress = profiledaddress;
   currentNode->beginningaddress = beginningaddress;
   currentNode->endingaddress = endingaddress;

    
   //adding the process id's to the structure   
   currentNode->startprocess_id = startprocess_id;
   currentNode->startparentprocess_id= startparentprocess_id;
   currentNode->endprocess_id = endprocess_id;
   currentNode->endparentprocess_id = endparentprocess_id;

   //adding the time's to the structure
   currentNode->startTime = startTime;
   currentNode->endTime = endTime;
   currentNode->runningtime = runningtime;
   

   int stackSize = (beginningaddress-endingaddress)/4;//calculate size needed for initial and final contents
   currentNode->initial = malloc(stackSize);//allocate memory for contents
   currentNode->final = malloc(stackSize);

   memcpy(currentNode->initial, currentNode->beginningaddress, stackSize);//copy the memory for the contents
   memcpy(currentNode->final, currentNode->endingaddress, stackSize);


   FILE *pFile;

   pFile = fopen("stack.bin", "ab");//open the binary file

  // pthread_mutex_lock(&lock);//mutex around when writing to the file to prevent a race condition
   fwrite(currentNode, sizeof(struct STACK_FRAME), 1, pFile);//writes each stack frame to the binary file
  // pthread_mutex_unlock(&lock);
 
  
   fclose(pFile);
  
}
