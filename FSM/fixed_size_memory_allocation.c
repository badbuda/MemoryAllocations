/*******************************************************************************
* Name: Fixed Size Memory Allocation C file                                    *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* Last Update: 2.12.18                                                         *
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* size_t, malloc, free, __WORDSIZE */

#include "../include/fixed_size_memory_allocation.h"

#define DUMMY 1
#define BITS_IN_BYTES 8
#define WORD_BYTES (__WORDSIZE / BITS_IN_BYTES)
/******************************************************************************/

struct fsmalloc_s
{
	size_t count_free;
	char *next_get;
	char *allocated_blocks;
};

fsmalloc_t *FSMAllocCreate(size_t block_size, size_t number_of_blocks)
{
	size_t i = 0;
	size_t *current_position = NULL;

	fsmalloc_t *fsm = malloc(sizeof(fsmalloc_t) + block_size * number_of_blocks
							- sizeof(fsm->allocated_blocks) + DUMMY);
	if (NULL == fsm)
	{
		return (NULL);
	}

	fsm->count_free = number_of_blocks;
	fsm->next_get = (char*)(&fsm->allocated_blocks);

	for (current_position = (size_t*)(&fsm->allocated_blocks);
			i < number_of_blocks;
			++i, current_position = (size_t*)((char*)current_position + block_size)
		  )
	{
		*current_position = (size_t)((char*)current_position + block_size);
	}

	return (fsm);
}
/******************************************************************************/

void FSMAllocDestroy(fsmalloc_t *fsm)
{
	assert(fsm);
	free(fsm);
}
/******************************************************************************/

void *FSMAllocGetBlock(fsmalloc_t *fsm)
{
	void* given_block = NULL;
	assert(fsm);

	given_block = fsm->next_get;
	fsm->next_get = *(char**)(fsm->next_get);
	--fsm->count_free;

	return (given_block);
}
/******************************************************************************/

void FSMAllocFreeBlock(fsmalloc_t *fsm, void *block)
{
	char* old_next = NULL;
	assert(fsm);
	assert(block);

	old_next = fsm->next_get;
	fsm->next_get = (char*)(block);
	*(size_t*)block = (size_t)old_next;
	++fsm->count_free;
}
/******************************************************************************/

size_t FSMAllocCountFree(fsmalloc_t *fsm)
{
	assert(fsm);
	return (fsm->count_free);
}
/******************************************************************************/

size_t FSMAllocSupportedBlockSize(size_t block_size)
{
	size_t coefficient = 0;
	
	if (0 == block_size % WORD_BYTES)
	{
		return (block_size);
	}
	
	coefficient = block_size / WORD_BYTES ;
	return (WORD_BYTES + (WORD_BYTES * coefficient));
}
/*******************************************************************************
*******************************************************************************/
