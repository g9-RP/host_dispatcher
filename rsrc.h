/**
	Khanh Cao Quoc Nguyen
	311253865
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// global variables set in hostd.c
extern int available_printers;
extern int available_scanners;
extern int available_modems;
extern int available_CDs;

// check if there are enough resources for the process
int rsrcChk(PcbPtr process){
	if (process->printers <= available_printers 
		&& process->scanners <= available_scanners 
		&& process->modems <= available_modems 
		&& process->CDs <= available_CDs)
		return 1;
	else
		return 0;
}

// allocates resources for a process
void rsrcAlloc(PcbPtr process){
	available_printers -= process->printers;
	available_scanners -= process->scanners;
	available_modems -= process->modems;
	available_CDs -= process->CDs;
}

// free resources that were allocated to process
void rsrcFree(PcbPtr process){
	available_printers += process->printers;
	available_scanners += process->scanners;
	available_modems += process->modems;
	available_CDs += process->CDs;
}