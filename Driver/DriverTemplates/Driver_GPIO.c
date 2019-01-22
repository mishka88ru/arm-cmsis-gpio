/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_GPIO.h"

#define ARM_GPIO_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_GPIO_API_VERSION,
    ARM_GPIO_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_GPIO_CAPABILITIES DriverCapabilities = {
    1, /* supports rising edge interrupts */
    1, /* supports falling edge interrupts */
    0, /* supports both edges interrupts */
    0, /* supports level-1 sensitive interrupts */
    0  /* supports level-0 sensitive interrupts */
};

//
//   Functions
//

ARM_DRIVER_VERSION ARM_GPIO_GetVersion(void)
{
}

ARM_GPIO_CAPABILITIES ARM_GPIO_GetCapabilities(void)
{
}

int32_t ARM_GPIO_Initialize(ARM_GPIO_SignalEvent_t cb_event)
{
}

int32_t ARM_GPIO_Uninitialize(void)
{
}

int32_t ARM_GPIO_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

int32_t ARM_GPIO_Control(uint32_t control, uint32_t arg)
{
}

ARM_GPIO_STATUS ARM_GPIO_GetStatus(void)
{
}

void SetPort(uint32_t mask)
{
}

void ClearPort(uint32_t mask)
{
}

void TogglePort(uint32_t mask)
{
}

void WritePort(uint32_t values)
{
}

uint32_t ReadPort(void)
{
}

uint32_t GetPortEvents(void)
{
}

void ClearPortEvents(uint32_t mask)
{
}

int32_t ControlPin(uint32_t pin, uint32_t control, uint32_t arg)
{
}

void SetPin(uint32_t pin)
{
}

void ClearPin(uint32_t pin)
{
}

void TogglePin(uint32_t pin)
{
}

void WritePin(uint32_t pin, uint32_t value)
{
}

uint32_t ReadPin(uint32_t pin)
{
}

void ARM_GPIO_SignalEvent(uint32_t event)
{
    // function body
}

// End GPIO Interface

ARM_DRIVER_GPIO Driver_GPIO = {
    ARM_GPIO_GetVersion,
    ARM_GPIO_GetCapabilities,
    
    ARM_GPIO_Initialize,
    ARM_GPIO_Uninitialize,
    ARM_GPIO_PowerControl,
    ARM_GPIO_Control,
    ARM_GPIO_GetStatus,
    
    ARM_GPIO_SetPort,
    ARM_GPIO_ClearPort,
    ARM_GPIO_TogglePort,
    ARM_GPIO_WritePort,
    ARM_GPIO_ReadPort,
    ARM_GPIO_GetPortEvents,
    ARM_GPIO_ClearPortEvents,
    
    ARM_GPIO_ControlPin,
    ARM_GPIO_SetPin,
    ARM_GPIO_ClearPin,
    ARM_GPIO_TogglePin,
    ARM_GPIO_WritePin,
    ARM_GPIO_ReadPin
};
