/*******************************************************************************
*Description: variable size memory allocation                                  *
*Created BY Bareket Sar Shalom                                                 *
*Date: 4.11.18                                                                 *
*******************************************************************************/
#include <stdio.h>
#include "../include/test.h"
#include "../include/variable_size_memory_allocation.h"
/******************************************************************************/
#define NUMBER_OF_BYTES 1024

#define FIRST_BLOCK 50
#define SECOND_BLOCK 800
#define THIRD_BLOCK 10
#define FORTH_BLOCK 25
/******************************************************************************/
TestResult TestVSMAllocSupportedBlockSize();
TestResult TestGetBiggestFreeBlockSize();
TestResult TestDefragmentationGetBlock();
/******************************************************************************/

int main()
{
	RUN_TEST(TestVSMAllocSupportedBlockSize);
	RUN_TEST(TestGetBiggestFreeBlockSize);
	RUN_TEST(TestDefragmentationGetBlock);
	
	return (0);
}

TestResult TestVSMAllocSupportedBlockSize()
{
	REQUIRE(24 == VSMAllocSupportedBlockSize(17));
	REQUIRE(32 == VSMAllocSupportedBlockSize(26));
	REQUIRE(8 == VSMAllocSupportedBlockSize(3));		
	REQUIRE(0 == VSMAllocSupportedBlockSize(0));
	REQUIRE(8 == VSMAllocSupportedBlockSize(8));						
		
	return (TEST_PASS);
}

TestResult TestGetBiggestFreeBlockSize()
{	
	vsmalloc_t *vsm = VSMAllocCreate(NUMBER_OF_BYTES);

	VSMAllocGetBlock(vsm, FIRST_BLOCK);
	REQUIRE(NUMBER_OF_BYTES - 2 * VSMAllocGetHeaderSize() - FIRST_BLOCK == 
										VSMAllocGetBiggestFreeBlockSize(vsm));

	VSMAllocDestroy(vsm);

	return (TEST_PASS);
}

TestResult TestDefragmentationGetBlock()
{
	vsmalloc_t *vsm = VSMAllocCreate(NUMBER_OF_BYTES);

	void *second_block = NULL;	
	void *third_block = NULL;
			
	VSMAllocGetBlock(vsm, FIRST_BLOCK);
	second_block = VSMAllocGetBlock(vsm, SECOND_BLOCK);
	third_block = VSMAllocGetBlock(vsm, THIRD_BLOCK);
	VSMAllocGetBlock(vsm, FORTH_BLOCK);

	REQUIRE(NUMBER_OF_BYTES - 5 * VSMAllocGetHeaderSize() - 
	FIRST_BLOCK - SECOND_BLOCK - THIRD_BLOCK - FORTH_BLOCK == 
										VSMAllocGetBiggestFreeBlockSize(vsm));

	VSMAllocFreeBlock(second_block);

	REQUIRE(SECOND_BLOCK == VSMAllocGetBiggestFreeBlockSize(vsm));
	VSMAllocFreeBlock(third_block);
	REQUIRE(SECOND_BLOCK + THIRD_BLOCK + VSMAllocGetHeaderSize() ==
										 VSMAllocGetBiggestFreeBlockSize(vsm));
	VSMAllocDestroy(vsm);

	return (TEST_PASS);	
}

