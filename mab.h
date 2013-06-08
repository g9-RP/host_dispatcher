/**
	Khanh Cao Quoc Nguyen
	311253865
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MEMORY 960

// structure of a memory block
struct mab {
	int offset;
	int size;
	int allocated;
	int process;
	struct mab * next;
	struct mab * prev;
};
typedef struct mab Mab;
typedef Mab * MabPtr; 

MabPtr memSplit(MabPtr m, int size);
MabPtr memMerge(MabPtr m);

// creates a new block of memory
MabPtr createnullMab(void) {
	Mab tmp;
	MabPtr newMab = (MabPtr) malloc(sizeof(tmp));
	newMab->size = MAX_MEMORY;
	newMab->offset = 0;
	newMab->allocated = 0;
	return newMab;
}

// check if memory available - first fit
MabPtr memChk(MabPtr m, int size) {
	MabPtr current = m;
	while(current != NULL) {
	  if(size <= current->size)
	    return current;
	  if(current->next != NULL)
	    current = current->next;
	  else
	    return NULL;
	}
	return NULL;
}

// first fit algorithm to find the first block of memory that fits and then allocates it
MabPtr memAlloc(MabPtr m, int size, int pro) {
	MabPtr current = m;
	while(current != NULL) {
		if(size <= current->size && current->allocated == 0) {
			if(size == current->size) {
				current->allocated = 1;
				current->process = pro;
				return current;
			}
			else {
				memSplit(current, size);
				current->allocated = 1;
				current->process = pro;
				return current;
			}
		}
		if(current->next != NULL)
			current = current->next;
		else
			return NULL;
	}	
	return m;
}

// free memory block, searches the structure for block assigned to the given process (merges adjacent free blocks if needed)
MabPtr memFree(MabPtr m, int pro) {
	MabPtr current = m;
	while(current != NULL) {
		if(current->process == pro) {
			current->allocated = 0;
			current->process = 0;
			memMerge(current);
			return current;
		}
		if(current->next != NULL)
			current = current->next;
		else
			return NULL;
	}	
	return m;
}
 
// merge two memory blocks
MabPtr memMerge(MabPtr m) {
	MabPtr current = m;
	//block to left
	while(current->prev != NULL && current->prev->allocated == 0) { 
		MabPtr prev = current->prev;
		prev->size = prev->size + current->size;
		if(current->next != NULL) {
			prev->next = current->next;
			current->next->prev = prev;
			current = prev;
		}
		else {
			prev->next = NULL;
			current = prev;
			break;
		}
	}
	//block to right
	while(current->next != NULL && current->next->allocated == 0) { 
		MabPtr next = current->next;
		current->size = current->size + next->size;
		if(next->next != NULL) {
			current->next = next->next;
			next->next->prev = current;
		}
		else {
			current->next = NULL;
			break;
		}
	}
	return m;
}

// split a memory block
MabPtr memSplit(MabPtr m, int size) {
	MabPtr second = createnullMab();
	second->offset = m->offset+size;
	second->size = m->size-size;
	second->next = m->next;
	second->prev = m;
	m->size = size;
	m->next = second;
	if(second->next != NULL)
		second->next->prev = second;
	return m;
}