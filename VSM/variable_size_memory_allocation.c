/*******************************************************************************
* Name: variable Size Memory Allocation C file                                 *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* Last Update: 4.12.18                                                         *
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* size_t, malloc, free, __WORDSIZE */

#include "../include/variable_size_memory_allocation.h"
#include "../include/enum_rd58.h"

#define END_MARK 1
#define BITS_IN_BYTES 8
#define WORD_BYTES (__WORDSIZE / BITS_IN_BYTES)
#define MAGIC_NUMBER 0xCAFEBABE

#define IS_FREE(header) (0 <= header->block_size)
/******************************************************************************/

struct vsm_head_s
{
	long block_size;

	#ifndef NDEBUG
		size_t magic_number;
	#endif
};

struct vsmalloc_s
{
	void* end_of_segment;
	vsm_head_t first_header;
};
/**********************Utility Functions***************************************/

static vsm_head_t UtilityHeaderCreate(size_t block_size)
{
	vsm_head_t header;

	header.block_size = block_size;

	#ifndef NDEBUG
		header.magic_number = MAGIC_NUMBER;
	#endif

	return (header);
}

static void UtilityHeadersUpdateAfterGetBlock(vsm_head_t* header, size_t block_size)
{
	assert(header);

	if (header->block_size - block_size >= VSMAllocGetHeaderSize())
  {
		vsm_head_t* new_header = (vsm_head_t*)((char*)header +
												 		 (VSMAllocGetHeaderSize() + block_size));

		*new_header = UtilityHeaderCreate(header->block_size - block_size -
																		  VSMAllocGetHeaderSize());

		header->block_size = - block_size;
 	}
	else
	{
		header->block_size *= -1;
	}
}

static status UtilityMergeTwoHeaders(vsmalloc_t* vsm, void *current_header)
{
	vsm_head_t* next_header = NULL;
	assert(vsm);
	assert(current_header);

	next_header = (vsm_head_t*)((char*)current_header + (VSMAllocGetHeaderSize() +
															((vsm_head_t*)current_header)->block_size));

	if (next_header == vsm->end_of_segment)
	{
		return (FAIL);
	}

	if (IS_FREE(next_header))
	{
		((vsm_head_t*)current_header)->block_size += next_header->block_size +
																							   VSMAllocGetHeaderSize();

		return (SUCCESS);
	}

		return (FAIL);
}
/******************************************************************************/

vsmalloc_t *VSMAllocCreate(size_t requested_byte_count)
{
	vsmalloc_t *vsm = malloc(sizeof (vsmalloc_t) + requested_byte_count
														- VSMAllocGetHeaderSize() + END_MARK);
	if (NULL == vsm)
	{
		return (NULL);
	}

	vsm->end_of_segment = (char*)(&vsm->first_header) + requested_byte_count;
	vsm->first_header = UtilityHeaderCreate(requested_byte_count -
																						sizeof (vsm_head_t));

	return (vsm);
}
/******************************************************************************/

void VSMAllocDestroy(vsmalloc_t *vsm)
{
	assert(vsm);
	free(vsm);
}
/******************************************************************************/

void *VSMAllocGetBlock(vsmalloc_t *vsm, size_t block_size)
{
	vsm_head_t* given_block = NULL;
	assert(vsm);

	given_block = &vsm->first_header;

	while (given_block != vsm->end_of_segment)
	{
		if(!IS_FREE(given_block))
		{
			given_block = (vsm_head_t*)((char*)given_block + (VSMAllocGetHeaderSize() -
										((vsm_head_t*)given_block)->block_size));
			continue;
		}

		if ((size_t)(given_block->block_size) >= block_size)
		{
			UtilityHeadersUpdateAfterGetBlock((vsm_head_t*)given_block, block_size);
			given_block = (vsm_head_t*)((char*)given_block + VSMAllocGetHeaderSize());

			return (given_block);
		}

		if (SUCCESS == UtilityMergeTwoHeaders(vsm, given_block))
		{
			continue;
		}

		given_block = (vsm_head_t*)((char*)given_block + (VSMAllocGetHeaderSize() +
									((vsm_head_t*)given_block)->block_size));
	}

	return (NULL);
}
/******************************************************************************/

void VSMAllocFreeBlock(void *block)
{
	vsm_head_t* header = NULL;
	assert(block);

	header = (vsm_head_t*)block - 1;
	assert(header->magic_number == MAGIC_NUMBER);
	assert(!IS_FREE(header));

	header->block_size *= -1;
}
/******************************************************************************/

size_t VSMAllocGetBiggestFreeBlockSize(vsmalloc_t *vsm)
{
	size_t biggest = 0;
	vsm_head_t* iterator = NULL;
	assert(vsm);

	iterator = &vsm->first_header;

	while (iterator != vsm->end_of_segment)
	{
		if (IS_FREE(iterator))
		{
			biggest = (biggest < (size_t)iterator->block_size) ?
								 (size_t)iterator->block_size : biggest;

			if (SUCCESS == UtilityMergeTwoHeaders(vsm, iterator))
			{
				continue;
			}

			iterator = (vsm_head_t*)((char*)iterator + (VSMAllocGetHeaderSize() +
															 iterator->block_size));

		}
		else
		{
			iterator = (vsm_head_t*)((char*)iterator + (VSMAllocGetHeaderSize() -
															 iterator->block_size));
		}

	}

	return (biggest);
}
/******************************************************************************/

size_t VSMAllocSupportedBlockSize(size_t block_size)
{
	if (0 == block_size % WORD_BYTES)
	{
		return (block_size);
	}
	else
	{
		size_t coefficient = block_size / WORD_BYTES ;
		return (WORD_BYTES + (WORD_BYTES * coefficient));
	}
}
/******************************************************************************/

size_t VSMAllocGetHeaderSize()
{
	return (sizeof (vsm_head_t));
}
/*******************************************************************************
*******************************************************************************/
