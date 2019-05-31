/*******************************************************************************
* Name: Fixed Size Memory Allocation Tests                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* Last Update: 4.12.18                                                         *
*******************************************************************************/
#include <stdio.h>	/* printf */
#include <string.h> /* strcpy, strcmp */

#include "../include/test.h"
#include "../include/variable_size_memory_allocation.h"

#define TOTAL_BYTES 160
#define BAD_BLOCK_SIZE 17
#define GOOD_BLOCK_SIZE 24
#define SMALL_SIZE_BLOCK 8
#define BIG_SIZE_BLOCK 64

#define BITS_IN_BYTES 8
#define WORD_BYTES (__WORDSIZE / BITS_IN_BYTES)
/******************************************************************************/

TestResult TestVSMAllocCreateDestroy();
TestResult TestVSMAllocGetBlock();
TestResult TestVSMAllocGetBiggestFreeBlockSize();
TestResult TestVSMAllocGetBlockDefragmentation();
TestResult TestVSMAllocContent();
TestResult TestVSMAllocSupportedBlockSize();
/******************************************************************************/

int main()
{
	RUN_TEST(TestVSMAllocCreateDestroy);
	RUN_TEST(TestVSMAllocGetBlock);
	RUN_TEST(TestVSMAllocGetBiggestFreeBlockSize);
	RUN_TEST(TestVSMAllocGetBlockDefragmentation);
	RUN_TEST(TestVSMAllocContent);
	RUN_TEST(TestVSMAllocSupportedBlockSize);
	
	return (0);
}
/******************************************************************************/

TestResult TestVSMAllocCreateDestroy()
{
	vsmalloc_t *vsm = VSMAllocCreate(TOTAL_BYTES);

	REQUIRE(NULL != vsm);
	VSMAllocDestroy(vsm);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestVSMAllocGetBlock()
{
	void *start_of_allocated_block = NULL;
	vsmalloc_t *vsm = VSMAllocCreate(TOTAL_BYTES);

	start_of_allocated_block = VSMAllocGetBlock(vsm, GOOD_BLOCK_SIZE);
	REQUIRE(NULL != start_of_allocated_block);

	VSMAllocDestroy(vsm);

	return (TEST_PASS);
}
/******************************************************************************/
TestResult TestVSMAllocGetBiggestFreeBlockSize()
{	
	vsmalloc_t *vsm = VSMAllocCreate(TOTAL_BYTES);
	REQUIRE(SMALL_SIZE_BLOCK < VSMAllocGetBiggestFreeBlockSize(vsm));
	VSMAllocDestroy(vsm);
	
	return (TEST_PASS);	
}
/******************************************************************************/

TestResult TestVSMAllocGetBlockDefragmentation()
{
	size_t i = 0 , number_of_free_block = 3;
	char* blocks[TOTAL_BYTES];
	
	vsmalloc_t *vsm = VSMAllocCreate(TOTAL_BYTES);
	
	for ( ; SMALL_SIZE_BLOCK < VSMAllocGetBiggestFreeBlockSize(vsm) ; ++i)
	{
		blocks[i] = VSMAllocGetBlock(vsm, SMALL_SIZE_BLOCK);
	}
	
	for (i = 0; i < number_of_free_block ; ++i)
	{
		VSMAllocFreeBlock(blocks[i]);
	}

	REQUIRE(NULL != VSMAllocGetBlock(vsm, SMALL_SIZE_BLOCK * 2));
	VSMAllocDestroy(vsm);
	
	return (TEST_PASS);	
}
/******************************************************************************/

TestResult TestVSMAllocContent()
{
	int i = 0;
	char* str8 = "eightln";
	char* str16 = "sixteen len str";
	vsmalloc_t *vsm = VSMAllocCreate(TOTAL_BYTES);
	char* blocks[TOTAL_BYTES];
	
	for ( ; SMALL_SIZE_BLOCK < VSMAllocGetBiggestFreeBlockSize(vsm) ; ++i)
	{
		blocks[i] = VSMAllocGetBlock(vsm, SMALL_SIZE_BLOCK);
		strcpy(blocks[i], str8);
	}
	
	for (--i ; i >= 0 ; --i)
	{
		REQUIRE(0 == strcmp(blocks[i],str8));
/*		printf("\n2for: %lu , %s", i,blocks[i]);*/
		VSMAllocFreeBlock(blocks[i]);
	}

	for (i = 0 ; BIG_SIZE_BLOCK < VSMAllocGetBiggestFreeBlockSize(vsm) ; ++i)
	{
		blocks[i] = VSMAllocGetBlock(vsm, BIG_SIZE_BLOCK);
		strcpy(blocks[i], str16);
/*		printf("\n1for: %lu , %s",i,blocks[i]);*/
	}
	
	for (--i ; i >= 0 ; --i)
	{
		REQUIRE(0 == strcmp(blocks[i],str16));
/*		printf("\n2for: %lu , %s", i, blocks[i]);*/
	}
	
	VSMAllocDestroy(vsm);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestVSMAllocSupportedBlockSize()
{
	size_t returned_block_size = VSMAllocSupportedBlockSize(BAD_BLOCK_SIZE);
	
	REQUIRE(0 == returned_block_size % WORD_BYTES);
	REQUIRE(returned_block_size < BAD_BLOCK_SIZE + WORD_BYTES);
	REQUIRE(returned_block_size >= BAD_BLOCK_SIZE);

	returned_block_size	= VSMAllocSupportedBlockSize(WORD_BYTES);
	REQUIRE(WORD_BYTES == returned_block_size);
	
	returned_block_size	= VSMAllocSupportedBlockSize(2 * WORD_BYTES);
	REQUIRE(returned_block_size == 2 * WORD_BYTES);
	
	returned_block_size	= VSMAllocSupportedBlockSize(WORD_BYTES - 1);
	REQUIRE(WORD_BYTES == returned_block_size);
	
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
