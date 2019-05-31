/*******************************************************************************
*Description: fixed size memory allocation                                     *
*Created BY Bareket Sar Shalom                                                 *
*Date: 30.11.18                                                                *
*******************************************************************************/
#include <stdlib.h>/*******************malloc free*****************************/
#include <assert.h>/***********************assert******************************/
#include "../include/fixed_size_memory_allocation.h" 
/******************************************************************************/

#define BOUNDERY 8

struct fsmalloc_s
{
	char *next;
	size_t block_size;
	size_t number_of_blocks;
	void *all_blocks;
};

fsmalloc_t *FSMAllocCreate(size_t block_size, size_t number_of_blocks)
{
	size_t i = 0;
	size_t* runner;
	
	fsmalloc_t *fsm = (fsmalloc_t *) malloc (sizeof(fsmalloc_t));
	
	if (NULL == fsm) 
	{
		return (NULL);	
	}
	
	fsm->all_blocks = malloc (block_size * (number_of_blocks + 1));
	
	if (NULL == fsm->all_blocks) 
	{
		free(fsm);
		return (NULL);	
	}
	
	runner = fsm->all_blocks;
	
	for(	; i < number_of_blocks; ++i)
	{
		*(runner) = (size_t)((char *)runner + block_size);
		runner = (size_t *)((char *)(runner) + block_size);		
	}
	
	fsm->block_size = block_size;
	fsm->number_of_blocks = number_of_blocks;
	fsm->next = fsm->all_blocks;
	
	return (fsm);
}

void FSMAllocDestroy(fsmalloc_t *fsm)
{
	assert(fsm);

	free(fsm->all_blocks);
	free(fsm);
}

void *FSMAllocGetBlock(fsmalloc_t *fsm)
{
	void* given_block = NULL;
	assert(fsm);

	given_block = fsm->next;
	fsm->next = *(char**)(fsm->next);
	--fsm->number_of_blocks;

	return (given_block);
}

void FSMAllocFreeBlock(fsmalloc_t *fsm, void *block)
{	
	char *data = fsm->next;
	assert(fsm);

	fsm->next = (char*)block;
	*(size_t*)block = (size_t)(data);
	fsm->number_of_blocks = fsm->number_of_blocks + 1;
}

size_t FSMAllocCountFree(fsmalloc_t *fsm)
{
	assert(fsm);
	
	return (fsm->number_of_blocks);
}

size_t FSMAllocSupportedBlockSize(size_t block_size)
{
	if (block_size % BOUNDERY != 0)
	{
		block_size = block_size + (BOUNDERY - block_size % BOUNDERY);
	}
	
	return (block_size);
}
