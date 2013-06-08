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

// reads the input file and puts job details into a PcbPtr
// enqueue jobs in order
PcbPtr readInput(char * filename) 
{ 
	// create a null pcb
	PcbPtr p = NULL;
	// open input file
	FILE * f = fopen(filename, "r");
	if(f == NULL)
		printf("error");

	int count = 1; // used to id the jobs

	// for each line
	while(!feof(f)) 
	{
		PcbPtr process = createnullPcb(); // create a holder for process
		// take values and set process variables
		fscanf(f,"%d, %d, %d, %d, %d, %d, %d, %d\n", &process->arrivaltime, &process->priority,
							 &process->remainingcputime, &process->memoryalloc,
							 &process->printers,&process->scanners,&process->modems,&process->CDs);
		
		process->args[0] = "./process"; // ./process is the command that the jobs do
		process->args[1] = NULL; // null terminator so when we exec it knows where the end of the command is
		process->suspended = 0;	// not suspended
		process->id = count; // set the id based on the order in the file
		count++; // increment the count

		// enqueue process to p
		if(p != NULL)
			p = enqPcb(p,process);
		else
			p = process;
	}
	return p;
}