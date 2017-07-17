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


Filename: "QX_App_Config.h"
-----------------------------------------------------------------*/

#ifndef QX_APP_CONFIG_H
#define QX_APP_CONFIG_H

//****************************************************************************
// Defines
//****************************************************************************
//#define QX_DEBUG // enables printf in QX code

//****************************************************************************
// Headers
//****************************************************************************
#include <stdlib.h>		// for Standard Data Types
#include <stdint.h>		// for Standard Data Types
#include <stdbool.h>

//****************************************************************************
// Definitions
//****************************************************************************
#define QX_NUM_SRV			1
#define QX_NUM_CLI			1


//****************************************************************************
// Data Types
//****************************************************************************

// Communication Ports - port dependent
typedef enum {
	QX_COMMS_PORT_UART = 0,
	//QX_Comms_Port_e = 0,
	QX_NUM_OF_PORTS		// Ensure this remains as the last value because it sets the number of ports!
} QX_Comms_Port_e;

//****************************************************************************
// Public Global Vars
//****************************************************************************


//****************************************************************************
// Public Function Prototypes
//****************************************************************************



#endif
