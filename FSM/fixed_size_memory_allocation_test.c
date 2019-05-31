/*******************************************************************************
* Name: Fixed Size Memory Allocation Tests                                     *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* Last Update: 2.12.18                                                         *
*******************************************************************************/
#include <stdio.h>
#include <string.h> /* strcpy, strcmp */

#include "../include/test.h"
#include "../include/fixed_size_memory_allocation.h"

#define BLOCK_SIZE 16
#define NUMBER_OF_BLOCKS 100
#define BAD_BLOCK_SIZE 17
#define GOOD_BLOCK_SIZE 24
#define BITS_IN_BYTES 8
#define WORD_BYTES (__WORDSIZE / BITS_IN_BYTES)
/******************************************************************************/
TestResult TestFSMAllocCreateDestroy();
TestResult TestFSMAllocCountFree();
TestResult TestFSMAllocSupportedBlockSize();
TestResult TestFSMAllocGetBlock();
TestResult TestFSMAllocFreeBlock();
TestResult TestFSMAllocContent();
/******************************************************************************/
int main()
{
	RUN_TEST(TestFSMAllocCreateDestroy);
	RUN_TEST(TestFSMAllocCountFree);
	RUN_TEST(TestFSMAllocSupportedBlockSize);
	RUN_TEST(TestFSMAllocGetBlock);
	RUN_TEST(TestFSMAllocFreeBlock);
	RUN_TEST(TestFSMAllocContent);
	
	return (0);
}
/******************************************************************************/

TestResult TestFSMAllocCreateDestroy()
{
	fsmalloc_t *fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);

	REQUIRE(NULL != fsm);
	FSMAllocDestroy(fsm);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestFSMAllocCountFree()
{
	fsmalloc_t *fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);

	REQUIRE(NUMBER_OF_BLOCKS == FSMAllocCountFree(fsm));
	FSMAllocDestroy(fsm);

	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestFSMAllocGetBlock()
{
	void *start_of_allocated_block = NULL;
	fsmalloc_t *fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);

	start_of_allocated_block = FSMAllocGetBlock(fsm);
	REQUIRE(NULL != start_of_allocated_block);
	REQUIRE(NUMBER_OF_BLOCKS - 1 == FSMAllocCountFree(fsm));
	FSMAllocDestroy(fsm);

	fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);
	
	while (FSMAllocCountFree(fsm))
	{
		REQUIRE(NULL != FSMAllocGetBlock(fsm));
	}  

	FSMAllocDestroy(fsm);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestFSMAllocFreeBlock()
{
	void *start_of_allocated_block = NULL;
	fsmalloc_t *fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);

	start_of_allocated_block = FSMAllocGetBlock(fsm);
	FSMAllocFreeBlock(fsm, start_of_allocated_block);
	REQUIRE(NUMBER_OF_BLOCKS == FSMAllocCountFree(fsm));
	start_of_allocated_block = FSMAllocGetBlock(fsm);
	REQUIRE(NULL != start_of_allocated_block);
	
	FSMAllocDestroy(fsm);
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestFSMAllocSupportedBlockSize()
{
	size_t returned_block_size = FSMAllocSupportedBlockSize(BAD_BLOCK_SIZE);
	
	REQUIRE(returned_block_size % WORD_BYTES == 0);
	REQUIRE(returned_block_size < BAD_BLOCK_SIZE + WORD_BYTES);
	REQUIRE(returned_block_size >= BAD_BLOCK_SIZE);

	returned_block_size	= FSMAllocSupportedBlockSize(WORD_BYTES);
	REQUIRE(returned_block_size == WORD_BYTES);
	
	returned_block_size	= FSMAllocSupportedBlockSize(2 * WORD_BYTES);
	REQUIRE(returned_block_size == 2 * WORD_BYTES);
	
	returned_block_size	= FSMAllocSupportedBlockSize(WORD_BYTES - 1);
	REQUIRE(returned_block_size == WORD_BYTES);
	
	return (TEST_PASS);
}
/******************************************************************************/

TestResult TestFSMAllocContent()
{
	size_t i = 0;
	char* str = "sixteen len str";
	fsmalloc_t *fsm = FSMAllocCreate(BLOCK_SIZE, NUMBER_OF_BLOCKS);
	char* blocks[NUMBER_OF_BLOCKS];
	
	for ( ; i < NUMBER_OF_BLOCKS ; ++i)
	{
		blocks[i] = FSMAllocGetBlock(fsm);
		strcpy(blocks[i], str);
/*		printf("\n1for: %u , %s",i,blocks[i]);*/
	}
	
	for (i = 0 ; i < NUMBER_OF_BLOCKS ; ++i)
	{
		REQUIRE(0 == strcmp(blocks[i],str));
/*		printf("\n2for: %u , %s",i,blocks[i]);*/
	}
		for (i = 0 ; i < NUMBER_OF_BLOCKS ; ++i)
	{
		FSMAllocFreeBlock(fsm, blocks[i]);
/*		printf("\n2for: %u , %s",i,blocks[i]);*/
	}
	
	for (i = 0 ; i < NUMBER_OF_BLOCKS ; ++i)
	{
		blocks[i] = FSMAllocGetBlock(fsm);
		strcpy(blocks[i], str);
/*		printf("\n1for: %u , %s",i,blocks[i]);*/
	}
	
	for (i = 0 ; i < NUMBER_OF_BLOCKS ; ++i)
	{
		REQUIRE(0 == strcmp(blocks[i],str));
/*		printf("\n2for: %u , %s",i,blocks[i]);*/
	}

	FSMAllocDestroy(fsm);
	return (TEST_PASS);
}
/******************************************************************************/
/******************************************************************************/
