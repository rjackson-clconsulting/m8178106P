/**
@file		ringbuf.h
@brief		ring buffer routines

@author		Nicola
@date		07/01/2013
@version	01.00
*/

#include "ringbuf.h"

static void ringBuffer_alloc(ringBuffer_t *__rb, int __size , int __dataSize)	__attribute__ (( section(".ramcode") ));
static void ringBuffer_destroy(ringBuffer_t *__rb)								__attribute__ (( section(".ramcode") ));
static void ringBuffer_zeroIndex(ringBuffer_t *__rb)							__attribute__ (( section(".ramcode") ));
inline void my_memcpy(void *__dst, const void *__src, uint32_t __size)			__attribute__ (( section(".ramcode") ));


void ringBuffer_init(ringBuffer_t *__rb, int __size, int __dataSize)
{
	ringBuffer_alloc(__rb,__size, __dataSize);
}

void ringBuffer_static_init(ringBuffer_t *__rb, void *__pBuf, int __size, int __dataSize)
{
	__rb->pBuf = __pBuf;
	__rb->size = __size;
	__rb->dataSize = __dataSize;
	ringBuffer_zeroIndex(__rb);
}

int ringBuffer_resize(ringBuffer_t *__rb, int __newsize)
{
	if( __rb->pBuf )
	{
		ringBuffer_destroy(__rb);
	}
	ringBuffer_alloc(__rb, __newsize, __rb->dataSize);
	return __rb->pBuf != NULL;
}// ridimensiona il buffer di lavoro

int ringBuffer_read(ringBuffer_t *__rb, void *__pBuf, int __count)
{
	int delta = 0;

	if( __rb->pBuf == NULL )
		return 0;

	if( __rb->idxGet + __count <= __rb->size )		// se ci sta in un sol fiato
	{
		my_memcpy(	__pBuf,
					(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize),
					__count * __rb->dataSize);	// copia tutto

		__rb->idxGet += __count;	// avanti l'indice di copia

		if( __rb->idxGet >= __rb->size )
		{
			#if(RING_BUFFER_DI_EI == 1)
			__disable_irq();
			#endif
			__rb->idxGetRound++;
			__rb->idxGet = 0;
			#if(RING_BUFFER_DI_EI == 1)
			__enable_irq();
			#endif
		}
	}
	else
	{
		delta = __rb->size - __rb->idxGet;					// rimanenti nel fondo del sacco

		__count -= delta;

		my_memcpy(	__pBuf,
					(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize),
					delta * __rb->dataSize);	// copia il primo pezzo

		#if(RING_BUFFER_DI_EI == 1)
		__disable_irq();
		#endif
		__rb->idxGetRound++;
		__rb->idxGet = 0;
		#if(RING_BUFFER_DI_EI == 1)
		__enable_irq();
		#endif
		my_memcpy(	(void*)(__pBuf + delta*__rb->dataSize),
					__rb->pBuf,
					__count * __rb->dataSize);	// copia il resto

		__rb->idxGet += __count;	// avanti l'indice di copia
	}

	return delta + __count;
}

int ringBuffer_read_isr(ringBuffer_t *__rb,void *__pBuf, int __count)
{
	int delta = 0;

	if( __rb->pBuf == NULL )
		return 0;

	if( __rb->idxGet + __count <= __rb->size )		// se ci sta in un sol fiato
	{
		my_memcpy(	__pBuf,
					(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize),
					__count * __rb->dataSize);	// copia tutto

		__rb->idxGet += __count;	// avanti l'indice di copia

		if( __rb->idxGet >= __rb->size )
		{
			__rb->idxGetRound++;
			__rb->idxGet = 0;
		}
	}
	else
	{
		delta = __rb->size - __rb->idxGet;					// rimanenti nel fondo del sacco

		__count -= delta;

		my_memcpy(	__pBuf,
					(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize),
					delta * __rb->dataSize);	// copia il primo pezzo

		__rb->idxGetRound++;
		__rb->idxGet = 0;

		my_memcpy(	(void*)(__pBuf + delta*__rb->dataSize),
					__rb->pBuf,
					__count * __rb->dataSize);	// copia il resto

		__rb->idxGet += __count;	// avanti l'indice di copia
	}

	return delta + __count;
}

int content_read( ringBuffer_t *__rb, void *__pBuf, int __count)
	{
		int delta = 0;
		int bufused = ringBuffer_used(__rb);
		int idxRead = __rb->idxGet;

		if( __rb->pBuf == NULL )
				return 0;

		//if( bufused == 0 )
				//return 0;

		if( bufused < __count )					// se tutto non ci sta...
				return 0;					// ritorniamo quello che c'è

		if( idxRead + __count <= __rb->size )		// se ci sta in un sol fiato
		{
			my_memcpy(	__pBuf,
					(void*)(__rb->pBuf + idxRead*__rb->dataSize), __count * __rb->dataSize);	// copia tutto
		}
		else
		{
			delta = __rb->size - idxRead;					// rimanenti nel fondo del sacco
			__count -= delta;
			my_memcpy(	__pBuf,
						(void*)(__rb->pBuf + idxRead*__rb->dataSize),
						delta * __rb->dataSize);	// copia il primo pezzo

			my_memcpy( 	(void*)(__pBuf + delta*__rb->dataSize),
						__rb->pBuf,
						__count * __rb->dataSize);	// copia il resto

		}
		return delta + __count;
	}

int ringBuffer_write(ringBuffer_t *__rb, const void *__pBuf, int __count)
{
	int delta = 0;
	int bufused = ringBuffer_used(__rb);

	if( __rb->pBuf == NULL )
		return 0;

	if( __rb->size - bufused < __count )		// se tutto non ci sta...
		return 0;

	if( __rb->idxMem + __count <= __rb->size )		// se ci sta in un sol fiato
	{
		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					__pBuf,
					__count * __rb->dataSize);	// copia tutto

		__rb->idxMem += __count;	// avanti l'indice di copia

		if( __rb->idxMem >= __rb->size )
		{
			#if(RING_BUFFER_DI_EI == 1)
			__disable_irq();
			#endif
			__rb->idxMemRound++;
			__rb->idxMem = 0;
			#if(RING_BUFFER_DI_EI == 1)
			__enable_irq();
			#endif
		}
	}
	else
	{
		delta = __rb->size - __rb->idxMem;					// rimanenti nel fondo del sacco

		__count -= delta;

		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					__pBuf,
					delta * __rb->dataSize);	// copia il primo pezzo

		#if(RING_BUFFER_DI_EI == 1)
		__disable_irq();
		#endif
		__rb->idxMemRound++;
		__rb->idxMem = 0;
		#if(RING_BUFFER_DI_EI == 1)
		__enable_irq();
		#endif

		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					(void*)(__pBuf + delta*__rb->dataSize ),
					__count * __rb->dataSize);	// copia il resto

		__rb->idxMem += __count;	// avanti l'indice di copia
	}
	return __count + delta;
}

int ringBuffer_write_isr(ringBuffer_t *__rb, const void *__pBuf, int __count)
{
	int delta = 0;
	int bufused = ringBuffer_used_isr(__rb);

	if( __rb->pBuf == NULL )
		return 0;

	if( __rb->size - bufused < __count )		// se tutto non ci sta...
		return 0;

	if( __rb->idxMem + __count <= __rb->size )		// se ci sta in un sol fiato
	{
		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					__pBuf,
					__count * __rb->dataSize);	// copia tutto

		__rb->idxMem += __count;	// avanti l'indice di copia

		if( __rb->idxMem >= __rb->size )
		{
			__rb->idxMemRound++;
			__rb->idxMem = 0;
		}
	}
	else
	{
		delta = __rb->size - __rb->idxMem;					// rimanenti nel fondo del sacco

		__count -= delta;

		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					__pBuf,
					delta * __rb->dataSize);	// copia il primo pezzo

		__rb->idxMemRound++;
		__rb->idxMem = 0;		// va a capo

		my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
					(void*)(__pBuf + delta*__rb->dataSize ),
					__count * __rb->dataSize);	// copia il resto

		__rb->idxMem += __count;	// avanti l'indice di copia
	}
	return __count + delta;
}

void ringBuffer_get(ringBuffer_t *__rb, void *__c)
{
	my_memcpy(	__c,
				(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize ),
				__rb->dataSize);
	 __rb->idxGet++;
	if(__rb->idxGet >= __rb->size)
	{
		#if(RING_BUFFER_DI_EI == 1)
		__disable_irq();
		#endif
		__rb->idxGetRound++;	//incrementare prima __rb->idxGetRound per evitare errori di overrun a interrupt attivati
		__rb->idxGet = 0;
		#if(RING_BUFFER_DI_EI == 1)
		__enable_irq();
		#endif
	}
}

void ringBuffer_put(ringBuffer_t *__rb, void *__c)
{
	my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
				__c,
				__rb->dataSize);

	__rb->idxMem++;
	if(__rb->idxMem >= __rb->size)
	{
		#if(RING_BUFFER_DI_EI == 1)
		__disable_irq();
		#endif
		__rb->idxMemRound++;
		__rb->idxMem = 0;
		#if(RING_BUFFER_DI_EI == 1)
		__enable_irq();
		#endif
	}
 }

		// routines dedicate per il transfer da routines irq
void ringBuffer_get_isr(ringBuffer_t *__rb, void *__c)
{
	my_memcpy(	__c,
				(void*)(__rb->pBuf + __rb->idxGet*__rb->dataSize ),
				__rb->dataSize);
	__rb->idxGet++;
	if(__rb->idxGet >= __rb->size)
	{
		__rb->idxGetRound++;
		__rb->idxGet = 0;
	}
}

void ringBuffer_put_isr(ringBuffer_t *__rb, void *__c)
{
	my_memcpy(	(void*)(__rb->pBuf + __rb->idxMem*__rb->dataSize ),
				__c,
				__rb->dataSize);
	__rb->idxMem++;
	if(__rb->idxMem >= __rb->size)
	{
		__rb->idxMemRound++;
		__rb->idxMem = 0;
	}
}

int ringBuffer_used(ringBuffer_t *__rb)
{
	int bufused;

	#if(RING_BUFFER_DI_EI == 1)
	__disable_irq();
	#endif
	register int idxMem = __rb->idxMem;
	register int idxMemRound = __rb->idxMemRound;

	register int idxGet = __rb->idxGet;
	register int idxGetRound = __rb->idxGetRound;
	#if(RING_BUFFER_DI_EI == 1)
	__enable_irq();
	#endif
	if( idxMem >= idxGet )	// questo è facile
	{
		if(idxGetRound == idxMemRound)
		{
			bufused = idxMem - idxGet;
		}else
		{
			bufused = __rb->size;
		}
	}
	else
	{
		bufused = __rb->size - idxGet;
		bufused += idxMem;
	}
	return( bufused );
}

int ringBuffer_used_isr(ringBuffer_t *__rb)
{
	int bufused;

	register int idxMem = __rb->idxMem;
	register int idxMemRound = __rb->idxMemRound;

	register int idxGet = __rb->idxGet;
	register int idxGetRound = __rb->idxGetRound;

	if( idxMem >= idxGet )	// questo è facile
	{
		if(idxGetRound == idxMemRound)
		{
			bufused = idxMem - idxGet;
		}else
		{
			bufused = __rb->size;
		}
	}
	else
	{
		bufused = __rb->size - idxGet;
		bufused += idxMem;
	}
	return( bufused );
}

int ringBuffer_size(ringBuffer_t *__rb)
{
	return __rb->size;
}

bool ringBuffer_valid(ringBuffer_t *__rb)
{
	return __rb->pBuf != NULL;
}

int ringBuffer_isEmpty(ringBuffer_t *__rb)
{
	return (__rb->idxMem == __rb->idxGet) && (__rb->idxGetRound == __rb->idxMemRound);
}

int ringBuffer_isEmpty_isr(ringBuffer_t *__rb)
{
	return (__rb->idxMem == __rb->idxGet) && (__rb->idxGetRound == __rb->idxMemRound);
}

int ringBuffer_isfull(ringBuffer_t *__rb)
{
	return (ringBuffer_used(__rb) >= __rb->size);
}

int ringBuffer_isfull_isr(ringBuffer_t *__rb)
{
	return (ringBuffer_used_isr(__rb) >= __rb->size);
}

int ringBuffer_isvalid(ringBuffer_t *__rb)
{
	return __rb->pBuf != NULL;
}

void ringBuffer_clear(ringBuffer_t *__rb)
{
	#if(RING_BUFFER_DI_EI == 1)
	__disable_irq();
	#endif
	__rb->count = __rb->idxMem = __rb->idxGet = __rb->idxMemRound = __rb->idxGetRound =0;
	#if(RING_BUFFER_DI_EI == 1)
	__enable_irq();
	#endif
}

void ringBuffer_clear_isr(ringBuffer_t *__rb)
{
	__rb->count = __rb->idxMem = __rb->idxGet = __rb->idxMemRound = __rb->idxGetRound =0;
}


static void ringBuffer_alloc(ringBuffer_t *__rb, int __size, int __dataSize)
{
	if( mem_alloc(__size*__dataSize, ((void*)__rb->pBuf)) == False )		// se non ce la fa
		__rb->pBuf = NULL;
	if( __rb->pBuf != NULL )
		__rb->size = __size;		// buffer size paddato
	else
		__rb->size = 0; 			// buffer size annullato
	__rb->dataSize = __dataSize;
	ringBuffer_zeroIndex(__rb);
}

static void ringBuffer_destroy(ringBuffer_t *__rb)
{
	mem_free(__rb->pBuf);
	__rb->pBuf = NULL;
	__rb->size = 0;
	ringBuffer_zeroIndex(__rb);
}

static void ringBuffer_zeroIndex(ringBuffer_t *__rb)
{
	__rb->count = 0;				// counter per ? profiling ? boh
	__rb->idxMem = 0;				// indice dentro
	__rb->idxGet = 0;				// indice fuori
	__rb->idxGetRound = 0;
	__rb->idxMemRound = 0;
}

inline void my_memcpy(void *__dst, const void *__src, uint32_t __size)
{
	while (__size > 0)
	{
		__size--;
		((uint8_t*)__dst)[__size] = ((uint8_t*)__src)[__size];
	}
}
