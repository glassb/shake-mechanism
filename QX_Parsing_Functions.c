/*-----------------------------------------------------------------
	Copyright 2017 Freefly Systems

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
		
    Filename: "QX_Parsing_Functions.c"
-----------------------------------------------------------------*/

//****************************************************************************
// Headers
//****************************************************************************
#include "QX_Parsing_Functions.h"
#include <stdlib.h>		// for Standard Data Types
#include <stdint.h>		// for Standard Data Types
#include <math.h>

//****************************************************************************
// Private Global Vars
//****************************************************************************
volatile uint8_t *msgPtr;
QB_Parser_Dir_e rw;
QB_Parser_Dir_e rw_orig;

uint16_t get_uint16_t;

//****************************************************************************
// Private Defines
//****************************************************************************

// Bit Masks
#define BITFIELD_MASK_1                    0x01
#define BITFIELD_MASK_2                    0x03
#define BITFIELD_MASK_3                    0x07
#define BITFIELD_MASK_4                    0x0F
#define BITFIELD_MASK_5                    0x1F
#define BITFIELD_MASK_6                    0x3F
#define BITFIELD_MASK_7                    0x7F
#define BITFIELD_MASK_8                    0xFF

// Parser Macros
#define ADDSL(v) { *msgPtr++ = (uint8_t)(v >> 24); *msgPtr++ = (uint8_t)(v >> 16); *msgPtr++ = (uint8_t)(v >> 8); *msgPtr++ = (uint8_t)(v); }
#define ADDSS(v) { *msgPtr++ = (uint8_t)(v >> 8); *msgPtr++ = (uint8_t)(v); }
#define ADDUS(v) { *msgPtr++ = (uint8_t)(v >> 8); *msgPtr++ = (uint8_t)(v); }
#define ADDCHAR(v) { *msgPtr++ = (uint8_t)(v); }

#define GETUC ((uint8_t)*msgPtr++)
#define GETSC ((int8_t)*msgPtr++)
#define GETSS ((int16_t)((*msgPtr) << 8) | (int16_t)((*(msgPtr + 1)))); msgPtr += 2;
#define GETUS ((uint16_t)((*msgPtr) << 8) | (uint16_t)((*(msgPtr + 1)))); msgPtr += 2;
#define GETSL ((int32_t)((*msgPtr) << 24) | (int32_t)((*(msgPtr + 1)) << 16) | (int32_t)((*(msgPtr + 2)) << 8) | (int32_t)((*(msgPtr + 3)))); msgPtr += 4;


//****************************************************************************
// Public Function Definitions
//****************************************************************************

// Set the Parser Message Pointer. 
// This function is used prior to the first parsing function calls, then the parsing functions increment the pointer as needed.
void QX_Parser_SetMsgPtr(uint8_t *p){
	msgPtr = p;
}

// Move the pointer by one byte (useful for bitfield parser functions that do not advance the pointer on their own
void QX_Parser_AdvMsgPtr(void){
	msgPtr++;
}

// Gets the Parser Pointer
volatile uint8_t *QX_Parser_GetMsgPtr(void){
	return msgPtr;
}

// Sets the Parser to Read
void QX_Parser_SetDir_Read(void){
	rw = QB_Parser_Dir_Read;
}

// Sets the Parser to Delta Write
void QX_Parser_SetDir_WriteRel(void){
	rw = QB_Parser_Dir_WriteDel;
}

// Sets the Parser to Absolute Write
void QX_Parser_SetDir_WriteAbs(void){
	rw = QB_Parser_Dir_WriteAbs;
}

// Get the parsing type
QB_Parser_Dir_e QX_Parser_GetDir(void){
	return rw;
}

// If write, temporarily force the parsing to be absolute, for attributes that have some absolute only values
void QX_Parser_Dir_ForceWriteAbs_Set(void){
	rw_orig = rw;
	if (rw == QB_Parser_Dir_WriteDel){
		rw = QB_Parser_Dir_WriteAbs;
	}
}

// Reset temporary absolute parser forcing
void QX_Parser_Dir_ForceWriteAbs_Reset(void){
	rw = rw_orig;
}

//****************************************************************************
// Private Function Definitions
//****************************************************************************

void AddFloatAsSignedLong(float *v, uint32_t n, float scaleto)
{
    while(n--)
    {
        int32_t value = (int32_t)(((*v) * scaleto) + 0.5f * ((0 < *v) - (*v < 0)));
        ADDSL(value);
        v++;
    }
}

void AddFloatAsSignedShort(float *v, uint32_t n, float scaleto)
{
    while(n--)
    {
        int16_t value = (int16_t)(((*v) * scaleto) + 0.5f * ((0 < *v) - (*v < 0)));
        ADDSS(value);
        v++;
    }
}

void AddFloatAsSignedChar(float *v, uint32_t n, float scaleto)
{
    while(n--)
    {
        int8_t value = (int8_t)(((*v) * scaleto) + 0.5f * ((0 < *v) - (*v < 0)));
        ADDCHAR(value);
        v++;
    }
}

void AddFloatAsUnsignedChar(float *v, uint32_t n, float scaleto)
{
    while(n--)
    {
        uint8_t value = (uint8_t) (((*v) * scaleto) + 0.5f * ((0 < *v) - (*v < 0)));
        ADDCHAR(value);
        v++;
    }
}

void AddFloatAsUnsignedShort(float *v, uint32_t n, float scaleto)
{
    while(n--)
    {
        uint16_t value = (uint16_t) (((*v) * scaleto) + 0.5f * ((0 < *v) - (*v < 0)));
        ADDUS(value);
        v++;
    }
}

void GetFloatAsSignedLong(float *v, uint32_t n, float max, float min, float scalefrom)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            float r = (float)GETSL;
            r = r * scalefrom;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            float r = (float)GETSL;
            r = r * scalefrom;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
}

void GetFloatAsSignedShort(float *v, uint32_t n, float max, float min, float scalefrom)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            float r = (float)GETSS;
            r = r * scalefrom;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            float r = (float)GETSS;
            r = r * scalefrom;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
}

void GetFloatAsSignedChar(float *v, uint32_t n, float max, float min, float scalefrom)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            float r = (float)GETSC;
            r = r * scalefrom;
            *v += r;
            if(max < *v) *v = max;
            else if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            float r = (float)GETSC;
            r = r * scalefrom;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
}

void GetFloatAsUnsignedChar(float *v, uint32_t n, float max, float min, float scalefrom)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            float r = (float)GETSC;
            r = r * scalefrom;
            *v += (float)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            float r = (float)GETUC;
            r = r * scalefrom;
            *v = (float)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
}

void GetFloatAsUnsignedShort(float *v, uint32_t n, float max, float min, float scalefrom)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            float r = (float)GETUS;
            r = r * scalefrom;
            *v += (float)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            float r = (float)GETUS;
            r = r * scalefrom;
            *v = (float)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            if(isnan(*v)) *v = 0.0f;
            v++;
        }
    }
}

void AddSignedLongAsSignedLong(int32_t *v, uint32_t n)
{
    while(n--)
    {
        int32_t value = *v;
        ADDSL(value);
        v++;
    }
}

void AddSignedLongAsSignedShort(int32_t *v, uint32_t n)
{
    while(n--)
    {
        int16_t value = (int16_t)(*v);
        ADDSS(value);
        v++;
    }
}

void AddSignedLongAsSignedChar(int32_t *v, uint32_t n)
{
    while(n--)
    {
        int8_t value = (int8_t)(*v);
        ADDCHAR(value);
        v++;
    }
}

void AddSignedLongAsUnsignedChar(int32_t *v, uint32_t n)
{
    while(n--)
    {
        uint8_t value = (uint8_t)(*v);
        ADDCHAR(value);
        v++;
    }
}

void GetSignedLongAsSignedLong(int32_t *v, uint32_t n, int32_t max, int32_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int32_t r = GETSL;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int32_t r = GETSL;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void GetSignedLongAsSignedShort(int32_t *v, uint32_t n, int32_t max, int32_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int32_t r = (int32_t)GETSS;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int32_t r = (int32_t)GETSS;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void GetSignedLongAsSignedChar(int32_t *v, uint32_t n, int32_t max, int32_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int32_t r = (int32_t)GETSC;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int32_t r = (int32_t)GETSC;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void GetSignedLongAsUnsignedChar(int32_t *v, uint32_t n, int32_t max, int32_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int32_t r = (int32_t)GETSC;    // Force int8_t if relative change
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int32_t r = (int32_t)GETUC;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void AddSignedShortAsSignedShort(int16_t *v, uint32_t n)
{
    while(n--)
    {
        int16_t value = *v;
        ADDSS(value);
        v++;
    }
}

void AddSignedShortAsSignedChar(int16_t *v, uint32_t n)
{
    while(n--)
    {
        int8_t value = (int8_t)(*v);
        ADDCHAR(value);
        v++;
    }
}

void AddSignedShortAsUnsignedChar(int16_t *v, uint32_t n)
{
    while(n--)
    {
        uint8_t value = (uint8_t)(*v);
        ADDCHAR(value);
        v++;
    }
}

void GetSignedShortAsSignedShort(int16_t *v, uint32_t n, float max, float min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int16_t r = GETSS;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int16_t r = GETSS;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void GetSignedShortAsSignedChar(int16_t *v, uint32_t n, float max, float min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int16_t r = (int16_t)GETSC;
            *v += (int16_t)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int16_t r = (int16_t)GETSC;
            *v = (int16_t)r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void GetSignedShortAsUnsignedChar(int16_t *v, uint32_t n, int16_t max, int16_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int16_t r = (int16_t)GETSC;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int16_t r = (int16_t)GETUC;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void AddSignedCharAsSignedChar(int8_t *v, uint32_t n)
{
    while(n--)
    {
        ADDCHAR(*v);
        v++;
    }
}

void GetSignedCharAsSignedChar(int8_t *v, uint32_t n, int8_t max, int8_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int8_t r = GETSC;
            int32_t temp = *v;    // Promote to signed so we can do a meaningful clamp check
            temp += r;
            if(max < temp) temp = max;
            if(temp < min) temp = min;
            *v = (uint8_t)temp;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            int8_t r = GETSC;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void AddUnsignedCharAsUnsignedChar(uint8_t *v, uint32_t n)
{
    while(n--)
    {
        ADDCHAR(*v);
        v++;
    }
}

void GetUnsignedCharAsUnsignedChar(uint8_t *v, uint32_t n, uint8_t max, uint8_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            int8_t r = GETSC;
            int32_t temp = *v;    //Promote to signed so we can do a meaningful clamp check
            temp += r;
            if(max < temp) temp = max;
            if(temp < min) temp = min;
            *v = (uint8_t)temp;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            uint8_t r = GETUC;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

void AddUnsignedShortAsUnsignedShort(uint16_t *v, uint32_t n)
{
    while(n--)
    {
        ADDUS(*v);
        v++;
    }
}

void GetUnsignedShortAsUnsignedShort(uint16_t *v, uint32_t n, uint16_t max, uint16_t min)
{
    if(rw == QB_Parser_Dir_WriteDel)
    {
        while(n--)
        {
            uint16_t r = GETUS;
            *v += r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
    else
    {
        while(n--)
        {
            uint16_t r = GETUS;
            *v = r;
            if(max < *v) *v = max;
            if(*v < min) *v = min;
            v++;
        }
    }
}

// Adds a field of bits to a char
void AddBitsAsByte(uint8_t *v, uint8_t start_bit, uint8_t n_bits)
{
	uint8_t temp = *msgPtr;
	uint8_t mask;
	
	switch (n_bits){
		case 1:
			mask = BITFIELD_MASK_1; break;
		case 2:
			mask = BITFIELD_MASK_2; break;
		case 3:
			mask = BITFIELD_MASK_3; break;
		case 4:
			mask = BITFIELD_MASK_4; break;
		case 5:
			mask = BITFIELD_MASK_5; break;
		case 6:
			mask = BITFIELD_MASK_6; break;
		case 7:
			mask = BITFIELD_MASK_7; break;
		case 8:
			mask = BITFIELD_MASK_8; break;
		default:
			break;
	}
	
	// Move the mask up to the starting position and clear the existing bits in the field
	temp &= ~(mask << start_bit);
	
	// Add the bit field in
	temp |= ((*v & mask) << start_bit);
	*msgPtr = temp;
}

// Applys the bitfield from the buffer to a byte within the range of bits specified. 
// The returned bitfield starts at bit 0 in the byte.
// Relative write direction applies the bitfield as an XOR toggle mask, which flips each bit within the mask area if the bit value is 1.
// Absolute write direction applies the bitfield as an absolute value.
void GetBitsAsByte(uint8_t *v, uint8_t start_bit, uint8_t n_bits)
{
	uint8_t mask, xor_mask;
	
	switch (n_bits){
		case 1:
			mask = BITFIELD_MASK_1; break;
		case 2:
			mask = BITFIELD_MASK_2; break;
		case 3:
			mask = BITFIELD_MASK_3; break;
		case 4:
			mask = BITFIELD_MASK_4; break;
		case 5:
			mask = BITFIELD_MASK_5; break;
		case 6:
			mask = BITFIELD_MASK_6; break;
		case 7:
			mask = BITFIELD_MASK_7; break;
		case 8:
			mask = BITFIELD_MASK_8; break;
		default:
			break;
	}
	
	if(rw == QB_Parser_Dir_WriteDel)
    {
		xor_mask = (*msgPtr >> start_bit) & mask;
		*v ^= xor_mask;
    }
    else
    {
		*v = (*msgPtr >> start_bit) & mask;
    }
}



