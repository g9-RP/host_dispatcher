/**
	Khanh Cao Quoc Nguyen
	311253865
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXARGS 64

//process structure
struct pcb 
{
    pid_t pid;         
    int id;
    char * args[MAXARGS];  
    int arrivaltime;
    int remainingcputime;
    int priority;
    int memoryalloc;
    int printers, scanners, modems, CDs;
    int suspended;
    struct pcb * next;     
};
typedef struct pcb Pcb;
typedef Pcb * PcbPtr; 


// create a new pcb
PcbPtr createnullPcb(void) 
{
    Pcb tmp;
    PcbPtr newPcb = (PcbPtr) malloc(sizeof(tmp));
    return newPcb;
}

// queues the process at the end of the headofQ queue
PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process) 
{
    PcbPtr tmp = headofQ;
	if(tmp != NULL) 
	{
		while(tmp->next != NULL) 
			tmp = tmp->next;
		
		tmp->next = process;
		process->next = NULL;
		return headofQ;
	}
	else return process;
}

// returns the head of the queue and sets a new head
PcbPtr deqPcb (PcbPtr * headofQ) 
{
    PcbPtr tmp;
    if(headofQ && (tmp = *headofQ)) 
    {
		*headofQ = tmp->next;
		tmp->next = NULL;
		return tmp;
    }
    return NULL;
}

// starts process fork and exec
PcbPtr startPcb(PcbPtr process) 
{
    pid_t pid;
    switch (pid = fork ()) 
    {
		case -1: // parent
			return NULL;
		case 0: // child
			execvp(process->args[0], process->args);
			return NULL;
		default:
			process->pid=pid;
			return process;
	}
}

// terminates a process
PcbPtr terminatePcb(PcbPtr process) 
{
    int status;
    if(kill(process->pid,SIGINT)) 
    {
		fprintf(stderr,"Termination of %d failed",(int)process->pid);
		return NULL;
    }
    waitpid(process->pid, &status, WUNTRACED);
    return process;
}

// suspends the process
PcbPtr suspendPcb (PcbPtr process)
{
	int status;
	if(kill(process->pid, SIGTSTP)) 
	{
		fprintf(stderr, "Suspension of %d failed.", (int)process->pid);
		return NULL;
	}
	waitpid(process->pid, &status, WUNTRACED);
	process->suspended = 1; 
	return process;
}

// restarts a suspended process
PcbPtr restartPcb (PcbPtr process)
{
	if(kill(process->pid,SIGCONT)) {
		fprintf(stderr,"Restarting of %d failed",(int)process->pid);
		return NULL;
    }
	process->suspended=0;
	return process;
}