/**
@file		ringbuf.h
@brief		ring buffer routines

@author		Nicola
@date		07/01/2013
@version	01.0
*/

#ifndef _RINGBUF_H_
#define _RINGBUF_H_

//#include "stdDataTypes.h"
//#include "alloc.h"
#include "global.h"

/**
 * Enable disable interrupt read/write protection:
 * 	- 1: Enabled
 * 	- 0: Disabled
 */
#define RING_BUFFER_DI_EI 		0

/**
 * Ring buffer struct
 */
struct ringBuffer_t
{
	void 	*pBuf;			// buffer dati allocato in dinamica
	int 	 size;			// buffer size paddato
	int 	 dataSize;
	int	 	 count;			// count locale per test
	int		 idxMem;		// indice dentro
	int		 idxGet;		// indice fuori
	int 	 idxMemRound;
	int 	 idxGetRound;
};
#ifdef __cplusplus
#else
typedef struct ringBuffer_t ringBuffer_t;
#endif

void ringBuffer_init(ringBuffer_t *__rb, int __size, int __dataSize) 			__attribute__ (( section(".ramcode") ));
void ringBuffer_static_init(ringBuffer_t *__rb, void *__pBuf,
							int __size, int __dataSize)							__attribute__ (( section(".ramcode") ));

int ringBuffer_resize(ringBuffer_t *__rb, int __newsize)						__attribute__ (( section(".ramcode") ));
int ringBuffer_read(ringBuffer_t *__rb, void *__pBuf, int __count)				__attribute__ (( section(".ramcode") ));
int ringBuffer_read_isr(ringBuffer_t *__rb, void *__pBuf, int __count)			__attribute__ (( section(".ramcode") ));
int ringBuffer_content_read( ringBuffer_t *__rb, void *__pBuf, int __count) 	__attribute__ (( section(".ramcode") ));
int ringBuffer_write(ringBuffer_t *__rb, const void *__pBuf, int __count)		__attribute__ (( section(".ramcode") ));
int ringBuffer_write_isr(ringBuffer_t *__rb, const void *__pBuf, int __count)	__attribute__ (( section(".ramcode") ));
void ringBuffer_get(ringBuffer_t *__rb, void *__c)								__attribute__ (( section(".ramcode") ));
void ringBuffer_put(ringBuffer_t *__rb, void *__c)								__attribute__ (( section(".ramcode") ));
void ringBuffer_get_isr(ringBuffer_t *__rb, void *__c)							__attribute__ (( section(".ramcode") ));
void ringBuffer_put_isr(ringBuffer_t *__rb, void *__c)							__attribute__ (( section(".ramcode") ));
int ringBuffer_used(ringBuffer_t *__rb)											__attribute__ (( section(".ramcode") ));
int ringBuffer_used_isr(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
int ringBuffer_size(ringBuffer_t *__rb)											__attribute__ (( section(".ramcode") ));
bool ringBuffer_valid(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
int ringBuffer_isEmpty(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
int ringBuffer_isEmpty_isr(ringBuffer_t *__rb)									__attribute__ (( section(".ramcode") ));

int ringBuffer_isfull(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
int ringBuffer_isfull_isr(ringBuffer_t *__rb)									__attribute__ (( section(".ramcode") ));

int ringBuffer_isvalid(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
void ringBuffer_clear(ringBuffer_t *__rb)										__attribute__ (( section(".ramcode") ));
void ringBuffer_clear_isr(ringBuffer_t *__rb)									__attribute__ (( section(".ramcode") ));


#endif /* _RINGBUF_H_ */
