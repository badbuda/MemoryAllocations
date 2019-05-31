/*******************************************************************************
*Created BY Bareket Sar Shalom                                                 *
*Date: 30.11.18                                                                *
*******************************************************************************/
#include "../include/fixed_size_memory_allocation.h" 
#include "../include/enum_rd58.h" 
#include "../include/test.h"
/******************************************************************************/

TestResult TestCountFree();
TestResult TestSupportedBlockSize();
TestResult TestGetBlock();
TestResult TestFSMAllocFreeBlock();

int main()
{
	RUN_TEST(TestCountFree);
	RUN_TEST(TestSupportedBlockSize);
	RUN_TEST(TestGetBlock);
	RUN_TEST(TestFSMAllocFreeBlock);
	
	return (SUCCESS);
}

TestResult TestCountFree()
{
	size_t number_of_blocks = 3;
	size_t block_size = 5;
	fsmalloc_t *fsm = FSMAllocCreate(block_size, number_of_blocks);
	
	REQUIRE(number_of_blocks == FSMAllocCountFree(fsm));
	
	FSMAllocDestroy(fsm);

	return (TEST_PASS);
}

TestResult TestSupportedBlockSize()
{
	REQUIRE(24 == FSMAllocSupportedBlockSize(17));
	REQUIRE(32 == FSMAllocSupportedBlockSize(26));
	REQUIRE(8 == FSMAllocSupportedBlockSize(3));		
	REQUIRE(0 == FSMAllocSupportedBlockSize(0));
	REQUIRE(8 == FSMAllocSupportedBlockSize(8));						
	return (TEST_PASS);
}

TestResult TestGetBlock()
{
	size_t number_of_blocks = 3;
	size_t block_size = 5;

	fsmalloc_t *fsm = FSMAllocCreate(block_size, number_of_blocks);

	FSMAllocGetBlock(fsm);

	REQUIRE(number_of_blocks -1 == FSMAllocCountFree(fsm));
	FSMAllocDestroy(fsm);

	return (TEST_PASS);
}

TestResult TestFSMAllocFreeBlock()
{
	size_t number_of_blocks = 3;
	size_t block_size = 5;
	void *get_block = NULL;
	fsmalloc_t *fsm = FSMAllocCreate(block_size, number_of_blocks);

	get_block = FSMAllocGetBlock(fsm);
	REQUIRE(number_of_blocks -1 == FSMAllocCountFree(fsm));
		
	FSMAllocFreeBlock(fsm, get_block);
	REQUIRE(number_of_blocks == FSMAllocCountFree(fsm));

	FSMAllocDestroy(fsm);

	return (TEST_PASS);
}
