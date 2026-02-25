/**
@file		Alloc.cpp
@brief		Functions to manage memory dynamic allocation.

			Here are dcumented only the functions not present in the file Alloc.h
@see 		Alloc.h
@author
@date		18/01/2011
@version	01.00
*/
#include "alloc.h"


/**
Heap base address. Defined in linker module.
*/

extern uint8_t __heap_base__;

/**
Heap end address. Defined in linker module.
*/

extern uint8_t __heap_end__;


/**
Top of user heap.
*/
static uint8_t *pTopHeap = &__heap_base__;

static ALLOCErrors memError;

/**
Small heap allocated blocks number.
*/
static uint16_t cntAllocSmall;
/**
Small heap freed blocks number.
*/
static uint16_t cntFreeSmall;


/**
Heap allocated blocks number.
*/
static uint32_t cntAlloc;
/**
Heap freed blocks number.
*/
static uint32_t cntFree;
/**
Heap free memory.
*/
static uint32_t memLeft;


/**
Small heap and Heap memory initialization.
*/
void mem_init(void)
{

	memError = MEM_ERR_NONE;
	pTopHeap = &__heap_base__;				// inizio heap definito dal linker
	memLeft = &__heap_end__ - &__heap_base__;	// memoria attualmente libera

	cntAlloc = 0;						// reset blocchi allocati & liberati
	cntFree = 0;
	cntAllocSmall = 0;					// numero di blocchi di memoria allocati
	cntFreeSmall = 0;					// numero di blocchi di memoria liberati

}

/**
	Returns the number of allocation and deallocation executed on heap memory.

	@param numAllocBlock number of allocation done.
	@param numFreeBlock number of deallocation done.
*/
void mem_count(uint32_t *numAllocBlock, uint32_t *numFreeBlock)
{

	*numAllocBlock = cntAlloc;
	*numFreeBlock = cntFree;
}

/**
	Returns the heap free memory.

	@param freeMem free memory NOT fragmented.
	@param topMem comprehensive free memory (fragmented).
*/
void mem_left(uint32_t *freeMem, uint32_t *topMem)
{

	*topMem = (uint32_t) (&__heap_end__ - pTopHeap);	// memoria ancora libera libera (non frammentata)
	*freeMem = memLeft;							// memoria libera complessiva (frammentata)
}

/**
	Memory dynamic allocation - Heap.
	- the maximum allocatable block dimension is 2Gb (0x7FFFFFFF)
	- the most significant bit, if 1, marks the block as freed
	- only 4 uint8_ts are used to each block beyond the block size.
	@param size block dimension. It has to be lower than or equal to 0x800000.
	@param pMemBlock memory pointer.

*/
bool mem_alloc(uint32_t size, void **pMemBlock)
{
uint32_t *pmem = NULL;
uint32_t *pfree;
uint8_t *pheap;
uint32_t topMem, blockSize;


	*pMemBlock = NULL;							// iniziamo bene

#if (ALLOC_ALIGNMENT == 2)						// se la vogliono allineata alla solita uint16_t
	if( (size & 0x0001) )						// allinea alla uint16_t
		size++;
#elif (ALLOC_ALIGNMENT == 4)					// se sono un po' + spreconi
	if( (size & 0x0001) )						// allinea alla uint16_t
		size++;

	if( !((size + 2)& 0x0002) )					// se non basta (considera comunque gli extra)
		size += 2;								// allinea alla long uint16_t
#else
	if( (size & 0x0001) )						// allinea alla uint16_t
		size++;									// allineamento d'ufficio .....
#endif

	if( size > 0x800000 )						// controlla comunque la dim. massima del blocco
	{
#ifdef __DEBUG__
		memError = MEM_ERR_BLOCK_SIZE;
		error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);
#endif
	}


	if( memLeft - 4 < size )					// se complessivamente non basta
	{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
	}


	topMem = (uint32_t) (&__heap_end__ - pTopHeap);	// memoria ancora libera libera (non frammentata)

	if( topMem - 4 > size )						// se ci sta nel blocco non frammentato
	{
		pmem = (uint32_t *) pTopHeap;				// punto di partenza del prossimo blocco
		*pmem = size;							// marca la dimensione del blocco

		pTopHeap += size + 4;					// nuovo punto di partenza heap
	}
	else
	{
		for(pheap=&__heap_base__; pheap<pTopHeap; )	// cerca tra le frattaglie
		{
			pmem = (uint32_t *) pheap;

			if( (*pmem & 0x80000000) != 0 )			// se il blocco è
			{
				if( (*pmem & 0x7FFFFFFF) >= size )	// se è grande abbastanza
				{
					blockSize = *pmem & 0x7FFFFFFF;	// registra: lo rimette in gioco

					if( blockSize > size + 20 )	// se ce ne avanza un bel goccio
					{
						*pmem = size;			// alloca solo quello che serve

						pheap += size + 4;		// va all'inizio del prossimo blocco

						pfree = (uint32_t *) pheap; 			// passa all'ala
												// marca come se fosse un blocco liberato
						*pfree = 0x80000000 | (blockSize - size);
					}
					else
					{
						*pmem = size = blockSize;	// tiene anche quello che avanza
					}
					break;						// fine della ricerca
				}
			}

			pheap += (*pmem & 0x7FFFFFFF) + 4;		// avanti all'inizio del prossimo blocco
		}

		if( pheap == pTopHeap )					// se non trova un buco libero
		{
#ifdef __DEBUG__
			memError = MEM_ERR_END_OF_MEM;		// non ce n'è proprio +
			error_trace("ALLOC", "mem_alloc", memError);	// segnala la cappella sistina
			return(False);
#else
			return(False);
#endif
		}

	}

	memLeft -= size + 4;
	if ( pmem )
	{
      *pMemBlock = (void *) (pmem+1);				// inizio reale del blocco (4 uint8_ts dopo)
    }
    else
    {
      return False;								// tutto ok!!
    }
	cntAlloc++;

	return True;								// tutto ok!!
}

/**
Frees the memory at pblock - Heap.
@param pblock address of the block to free.
*/
bool mem_free(void *pblock)
{
uint8_t *pheap = (uint8_t *) pblock;
uint32_t *pmem = (uint32_t *) pblock;

	if( pheap == NULL || pheap < &__heap_base__ || pheap > &__heap_end__ )	// se spara fuori
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_POINTER;
		error_trace("ALLOC", "mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}

	pmem--;											// punta al descrittore del blocco

	if( (*pmem & 0x80000000) != 0 )					// se era già cancellato
	{
#ifdef __DEBUG__
		memError = MEM_ERR_INV_BLOCK;
		error_trace("ALLOC", "mem_free", memError);	// segnala la cappella sistina
		return(False);
#else
		return(False);								// si limita al dissenso
#endif
	}


	pheap = (uint8_t *) pmem;							// inizio del blocco

	pheap += *pmem + 4;								// punta all'inizio del blocco succ.

	if( pheap == pTopHeap )							// se è l'ultimo blocco allocato
	{

		memLeft += (*pmem) + 4;						// tiene conto della memoria recuperata

		do{											// verifica se qualche altro blocco
													// adiacente all'ultimo liberato era
													// stato liberato in precedenza
			pTopHeap = (uint8_t *) pmem;				// nuovo punto di partenza heap libera
													// il blocco non solo e' libero ma non
													// esiste proprio +
			pmem = (uint32_t *) &__heap_end__;

			for(pheap=&__heap_end__; pheap<pTopHeap;) // trova l'ultimo blocco prima del top
			{
				pmem = (uint32_t *) pheap;

				pheap += (*pmem & 0x7FFFFFFF) + 4;	// avanza
			}
													// in pmem rimane l'inizio dell'ultimo blocco
													// prima del "tetto"
			if( (*pmem & 0x80000000) == 0 )				// se il penultimo blocco e' ancora in uso
				break;								// non lo posso buttare via

		}while( pmem != (uint32_t *) pheap );			// fino a che ne trova uno

	}
	else
	{
		memLeft += *pmem + 4;						// tiene conto della memoria recuperata
		*pmem |= 0x80000000;						// marca il blocco come libero
	}

	cntFree++;										// un blocco in + liberato

	return(True);
}



extern "C" {

void * malloc(size_t size)
{
void *p;

	if( mem_alloc(size, &p) == False )
		return(NULL);

	return(p);
}


void *calloc(size_t _nmemb, size_t size)
{
    void *p = NULL;
    bool returnVal = false;

	size *= _nmemb;

    returnVal = mem_alloc(size, &p);
	if ((p != NULL) && (returnVal == true))
	{
	  memset(p, 0, sizeof(p) * _nmemb);
	  return (p);
	}
	else
    {
	  return NULL;
    }
}

void *_calloc_r(struct _reent *pr, size_t _nmemb, size_t size)
{
void *p;

	while( pr->_inc != 0 );

	pr->_inc++;

	p = calloc(_nmemb, size);

	pr->_inc--;

	return( p );
}

void free(void *p)
{
	mem_free(p);
}

}		// extern "C"













