/******************************************************************************
* Name: Variable Size Memory Allocation                                        *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* Last Update: 2.12.18                                                         *
*******************************************************************************/
#include <stdlib.h> /* size_t */

#ifndef __VS_MALLOC_H__
#define __VS_MALLOC_H__

typedef struct vsmalloc_s vsmalloc_t;
typedef struct vsm_head_s vsm_head_t;

vsmalloc_t *VSMAllocCreate(size_t requested_byte_count);
/*******************************************************************************
* Creates Variable Size Memory Allocation unit                                 *
*                                                                              *
* requested_byte_count - memory to be managed                                  *
*                                                                              *
* return                                                                       *
*  pointer to Variable Size Memory Allocation unit                             *
*******************************************************************************/

void VSMAllocDestroy(vsmalloc_t *vsm);
/*******************************************************************************
* Frees all memory used by a Variable Size Memory Allocation unit              *
*                                                                              *
* vsm - pointer to Variable Size Memory Allocation unit, not NULL              *
*                                                                              *
* return                                                                       *
*  none                                                                        *
*******************************************************************************/

void *VSMAllocGetBlock(vsmalloc_t *vsm, size_t block_size);
/*******************************************************************************
* Allocates a block from a Variable Size Memory Allocation unit                *
*                                                                              *
* vsm - pointer to Variable Size Memory Allocation unit, not NULL,             *
* block_size - size of block to be allocated                                   *
*              should be a supported size, use VSMAllocSupportedBlockSize      *
*                                                                              *
* return                                                                       *
*  pointer to start of block. if not avilable - returns NULL                   *
*******************************************************************************/

void VSMAllocFreeBlock(void *block);
/*******************************************************************************
* Frees a Variable Size Memory Allocation block                                *
*                                                                              *
* block - block to be freed, must be block received with VSMAllocGetBlock      *
*                                                                              *
* return                                                                       *
*  none                                                                        *
*******************************************************************************/

size_t VSMAllocGetBiggestFreeBlockSize(vsmalloc_t *vsm);
/*******************************************************************************
**
*                                                                              *
* vsm - pointer to Variable Size Memory Allocation unit, not NULL              *
*                                                                              *
* return                                                                       *
**
*******************************************************************************/

size_t VSMAllocSupportedBlockSize(size_t block_size);
/*******************************************************************************
* return a supported block size to be used with a                              *
*  Variable Size Memory Allocation unit according to                           *
*  upper word boundry of block_size                                            *
*                                                                              *
* block_size - block size to be checked                                        *
*                                                                              *
* return                                                                       *
*  supported block size                                                        *
*******************************************************************************/

size_t VSMAllocGetHeaderSize();
/*******************************************************************************
* returns the size of the header that is connected to each data segment in the *
*  Variable Size Memory Allocation                                             *
*                                                                              *
* return                                                                       *
*  size of the Variable Size Memory Allocation header                          *
*******************************************************************************/

#endif
