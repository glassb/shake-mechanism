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
		
    Filename: "QX_Parsing_Functions.h"
-----------------------------------------------------------------*/

#ifndef QX_PARSING_FUNCTIONS_H
#define QX_PARSING_FUNCTIONS_H

//****************************************************************************
// Headers
//****************************************************************************
#include <stdlib.h>
#include <stdint.h>		// for Standard Data Types

//****************************************************************************
// Defines
//****************************************************************************


//****************************************************************************
// Types
//****************************************************************************
typedef enum {
	QB_Parser_Dir_Read,
	QB_Parser_Dir_WriteDel,
	QB_Parser_Dir_WriteAbs,
} QB_Parser_Dir_e;

//****************************************************************************
// Global Variables
//****************************************************************************
extern QB_Parser_Dir_e rw;

//****************************************************************************
// Public Function Prototypes
//****************************************************************************
void QX_Parser_SetMsgPtr(uint8_t *p);
void QX_Parser_AdvMsgPtr(void);
volatile uint8_t *QX_Parser_GetMsgPtr(void);
void QX_Parser_SetDir_Read(void);
void QX_Parser_SetDir_WriteRel(void);
void QX_Parser_SetDir_WriteAbs(void);
QB_Parser_Dir_e QX_Parser_GetDir(void);
void QX_Parser_Dir_ForceWriteAbs_Set(void);
void QX_Parser_Dir_ForceWriteAbs_Reset(void);

//****************************************************************************
// Private Function Prototypes
//****************************************************************************
void AddFloatAsSignedLong(float *v, uint32_t n, float scaleto);
void AddFloatAsSignedShort(float *v, uint32_t n, float scaleto);
void AddFloatAsSignedChar(float *v, uint32_t n, float scaleto);
void AddFloatAsUnsignedChar(float *v, uint32_t n, float scaleto);
void AddFloatAsUnsignedShort(float *v, uint32_t n, float scaleto);
void GetFloatAsSignedLong(float *v, uint32_t n, float max, float min, float scalefrom);
void GetFloatAsSignedShort(float *v, uint32_t n, float max, float min, float scalefrom);
void GetFloatAsSignedChar(float *v, uint32_t n, float max, float min, float scalefrom);
void GetFloatAsUnsignedChar(float *v, uint32_t n, float max, float min, float scalefrom);
void GetFloatAsUnsignedShort(float *v, uint32_t n, float max, float min, float scalefrom);
void AddSignedLongAsSignedLong(int32_t *v, uint32_t n);
void AddSignedLongAsSignedShort(int32_t *v, uint32_t n);
void AddSignedLongAsSignedChar(int32_t *v, uint32_t n);
void AddSignedLongAsUnsignedChar(int32_t *v, uint32_t n);
void GetSignedLongAsSignedLong(int32_t *v, uint32_t n, int32_t max, int32_t min);
void GetSignedLongAsSignedShort(int32_t *v, uint32_t n, int32_t max, int32_t min);
void GetSignedLongAsSignedChar(int32_t *v, uint32_t n, int32_t max, int32_t min);
void GetSignedLongAsUnsignedChar(int32_t *v, uint32_t n, int32_t max, int32_t min);
void AddSignedShortAsSignedShort(int16_t *v, uint32_t n);
void AddSignedShortAsSignedChar(int16_t *v, uint32_t n);
void AddSignedShortAsUnsignedChar(int16_t *v, uint32_t n);
void GetSignedShortAsSignedShort(int16_t *v, uint32_t n, float max, float min);
void GetSignedShortAsSignedChar(int16_t *v, uint32_t n, float max, float min);
void GetSignedShortAsUnsignedChar(int16_t *v, uint32_t n, int16_t max, int16_t min);
void AddSignedCharAsSignedChar(int8_t *v, uint32_t n);
void GetSignedCharAsSignedChar(int8_t *v, uint32_t n, int8_t max, int8_t min);
void AddUnsignedCharAsUnsignedChar(uint8_t *v, uint32_t n);
void GetUnsignedCharAsUnsignedChar(uint8_t *v, uint32_t n, uint8_t max, uint8_t min);
void AddUnsignedShortAsUnsignedShort(uint16_t *v, uint32_t n);
void GetUnsignedShortAsUnsignedShort(uint16_t *v, uint32_t n, uint16_t max, uint16_t min); 
void AddBitsAsByte(uint8_t *v, uint8_t start_bit, uint8_t n_bits);
void GetBitsAsByte(uint8_t *v, uint8_t start_bit, uint8_t n_bits);

//****************************************************************************
// Public Function Like Macros for 
//****************************************************************************

#define PARSE_FL_AS_SL(value, len, max, min, scale)\
    if(rw == QB_Parser_Dir_Read) { AddFloatAsSignedLong(value, len, scale); } else { GetFloatAsSignedLong(value, len, max, min, 1.0f / scale); }

#define PARSE_FL_AS_SS(value, len, max, min, scale)\
    if(rw == QB_Parser_Dir_Read) { AddFloatAsSignedShort(value, len, scale); } else { GetFloatAsSignedShort(value, len, max, min, 1.0f / scale); }

#define PARSE_FL_AS_SC(value, len, max, min, scale)\
    if(rw == QB_Parser_Dir_Read) { AddFloatAsSignedChar(value, len, scale); } else { GetFloatAsSignedChar(value, len, max, min, 1.0f / scale); }

#define PARSE_FL_AS_UC(value, len, max, min, scale)\
    if(rw == QB_Parser_Dir_Read) { AddFloatAsUnsignedChar(value, len, scale); } else { GetFloatAsUnsignedChar(value, len, max, min, 1.0f / scale); }

#define PARSE_FL_AS_US(value, len, max, min, scale)\
    if(rw == QB_Parser_Dir_Read) { AddFloatAsUnsignedShort(value, len, scale); } else { GetFloatAsUnsignedShort(value, len, max, min, 1.0f / scale); }
	
#define PARSE_SL_AS_SL(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedLongAsSignedLong((int32_t *)value, len); } else { GetSignedLongAsSignedLong((int32_t *)value, len, max, min); }

#define PARSE_SL_AS_SS(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedLongAsSignedShort((int32_t *)value, len); } else { GetSignedLongAsSignedShort((int32_t *)value, len, max, min); }

#define PARSE_SL_AS_SC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedLongAsSignedChar((int32_t *)value, len); } else { GetSignedLongAsSignedChar((int32_t *)value, len, max, min); }

#define PARSE_SL_AS_UC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedLongAsUnsignedChar((int32_t *)value, len); } else { GetSignedLongAsUnsignedChar((int32_t *)value, len, max, min); }

#define PARSE_SS_AS_SS(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedShortAsSignedShort((int16_t *)value, len); } else { GetSignedShortAsSignedShort((int16_t *)value, len, max, min); }

#define PARSE_SS_AS_SC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedShortAsSignedChar((int16_t *)value, len); } else { GetSignedShortAsSignedChar((int16_t *)value, len, max, min); }

#define PARSE_SS_AS_UC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedShortAsUnsignedChar((int16_t *)value, len); } else { GetSignedShortAsUnsignedChar((int16_t *)value, len, max, min); }

#define PARSE_SC_AS_SC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddSignedCharAsSignedChar(value, len); } else { GetSignedCharAsSignedChar(value, len, max, min); }

#define PARSE_UC_AS_UC(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddUnsignedCharAsUnsignedChar(value, len); } else { GetUnsignedCharAsUnsignedChar(value, len, max, min); }

#define PARSE_US_AS_US(value, len, max, min)\
    if(rw == QB_Parser_Dir_Read) { AddUnsignedShortAsUnsignedShort((uint16_t *)value, len); } else { GetUnsignedShortAsUnsignedShort((uint16_t *)value, len, max, min); }

#define PARSE_BITS_AS_UC(value, start_bit, n_bits)\
    if(rw == QB_Parser_Dir_Read) { AddBitsAsByte((uint8_t *)value, start_bit, n_bits); } else { GetBitsAsByte((uint8_t *)value, start_bit, n_bits); }
	
#endif
		
