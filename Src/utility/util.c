/**
@file		util.c
@brief		General Purpose utility routines in C
@author		Fernando Morani
@date		04/05/2017
@version	01.00
@source     M75-CPU Protective
*/

#include "util.h"


/**
 *****************************************************************************
 ** \brief Memory clear function
 *****************************************************************************/
void struct_memclr(uint8_t* pu8Address, uint32_t u32Count)
{
    while(u32Count--)
    {
        *pu8Address++ = 0;
    }
}

/**
 Iterative function
 \return 1 if the bit has found
        0xFF if no index could be extracted
**/
uint8_t extractBit(uint32_t bitMask)
{
uint8_t u8RetVal = NO_INDEX_AVAIL;

    if (bitMask % 2 == 0)
        u8RetVal = 1;
    else
        u8RetVal = NO_INDEX_AVAIL;

return u8RetVal;
}

/**
 *****************************************************************************
 ** \brief Extract Index From a bit Mask
 ** i.e.:
 ** for a BitMask = 0b00000000000000000000000000100000 the index is 5
 **\param bitMask
 **\return index
 *****************************************************************************/
uint8_t extratcIndexFromBitMask (uint32_t bitMask)
{
uint8_t u8RetVal = NO_INDEX_AVAIL;
bool flag = false;
uint32_t tempBitMask = 0;

    if (bitMask == 0){
        u8RetVal = NO_INDEX_AVAIL; //No bit are selected
        return u8RetVal;
        }else {
            u8RetVal = 0;
        }

    if (bitMask / 2 == 0){
        u8RetVal = 0; //Only 1 is possible... is the zero index bit
        return u8RetVal;
        }else{
            u8RetVal = 1;
        }

    if (bitMask / 2 == 1){
        if (bitMask % 2 == 0){
            u8RetVal = 1;
            return u8RetVal;
        }else {
        u8RetVal = NO_INDEX_AVAIL; //This is number 3... No single index for this pattern
        return u8RetVal;
        }
        }else {
            //u8RetVal++;
        }
    tempBitMask = bitMask;
    while (flag != true){
        u8RetVal = u8RetVal + extractBit(tempBitMask);
        if (u8RetVal >= NO_INDEX_AVAIL){
                u8RetVal = NO_INDEX_AVAIL;
                break;
            }else {
                    tempBitMask = tempBitMask / 2;
                    if (tempBitMask / 2 >= 2){
                        flag = false;
                    }else {
                        flag = true;  //terminate research
                    }
                }
                        }

return u8RetVal;
}
