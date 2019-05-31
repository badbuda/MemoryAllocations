/*******************************************************************************
*Description: variable size memory allocation                                  *
*Created BY Bareket Sar Shalom                                                 *
*Date: 4.11.18                                                                 *
*******************************************************************************/
#include <stdlib.h>/*******************malloc free*****************************/
#include <stdio.h>/*******************printf **********************************/
#include <assert.h>/***********************assert******************************/
#include "../include/variable_size_memory_allocation.h" 
#include "../include/enum_rd58.h" 
/******************************************************************************/
#define MAGIC_NUM 11223344
#define BOUNDERY sizeof(size_t)
#define FIRST_HEADER 1
#define DEFREGMANTED_HEADER 1
#define NEXT_MEMORY_ALLOC 1

struct vsm_head_s 
{
	#ifndef NDEBUG
		size_t magic_num;
	#endif
	size_t block_size;
	bool is_occupied;
};

struct vsmalloc_s
{
	void *allocated;	
	size_t number_of_headers;
};

void CreateHead(vsm_head_t *head, size_t size)
{
	assert(head);
	head->is_occupied = FALSE;
	head->block_size = size;
	#ifndef NDEBUG
		head->magic_num = MAGIC_NUM;
	#endif	
}
vsmalloc_t *VSMAllocCreate(size_t requested_byte_count)
{
	vsmalloc_t *vsm = malloc(sizeof(vsmalloc_t));
	assert (requested_byte_count % BOUNDERY == 0);
	
	if (NULL == vsm) 
	{
		return (NULL);	
	}
	
	vsm->allocated = malloc(requested_byte_count);
	
	if (NULL == vsm->allocated) 
	{
		free(vsm);
		return (NULL);	
	}
	
	CreateHead(vsm->allocated, requested_byte_count - VSMAllocGetHeaderSize());

	vsm->number_of_headers = 1;

	return (vsm);
}

void VSMAllocDestroy(vsmalloc_t *vsm)
{
	assert(vsm);

	free(vsm->allocated);
	free(vsm);
}

void *Defragmantation(vsmalloc_t *vsm, size_t block_size)
{
	size_t i = 0;
	vsm_head_t *header = vsm->allocated;
	vsm_head_t *next_header = header;
	size_t number_of_headers = vsm->number_of_headers - FIRST_HEADER;
	void *dest_data;
	assert(vsm);
			
	for (	; i < number_of_headers; ++i)
	{
		next_header = (vsm_head_t*)((char*)header + header->block_size 
                                                     + VSMAllocGetHeaderSize());

		if (!header->is_occupied && !next_header->is_occupied)
		{
			vsm->number_of_headers = vsm->number_of_headers - DEFREGMANTED_HEADER;
			header->block_size = header->block_size + next_header->block_size
                                                      + VSMAllocGetHeaderSize();
													 
			if (header->block_size >= block_size && block_size != 0)
			{
				dest_data = (void*)(header + NEXT_MEMORY_ALLOC);
				
				return (dest_data);
			}
		}
		else
		{
			header = next_header;
		}
	}
	
	return (NULL);
}

void *VSMAllocGetBlock(vsmalloc_t *vsm, size_t block_size)
{
	size_t i = 0;
	vsm_head_t *header = vsm->allocated;
	vsm_head_t *next_header;
	void *dest_data;
	assert(vsm);	
	
	for (	; i < vsm->number_of_headers; ++i)
	{
		if (!header->is_occupied && header->block_size >= block_size)
		{
			size_t next_header_block_Size = header->block_size - block_size - 
                                                        VSMAllocGetHeaderSize();

			header->block_size = block_size;
			header->is_occupied = TRUE;
			
			next_header = (vsm_head_t*)((char*)header + block_size + 
                                                       VSMAllocGetHeaderSize());
			CreateHead(next_header, next_header_block_Size);
			vsm->number_of_headers = vsm->number_of_headers + 1;

			dest_data = (void*)(header + 1);
			
			return (dest_data);
		}
		else
		{
			header = (vsm_head_t*)((char*)header + header->block_size 
													+ VSMAllocGetHeaderSize());
		}
	}
	
	return (Defragmantation(vsm, block_size));
}

void VSMAllocFreeBlock(void *block)
{
	vsm_head_t *header = (vsm_head_t *)((char*)block - VSMAllocGetHeaderSize());
	
	#ifndef NDEBUG
	if (MAGIC_NUM != header->magic_num)
	{
		return;
	}
	#endif
	
	header->is_occupied = FALSE;
}

size_t VSMAllocGetBiggestFreeBlockSize(vsmalloc_t *vsm)
{
	size_t biggest_free = 0;
	vsm_head_t *iter = vsm->allocated;
	size_t i = 0;
	assert(vsm);
	Defragmantation(vsm, 0);
	
	for (	; i < vsm->number_of_headers; ++i)
	{
		if (iter->block_size >= biggest_free && !iter->is_occupied)
		{
			biggest_free = iter->block_size;
		}
		
		iter = (vsm_head_t*)((char*)iter + VSMAllocGetHeaderSize()
                                                       + iter->block_size);	
	}

	return (biggest_free);
}

size_t VSMAllocSupportedBlockSize(size_t block_size)
{
	if (block_size % BOUNDERY != 0)
	{
		block_size = block_size + (BOUNDERY - block_size % BOUNDERY);
	}
	
	return (block_size);
}

size_t VSMAllocGetHeaderSize()
{
	return (sizeof(vsm_head_t));
}
