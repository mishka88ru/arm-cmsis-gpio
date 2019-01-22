/*
 * Copyright (c) 2013-2017 ARM Limited. All rights reserved.
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
 *
 * $Date:        18. Jan 2019
 * $Revision:    V0.0
 *
 * Project:      GPIO (General Purpose Input Output)
 *               Driver definitions
 */

/* History:
 *  Version 0.00
 *    experimental
 */

#ifndef DRIVER_GPIO_H_
#define DRIVER_GPIO_H_

#ifdef  __cplusplus
extern "C"
{
#endif

#include "Driver_Common.h"

#define ARM_GPIO_API_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(0,0)  /* API version */


/****** GPIO Control Codes *****/

	
/****** GPIO specific error codes *****/


/****** GPIO Pin Control Codes *****/

#define ARM_GPIO_PIN_CFG                  (0x01)     ///< Command to configure all pin parameters at once; arg = configuration.
#define ARM_GPIO_PIN_STATE                (0x02)     ///< Command to configure pin as GPIO; arg = configuration.
#define ARM_GPIO_PIN_DIRECTION            (0x03)     ///< Configure directions of the port's pins; arg = directions (0 - input, 1 - output)
#define ARM_GPIO_PIN_IRQ                  (0x04)     ///< Command to configure interrupt on the pin; arg = configuration.
#define ARM_GPIO_PIN_PULL                 (0x05)     ///< Command to configure pull-up/-down register on the pin; arg = configuration.
#define ARM_GPIO_PIN_SPEED                (0x06)     ///< Command to configure output speed of the pin; arg = configuration.
#define ARM_GPIO_PIN_OPEN_DRAIN           (0x07)     ///< Command to configure open-drain output on the pin; arg = configuration.
#define ARM_GPIO_PIN_DRIVE_STRENGTH       (0x08)     ///< Command to configure open-drain output on the pin; arg = configuration.

/****** GPIO Pin Control - single commands arguments codes *****/
#define ARM_GPIO_PIN_STATE_DISABLE        (0x00)     ///< Disable pin.
#define ARM_GPIO_PIN_STATE_ENABLE         (0x01)     ///< Enable pin.

#define ARM_GPIO_PIN_DIRECTION_INPUT      (0x00)     ///< Pin is an input.
#define ARM_GPIO_PIN_DIRECTION_OUTPUT     (0x01)     ///< Pin is an output.

#define ARM_GPIO_PIN_IRQ_NONE             (0x00)     ///< Interrupt disabled.
#define ARM_GPIO_PIN_IRQ_RISING           (0x01)     ///< Interrupt on rising edge.
#define ARM_GPIO_PIN_IRQ_FALLING          (0x02)     ///< Interrupt on falling edge.
#define ARM_GPIO_PIN_IRQ_BOTH             (0x03)     ///< Interrupt on both edges.
#define ARM_GPIO_PIN_IRQ_LEVEL_HIGH       (0x04)     ///< Interrupt when logic 1.
#define ARM_GPIO_PIN_IRQ_LEVEL_LOW        (0x05)     ///< Interrupt when logic 0.

#define ARM_GPIO_PIN_PULL_NONE            (0x00)     ///< No pull-up or pull-down register.
#define ARM_GPIO_PIN_PULL_UP              (0x01)     ///< Activate pull-up register.
#define ARM_GPIO_PIN_PULL_DOWN            (0x02)     ///< Activate pull-down register.

#define ARM_GPIO_PIN_SPEED_VERY_HIGH      (0x00)     ///< Very high speed.
#define ARM_GPIO_PIN_SPEED_HIGH           (0x01)     ///< High speed.
#define ARM_GPIO_PIN_SPEED_MEDIUM         (0x02)     ///< Medium speed.
#define ARM_GPIO_PIN_SPEED_LOW            (0x03)     ///< Low speed.

#define ARM_GPIO_PIN_OPEN_DRAIN_DISABLE   (0x00)     ///< Disable open-drain.
#define ARM_GPIO_PIN_OPEN_DRAIN_ENABLE    (0x01)     ///< Enable open-drain.

#define ARM_GPIO_PIN_DRIVE_STRENGTH_LOW   (0x00)     ///< Low drive strength.
#define ARM_GPIO_PIN_DRIVE_STRENGTH_HIGH  (0x01)     ///< High drive strength.

	
/****** GPIO Pin Control - CFG command's argument structure *****/
#define ARM_GPIO_PIN_CFG_Pos                   0
#define ARM_GPIO_PIN_CFG_Msk                  (((1 << (ARM_GPIO_PIN_CFG_DRIVE_STRENGTH_Pos + 1)) - 1) << ARM_GPIO_PIN_CFG_Pos)

#define ARM_GPIO_PIN_CFG_ENABLED_Pos           0
#define ARM_GPIO_PIN_CFG_ENABLED_Msk          (ARM_GPIO_PIN_STATE_ENABLE << ARM_GPIO_PIN_CFG_ENABLED_Pos)
#define ARM_GPIO_PIN_CFG_ENABLED              ARM_GPIO_PIN_CFG_ENABLED_Msk              ///< State of the pin: enabled (1) or disabled (0).

#define ARM_GPIO_PIN_CFG_OUTPUT_Pos            1
#define ARM_GPIO_PIN_CFG_OUTPUT_Msk           (ARM_GPIO_PIN_DIRECTION_OUTPUT << ARM_GPIO_PIN_CFG_OUTPUT_Pos)
#define ARM_GPIO_PIN_CFG_OUTPUT               ARM_GPIO_PIN_CFG_OUTPUT_Msk               ///< Direction of the pin: output (1) or input (0).

#define ARM_GPIO_PIN_CFG_IRQ_Pos               2
#define ARM_GPIO_PIN_CFG_IRQ_Msk              (0x07ul << ARM_GPIO_PIN_CFG_IRQ_Pos)
#define ARM_GPIO_PIN_CFG_IRQ_NONE             (ARM_GPIO_PIN_IRQ_NONE       << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt disabled.
#define ARM_GPIO_PIN_CFG_IRQ_RISING           (ARM_GPIO_PIN_IRQ_RISING     << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt on rising edge.
#define ARM_GPIO_PIN_CFG_IRQ_FALLING          (ARM_GPIO_PIN_IRQ_FALLING    << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt on falling edge.
#define ARM_GPIO_PIN_CFG_IRQ_BOTH             (ARM_GPIO_PIN_IRQ_BOTH       << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt on both edges.
#define ARM_GPIO_PIN_CFG_IRQ_LEVEL_HIGH       (ARM_GPIO_PIN_IRQ_LEVEL_HIGH << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt when logic 1.
#define ARM_GPIO_PIN_CFG_IRQ_LEVEL_LOW        (ARM_GPIO_PIN_IRQ_LEVEL_LOW  << ARM_GPIO_PIN_CFG_IRQ_Pos)        ///< Interrupt when logic 0.

#define ARM_GPIO_PIN_CFG_PULL_Pos              5
#define ARM_GPIO_PIN_CFG_PULL_Msk             (0x03ul << ARM_GPIO_PIN_CFG_PULL_Pos)
#define ARM_GPIO_PIN_CFG_PULL_NONE            (ARM_GPIO_PIN_PULL_NONE << ARM_GPIO_PIN_CFG_PULL_Pos)            ///< No pull-up or pull-down register.
#define ARM_GPIO_PIN_CFG_PULL_UP              (ARM_GPIO_PIN_PULL_UP   << ARM_GPIO_PIN_CFG_PULL_Pos)            ///< Activate pull-up register.
#define ARM_GPIO_PIN_CFG_PULL_DOWN            (ARM_GPIO_PIN_PULL_DOWN << ARM_GPIO_PIN_CFG_PULL_Pos)            ///< Activate pull-down register.

#define ARM_GPIO_PIN_CFG_SPEED_Pos             7
#define ARM_GPIO_PIN_CFG_SPEED_Msk            (0x03ul << ARM_GPIO_PIN_CFG_SPEED_Pos)
#define ARM_GPIO_PIN_CFG_SPEED_VERY_HIGH      (ARM_GPIO_PIN_SPEED_VERY_HIGH << ARM_GPIO_PIN_CFG_SPEED_Pos)     ///< Very high speed.
#define ARM_GPIO_PIN_CFG_SPEED_HIGH           (ARM_GPIO_PIN_SPEED_HIGH      << ARM_GPIO_PIN_CFG_SPEED_Pos)     ///< High speed.
#define ARM_GPIO_PIN_CFG_SPEED_MEDIUM         (ARM_GPIO_PIN_SPEED_MEDIUM    << ARM_GPIO_PIN_CFG_SPEED_Pos)     ///< Medium speed.
#define ARM_GPIO_PIN_CFG_SPEED_LOW            (ARM_GPIO_PIN_SPEED_LOW       << ARM_GPIO_PIN_CFG_SPEED_Pos)     ///< Low speed.

#define ARM_GPIO_PIN_CFG_OPEN_DRAIN_Pos        9
#define ARM_GPIO_PIN_CFG_OPEN_DRAIN_Msk       (ARM_GPIO_PIN_OPEN_DRAIN_ENABLE << ARM_GPIO_PIN_CFG_OPEN_DRAIN_Pos)
#define ARM_GPIO_PIN_CFG_OPEN_DRAIN           ARM_GPIO_PIN_CFG_OPEN_DRAIN_Msk               ///< Configure open drain on the pin: open-drain enabled (1) or disabled (0).

#define ARM_GPIO_PIN_CFG_DRIVE_STRENGTH_Pos    10
#define ARM_GPIO_PIN_CFG_DRIVE_STRENGTH_Msk   (ARM_GPIO_PIN_DRIVE_STRENGTH_HIGH << ARM_GPIO_PIN_CFG_DRIVE_STRENGTH_Pos)
#define ARM_GPIO_PIN_CFG_DRIVE_STRENGTH       ARM_GPIO_PIN_CFG_DRIVE_STRENGTH_Msk           ///< Configure drive strength of the pin: high (1) or low (0).


/**
\brief GPIO Status
*/
typedef volatile struct _ARM_GPIO_STATUS {
  uint32_t reserved        : 32;       ///< Active events bitmask
} ARM_GPIO_STATUS;


/****** GPIO Event *****/
// Event argument is just a bitmask of GPIO pins, waiting for interrupt processing.


// Function documentation
/**
  \fn          ARM_DRIVER_VERSION ARM_GPIO_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
  
  \fn          ARM_GPIO_CAPABILITIES ARM_GPIO_GetCapabilities (void)
  \brief       Get driver capabilities
  \return      \ref ARM_GPIO_CAPABILITIES
  
  \fn          int32_t ARM_GPIO_Initialize (ARM_GPIO_SignalEvent_t cb_event)
  \brief       Initialize GPIO Interface.
  \param[in]   cb_event  Pointer to \ref ARM_GPIO_SignalEvent
  \return      \ref execution_status
  
  \fn          int32_t ARM_GPIO_Uninitialize (void)
  \brief       De-initialize GPIO Interface.
  \return      \ref execution_status
  
  \fn          int32_t ARM_GPIO_PowerControl (ARM_POWER_STATE state)
  \brief       Control GPIO Interface Power.
  \param[in]   state  Power state
  \return      \ref execution_status
  
  \fn          int32_t ARM_GPIO_Control (uint32_t control, uint32_t arg)
  \brief       Control GPIO Interface.
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation (optional)
  \return      common \ref execution_status and driver specific \ref GPIO_execution_status
  
  \fn          ARM_GPIO_STATUS ARM_GPIO_GetStatus (void)
  \brief       Get GPIO status.
  \return      GPIO status \ref ARM_GPIO_STATUS
  
  
  \fn          void SetPort (uint32_t mask)
  \brief       Set port pins values to 1s.
  \param[in]   mask  Pins to set to 1.
  \return      none
  
  \fn          void ClearPort (uint32_t mask)
  \brief       Clear port pins values to 0s.
  \param[in]   mask  Pins to clear to 0.
  \return      none

  \fn          void TogglePort (uint32_t mask)
  \brief       Toggle port pins values to 1.
  \param[in]   mask  Pins to toggle.
  \return      none

  \fn          void WritePort (uint32_t values)
  \brief       Write port pins values.
  \param[in]   values  Values of all pins.
  \return      none

  \fn          uint32_t ReadPort ()
  \brief       Read port pins values.
  \return      Current values of pins.

  \fn          uint32_t GetPortEvents ()
  \brief       Get port events mask.
  \return      Current events.
  
  \fn          void ClearPortEvents (uint32_t mask)
  \brief       Clear port events mask.
  \param[in]   mask  Events to clear.
  \return      none
  
  
  \fn          int32_t ARM_GPIO_ControlPin (uint32_t pin, uint32_t control, uint32_t arg)
  \brief       Control GPIO Pin Interface.
  \param[in]   pin      Pin index
  \param[in]   control  Operation
  \param[in]   arg      Argument of operation
  \return      common \ref execution_status and driver specific \ref GPIO_execution_status
  
  \fn          void SetPin (uint32_t pin)
  \brief       Set pin value to 1.
  \param[in]   pin      Pin index
  \return      none
  
  \fn          void ClearPin (uint32_t pin)
  \brief       Clear pin value to 0.
  \param[in]   pin      Pin index
  \return      none

  \fn          void TogglePin (uint32_t pin)
  \brief       Toggle pin value.
  \param[in]   pin      Pin index
  \return      none
  
  \fn          void WritePin (uint32_t pin, uint32_t value)
  \brief       Set pin value to 1.
  \param[in]   pin      Pin index
  \param[in]   value    Value of the pin.
  \return      none

  \fn          void ReadPin (uint32_t pin)
  \brief       Read value of the pin.
  \param[in]   pin      Pin index
  \return      Current value of the pin.
  
  
  \fn          void ARM_GPIO_SignalEvent (uint32_t event)
  \brief       Signal GPIO Events.
  \param[in]   event  \ref GPIO_events notification mask
  \return      none
*/

typedef void (*ARM_GPIO_SignalEvent_t) (uint32_t event);  ///< Pointer to \ref ARM_GPIO_SignalEvent : Signal GPIO Event.


/**
\brief GPIO Device Driver Capabilities.
*/
typedef struct _ARM_GPIO_CAPABILITIES {
  uint32_t rising_edge        : 1;      ///< supports rising edge interrupts
  uint32_t falling_edge       : 1;      ///< supports falling edge interrupts
  uint32_t both_edges         : 1;      ///< supports both edges interrupts
  uint32_t level_1            : 1;      ///< supports level-1 sensitive interrupts
  uint32_t level_0            : 1;      ///< supports level-0 sensitive interrupts
  
  uint32_t pull_up            : 1;      ///< supports pull-up register on a pin
  uint32_t pull_down          : 1;      ///< supports pull-down register on a pin
  uint32_t speed              : 1;      ///< supports configuration of speed
  uint32_t open_drain         : 1;      ///< supports open-drain on a pin
  uint32_t drive_strength     : 1;      ///< supports configuration of drive strength on a pin
  
  uint32_t reserved           : 22;     ///< Reserved (must be zero)
} ARM_GPIO_CAPABILITIES;


/**
\brief Access structure of the GPIO Driver.
*/
typedef struct _ARM_DRIVER_GPIO {
  ARM_DRIVER_VERSION     (*GetVersion)      (void);                              ///< Pointer to \ref ARM_GPIO_GetVersion : Get driver version.
  ARM_GPIO_CAPABILITIES  (*GetCapabilities) (void);                              ///< Pointer to \ref ARM_GPIO_GetCapabilities : Get driver capabilities.
  int32_t                (*Initialize)      (ARM_GPIO_SignalEvent_t cb_event);   ///< Pointer to \ref ARM_GPIO_Initialize : Initialize GPIO Interface.
  int32_t                (*Uninitialize)    (void);                              ///< Pointer to \ref ARM_GPIO_Uninitialize : De-initialize GPIO Interface.
  int32_t                (*PowerControl)    (ARM_POWER_STATE state);             ///< Pointer to \ref ARM_GPIO_PowerControl : Control GPIO Interface Power.
  int32_t                (*Control)         (uint32_t control, uint32_t arg);    ///< Pointer to \ref ARM_GPIO_Control : Control GPIO Interface.
  ARM_GPIO_STATUS        (*GetStatus)       (void);                              ///< Pointer to \ref ARM_GPIO_GetStatus : Get GPIO status.
  
  void                   (*SetPort)         (uint32_t mask);                     ///< Pointer to \ref ARM_GPIO_SetPort : Set port pins values to 1s.
  void                   (*ClearPort)       (uint32_t mask);                     ///< Pointer to \ref ARM_GPIO_ClearPort : Clear port pins values to 0s.
  void                   (*TogglePort)      (uint32_t mask);                     ///< Pointer to \ref ARM_GPIO_TogglePort : Toggle port pins values.
  void                   (*WritePort)       (uint32_t values);                   ///< Pointer to \ref ARM_GPIO_WritePort : Write port pins values.
  uint32_t               (*ReadPort)        (void);                              ///< Pointer to \ref ARM_GPIO_ReadPort : Read port pins values.
  uint32_t               (*GetPortEvents)   (void);                              ///< Pointer to \ref ARM_GPIO_GetPortEvents : Get port events mask.
  void                   (*ClearPortEvents) (uint32_t mask);                     ///< Pointer to \ref ARM_GPIO_ClearPortEvents : Clear port events mask.
  
  int32_t                (*ControlPin)      (uint32_t pin, 
                                             uint32_t control, uint32_t arg);    ///< Pointer to \ref ARM_GPIO_ControlPin : Control single GPIO Pin.
  void                   (*SetPin)          (uint32_t pin);                      ///< Pointer to \ref ARM_GPIO_SetPin : Set pin value to 1.
  void                   (*ClearPin)        (uint32_t pin);                      ///< Pointer to \ref ARM_GPIO_ClearPin : Clear pin value to 0.
  void                   (*TogglePin)       (uint32_t pin);                      ///< Pointer to \ref ARM_GPIO_TogglePin : Toggle pin value.
  void                   (*WritePin)        (uint32_t pin, uint32_t value);      ///< Pointer to \ref ARM_GPIO_WritePin : Write pin value.
  uint32_t               (*ReadPin)         (uint32_t pin);                      ///< Pointer to \ref ARM_GPIO_ReadPin : Read pin value.

} const ARM_DRIVER_GPIO;

#ifdef  __cplusplus
}
#endif

#endif /* DRIVER_GPIO_H_ */
