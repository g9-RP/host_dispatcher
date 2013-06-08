/**
	Khanh Cao Quoc Nguyen
	311253865
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "pcb.h"
#include "mab.h"
#include "rsrc.h"
#include "utility.h"

// Setting the avaiable resources
int available_printers = 2;
int available_scanners = 1;
int available_modems = 1;
int available_CDs = 2;

void HOSTDispatcher(PcbPtr queue) {
	// Initialize dispatcher queues, real time queue and memory
	MabPtr memory = createnullMab();
	PcbPtr inputQueue = queue;
	PcbPtr running = NULL;
	int timer = 0; // Set dispatcher timer to 0
	PcbPtr priorityQueue1 = NULL;
	PcbPtr priorityQueue2 = NULL;
	PcbPtr priorityQueue3 = NULL;
	PcbPtr userQueue = NULL;
	PcbPtr realTimeQueue  = NULL;
	int ready; // used to determine if we're ready to start/restart processes, i.e. queue management has been completed
	int r = 0;

	// While there's anything in any of the queues or there is a currently running process:
	while(inputQueue != NULL || realTimeQueue != NULL || userQueue != NULL || priorityQueue1 != NULL || priorityQueue2 != NULL || priorityQueue3 != NULL || running != NULL) {
		//   Unload pending processes from the input queue:
		while(inputQueue != NULL && inputQueue->arrivaltime <= timer) {
			// Check priority of process - not real time so put into user queue
			if (inputQueue->priority != 0){
				if(userQueue != NULL)
					userQueue = enqPcb(userQueue,deqPcb(&inputQueue));
				else 
					userQueue = deqPcb(&inputQueue);
				
			}
			// Process is real time so put into real time queue
			else{
				if(realTimeQueue != NULL) 
					realTimeQueue = enqPcb(realTimeQueue,deqPcb(&inputQueue));
				else 
					realTimeQueue = deqPcb(&inputQueue);
			}
		}

		// user queue processes wait for memory and resources to be allocated then put into priority queue
		while(userQueue != NULL && memChk(memory, userQueue->memoryalloc) != NULL && rsrcChk(userQueue) == 1) {
			// if priority is 1 then put into priorityQueue 1
			if (userQueue->priority == 1){
				if(priorityQueue1 != NULL) {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue1 = enqPcb(priorityQueue1,process);
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process); 
				}
				else {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue1 = process;
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process); 
				}
			}
			// if priority is 2 then put into priorityQueue 2
			else if (userQueue->priority == 2){
				if(priorityQueue2 != NULL) {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue2 = enqPcb(priorityQueue2,process);
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process);  
				}
				else {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue2 = process;
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process);  
				}
			}
			// if priority is 3 then put into priorityQueue 3
			else if (userQueue->priority == 3){
				if(priorityQueue3 != NULL) {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue3 = enqPcb(priorityQueue3,process);
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process);  
				}
				else {
					PcbPtr process = deqPcb(&userQueue);
					priorityQueue3 = process;
					memAlloc(memory,process->memoryalloc,process->id);
					rsrcAlloc(process);  
				}
			}
		}

		// If a process is currently running:
		if(running != NULL && r == 0) {
			//  Decrement process remainingcputime;
			running->remainingcputime--;

			//  If times up:
			if(running->remainingcputime == 0) { //stop process if it has finished running
				//  Send SIGINT to the process to terminate it;
				terminatePcb(running);
				//  Free up process structure memory;
				memFree(memory, running->id);
				rsrcFree(running);
				running = NULL;
				ready = 1;
			}
			else  {
				// Send SIGTSTP to suspend it;
				suspendPcb(running);

				// Reduce the priority of the process (if possible)
				if (running->priority < 3)
					running->priority++;

				//enqueue it on the appropriate feedback queue
				if(running->priority == 2) { 
					if(priorityQueue2 != NULL)
						priorityQueue2 = enqPcb(priorityQueue2, running);
					else
						priorityQueue2 = running;
				}
				if(running->priority == 3) { 
					if(priorityQueue3 != NULL)
						priorityQueue3 = enqPcb(priorityQueue3, running);
					else
						priorityQueue3 = running;
				}
				running = NULL;
				ready = 1;
			}
		}
		else if (r == 1){
			running->remainingcputime--;
			if(running->remainingcputime == 0) { //stop process if it has finished running
				terminatePcb(running);
				running = NULL;
				ready = 1;
			}
		}
		else {
			ready = 1;
		}
		
		if(ready == 1) {
			// If there is a real time process waiting then run it
			if (realTimeQueue != NULL){
				running = deqPcb(&realTimeQueue);
				startPcb(running);
				r = 1;
			}
			// If no process currently running && feedback queues are not all empty:
			else if(priorityQueue1 != NULL || priorityQueue2 != NULL || priorityQueue3 != NULL){
				// Dequeue a process from the highest priority feedback queue that is not empty and Set it as currently running process;
				if(priorityQueue1 != NULL)
					running = deqPcb(&priorityQueue1);
				else if(priorityQueue2 != NULL)
					running = deqPcb(&priorityQueue2);
				else if(priorityQueue3 != NULL)
					running = deqPcb(&priorityQueue3);
				// If already started but suspended, restart it (send SIGCONT to it) else start it (fork & exec)
				if(running->suspended == 1){
					r = 0;
					restartPcb(running);
				}
				else {
					r = 0;
					startPcb(running);
				}
			}
			ready = 0;
		}
		// sleep for one second;
		sleep(1);
		// Increment dispatcher timer;
		timer++;
	}
}

int main(int argc, char ** argv)
{
	PcbPtr newPcb = (PcbPtr) malloc(sizeof(Pcb));
	newPcb = readInput(argv[1]);

	HOSTDispatcher(newPcb);

	free(newPcb);

	return 0;

}