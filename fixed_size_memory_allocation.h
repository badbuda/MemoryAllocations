/******************************************************************************
* Name: Fixed Size Memory Allocation                                           *
*                                                                              *
* Author: Bareket Umiel Sar Shalom                                             *
* Last Update: 29.11.18                                                        *
*******************************************************************************/
#include <stdlib.h> /* size_t */

#ifndef __FS_MALLOC_H__
#define __FS_MALLOC_H__

typedef struct fsmalloc_s fsmalloc_t;

fsmalloc_t *FSMAllocCreate(size_t block_size, size_t number_of_blocks);
/*******************************************************************************
* Creates Fixed Size Memory Allocation unit                                    *
*                                                                              *
* block_size - size to be allocated for each block,                            *
*              should be a supported size, use FSMAllocSupportedBlockSize      *
* number_of_blocks - number of blocks to be allocated                          *
*                                                                              *
* return                                                                       *
*  pointer to Fixed Size Memory Allocation unit                                *
*******************************************************************************/

void FSMAllocDestroy(fsmalloc_t *fsm);
/*******************************************************************************
* Frees all memory used by a Fixed Size Memory Allocation unit                 *
*                                                                              *
* fsm - pointer to Fixed Size Memory Allocation unit, not NULL                 *
*                                                                              *
* return                                                                       *
*  none                                                                        *
*******************************************************************************/

void *FSMAllocGetBlock(fsmalloc_t *fsm);
/*******************************************************************************
* Allocates a block from a Fixed Size Memory Allocation unit                   *
*                                                                              *
* fsm - pointer to Fixed Size Memory Allocation unit, not NULL,                *
*       must have free blocks                                                  *
*                                                                              *
* return                                                                       *
*  pointer to start of block                                                   *
*******************************************************************************/

void FSMAllocFreeBlock(fsmalloc_t *fsm, void *block);
/*******************************************************************************
* Frees a Fixed Size Memory Allocation block for future use                    *
*                                                                              *
* fsm - pointer to Fixed Size Memory Allocation unit, not NULL                 *
* block - block to be freed, must be block revived with FSMAllocGetBlock       *
*                                                                              *
* return                                                                       *
*  none                                                                        *
*******************************************************************************/

size_t FSMAllocCountFree(fsmalloc_t *fsm);
/*******************************************************************************
* Counts the number of blocks that can be requested from a                     *
*  Fixed Size Memory Allocation unit                                           *
*                                                                              *
* fsm - pointer to Fixed Size Memory Allocation unit, not NULL                 *
*                                                                              *
* return                                                                       *
*  number of blocks that can be requested                                      *
*******************************************************************************/

size_t FSMAllocSupportedBlockSize(size_t block_size);
/*******************************************************************************
* return a supported block size to be used with a                              *
*  Fixed Size Memory Allocation unit according to                              *
*  upper word boundry of block_size                                            *
*                                                                              *
* block_size - block size to be checked                                        *
*                                                                              *
* return                                                                       *
*  supported block size                                                        *
*******************************************************************************/

#endif
