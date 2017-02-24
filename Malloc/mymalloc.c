#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <sys/mman.h>

//to be used to assign 1 GB of memory
static void *base = NULL;

struct Node{

	//info to hold size when splitting for buddy allocation
	char header;
	
	//linked list approach (horizontal envisionment)
	struct Node *left;

	struct Node *right;
};

struct memoryTable{

	//recommended from crc to use some sort of table that 
	//housed sizes starting from 32 bytes all the way to 
	//1GB
	//basically the free array from the FAQ sheet
	struct Node *table[26];

}memoryTable;

void *my_buddy_malloc(int size){
	
	//base case where nothing has been set yet/first iteration
	if(base==NULL){
		//base is now 1 GB from project sheet
		base = mmap(NULL,1<<30,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON,0,0);
		int i;
		
		//create memoryTable
		for(i = 0; i<26; i++){
			memoryTable.table[i] = NULL;
		}

		struct Node head;
		head.header = 0;
	
		//initialize first node
		head.header = head.header | 30;
		head.left = NULL;
		head.right = NULL;

		//place 1 GB into the last space of our memory space since that is the goal
		memoryTable.table[25] = (struct Node*)base;
		*memoryTable.table[25] = head;
		
	}
	//pow2 holds a value that can sort out the space for say, memory size between 32 and 64. 
	//using temp helps me round up in such a way that there is enough space for 50 mb
	//temp just holds size 
	int pow2 = 0;
	double temp = (double) size;
	
	while(temp >=32){
		temp = temp/2;
		pow2 = pow2 + 1;
	}
	
	//check for node
	if(memoryTable.table[pow2] != NULL){
		//retrieve node pointer
		struct Node *tempNode = memoryTable.table[pow2];
		
		//sets first byte to 1
		(*tempNode).header = (*tempNode).header | 128;

		//remove first node
		memoryTable.table[pow2] = (*tempNode).right;

		//to complete removal, set next nodes reference to the previous node "left" to null
		if((*tempNode).right != NULL){
			(*memoryTable.table[pow2]).left = NULL;
		}
		
		//increment address by 1 byte and return pointer to user
		return((char *)tempNode + 1);
		
	}
	else{
		//after splitting data evenly, buddy allocation will be done
		int index = pow2;

		while(memoryTable.table[index] == NULL){
			index++;
		}
		
		while(index != pow2){
			//split first chunk of memory
			struct Node *chunk = memoryTable.table[index];
			
			//remove chunk from memoryTable
			memoryTable.table[index] = (*chunk).right;

			//replace pointer of head
			if((*chunk).right != NULL){
				(*memoryTable.table[index]).left = NULL;
			}

			//split evenly
			char pow = (*chunk).header;
			int chunkMem = (1<<pow)/2;
		
			//get to half of block
			struct Node *nextChunk = (struct Node*)(((char *)chunk) + chunkMem);

			//need more temp nodes for memory of chunks
			struct Node tempNode1;
			struct Node tempNode2;

			tempNode1.header = (index -1) + 5;
			tempNode2.header = (index -1) + 5;

			tempNode1.right = nextChunk;
			tempNode1.left = NULL;
			tempNode2.right = NULL;
			tempNode2.left = chunk;

			*chunk = tempNode1;
			*nextChunk = tempNode2;

			if(memoryTable.table[index-1] == NULL){
				memoryTable.table[index-1] = chunk;
			}
			else{
				(*nextChunk).right = &(*memoryTable.table[index -1]);
				(*memoryTable.table[index-1]).left = &(*nextChunk);
				memoryTable.table[index-1] =&(*chunk);
			}
			index--;
		

		}
		struct Node *temp = memoryTable.table[pow2];
		memoryTable.table[pow2] = (*temp).right;

		if((*temp).right != NULL){
			(*memoryTable.table[pow2]).left = NULL;
		}
		
		(*temp).header = (*temp).header |128;
		return ((char *)temp +1);
	}
	return NULL;
}

void my_free(void *ptr){
	char *pointer = ptr;
	pointer = pointer -1;
	pointer[0] = pointer[0] & 127;
	
	char chunk = pointer[0];
	
	int index = ((int)chunk) - 5;

	int address = (pointer - (char *)base);

	int size = 1<<chunk;

	int buddyOffset = (address ^ size);

	char *buddy = ((char *)base) + buddyOffset;

	char buddyHeader = buddy[0];
	
	int bit7 = buddyHeader>>7 & 1;

	struct Node *block = (struct Node*)pointer;
	struct Node head;
	head.header = chunk;
	head.left = NULL;
	head.right = NULL;
	*block = head;

	while(bit7 == 0){
		struct Node first;
		first.header = chunk + 1;
		first.left = NULL;
		first.right = NULL;
		
		*block = first;

		struct Node *buddyNode = (struct Node*)buddy;

		if( (*buddyNode).left == NULL ) 
		{
			memoryTable.table[index] = (*buddyNode).right;
		}
		else
		{
			(*(*buddyNode).left).right = (*buddyNode).right;
		}
		
		if(buddy < pointer)
		{
			pointer = buddy;
			block = (struct Node*)pointer;
			*block = first;
		}

		chunk = chunk + 1;
		index = ((int)chunk) - 5;

		if(index == 25)
			break;

		size = 1<<chunk;
		buddyOffset = (address ^ size);
		buddy = ((char *)base) + buddyOffset;
		buddyHeader = buddy[0];
		bit7 = buddyHeader>>7 & 1;	
	}
	
	int indexPlace = ((int)pointer[0]) - 5;
	
	if(memoryTable.table[indexPlace] != NULL){
		(*memoryTable.table[indexPlace]).left = block;
		memoryTable.table[indexPlace] = block;
	}
	else{
		memoryTable.table[indexPlace] = block;
	}
}

/*
int main(){
	return 0;
}
*/