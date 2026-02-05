/**
@file		Alloc.h
@brief		Functions to manage memory dynamic allocation.
@see 		Alloc.cpp
@author
@date		18/01/2011
@version	01.00
*/


#ifndef _ALLOC_H_
#define _ALLOC_H_

#include "stdDataTypes.h"
#include "ch.h"
#include <stdio.h>
#include <string.h>
/**
Errors start number.
*/
#define ALLOC_BASE_ERR	1000			//@@@@ BASEMOD_ALLOC


#ifdef ALLOC_ALIGNMENT
	#if (ALLOC_ALIGNMENT != 2 && ALLOC_ALIGNMENT != 4)
	#error "ALLOC_ALIGNMENT not supported!!!!"
	#endif
#else
/**
Memory default alignment.
*/
	#define ALLOC_ALIGNMENT	2
	//#warning "ALLOC_ALIGNMENT set to 2 by default...."
#endif

/**
Allocation Errors
*/
enum ALLOCErrors{ MEM_ERR_NONE,							/**< No Error.*/
				  MEM_ERR_BLOCK_SIZE=ALLOC_BASE_ERR,	/**< Invalid block dimension.*/
				  MEM_ERR_END_OF_MEM,					/**< End of Memory.*/
				  MEM_ERR_INV_POINTER,					/**< Invalid pointer.*/
				  MEM_ERR_INV_BLOCK,					/**< Invalid block. Block can not be deallocated.*/
				};

typedef enum ALLOCErrors ALLOCErrors;


#ifdef __cplusplus
extern "C" {
#endif


void mem_init(void);

void mem_left(uint32_t *freeMem, uint32_t *topMem);
void mem_count(uint32_t *numAllocBlock, uint32_t *numFreeBlock);

bool mem_alloc(uint32_t size, void **pMemBlock);
bool mem_free(void *pblock);


#ifdef __cplusplus
}
#endif


#endif



























