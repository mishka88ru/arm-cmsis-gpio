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

#include <MK66F18.h>
#include <intrinsics.h>

#define ARM_GPIO_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(0, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_GPIO_API_VERSION,
    ARM_GPIO_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_GPIO_CAPABILITIES DriverCapabilities = {
    1, /* supports rising edge interrupts */
    1, /* supports falling edge interrupts */
    1, /* supports both edges interrupts */
    1, /* supports level-1 sensitive interrupts */
    1  /* supports level-0 sensitive interrupts */
};

//
//   Functions
//

////////////////////////////////////////////////////////////////////////////////
ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_Shared    (void) { return DriverVersion; }

ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_0    (void) { return ARM_GPIO_GetVersion_Shared(); }
ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_1    (void) { return ARM_GPIO_GetVersion_Shared(); }
ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_2    (void) { return ARM_GPIO_GetVersion_Shared(); }
ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_3    (void) { return ARM_GPIO_GetVersion_Shared(); }
ARM_DRIVER_VERSION 	ARM_GPIO_GetVersion_4    (void) { return ARM_GPIO_GetVersion_Shared(); }

////////////////////////////////////////////////////////////////////////////////
ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_Shared	(void) { return DriverCapabilities; }

ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_0    (void) { return ARM_GPIO_GetCapabilities_Shared(); }
ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_1    (void) { return ARM_GPIO_GetCapabilities_Shared(); }
ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_2    (void) { return ARM_GPIO_GetCapabilities_Shared(); }
ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_3    (void) { return ARM_GPIO_GetCapabilities_Shared(); }
ARM_GPIO_CAPABILITIES 	ARM_GPIO_GetCapabilities_4    (void) { return ARM_GPIO_GetCapabilities_Shared(); }

////////////////////////////////////////////////////////////////////////////////

typedef void (*ISR)();

// placed in ROM
typedef struct
{
    const PORT_MemMapPtr    port;
    const GPIO_MemMapPtr    gpio;
    const uint32_t          irq_vector;
    const ISR               irq_handler;
} ARM_GPIO_CONFIG;

// placed in RAM
typedef struct
{
    ARM_GPIO_SignalEvent_t     signal;
    ARM_GPIO_STATUS            status;
} ARM_GPIO_STATE;

// Will be called from IRQ handler.
void gpio_shared_handler(const ARM_GPIO_CONFIG* cfg, ARM_GPIO_SignalEvent_t signal)
{
    // Clear interrupts.
    const uint32_t isfr = cfg->port->ISFR;
    cfg->port->ISFR = isfr;
    
    // Call user handler, if it is configured.
    // Argument = bitmask of active interrupts.
    if (signal)
        (*signal)(isfr);
}

////////////////////////////////////////////////////////////////////////////////

void gpio_a_handler();
void gpio_b_handler();
void gpio_c_handler();
void gpio_d_handler();
void gpio_e_handler();


const ARM_GPIO_CONFIG gpio_a = {
    .port           = PORTA_BASE_PTR,
    .gpio           = PTA_BASE_PTR,
    .irq_vector     = INT_PORTE,
    .irq_handler    = gpio_a_handler
};

const ARM_GPIO_CONFIG gpio_b = {
    .port           = PORTB_BASE_PTR,
    .gpio           = PTB_BASE_PTR,
    .irq_vector     = INT_PORTB,
    .irq_handler    = gpio_b_handler
};

const ARM_GPIO_CONFIG gpio_c = {
    .port           = PORTC_BASE_PTR,
    .gpio           = PTC_BASE_PTR,
    .irq_vector     = INT_PORTC,
    .irq_handler    = gpio_c_handler
};

const ARM_GPIO_CONFIG gpio_d = {
    .port           = PORTD_BASE_PTR,
    .gpio           = PTD_BASE_PTR,
    .irq_vector     = INT_PORTD,
    .irq_handler    = gpio_d_handler
};

const ARM_GPIO_CONFIG gpio_e = {
    .port           = PORTE_BASE_PTR,
    .gpio           = PTE_BASE_PTR,
    .irq_vector     = INT_PORTE,
    .irq_handler    = gpio_e_handler
};


ARM_GPIO_STATE state_a = { .signal = 0, .status = 0 };
ARM_GPIO_STATE state_b = { .signal = 0, .status = 0 };
ARM_GPIO_STATE state_c = { .signal = 0, .status = 0 };
ARM_GPIO_STATE state_d = { .signal = 0, .status = 0 };
ARM_GPIO_STATE state_e = { .signal = 0, .status = 0 };

void gpio_a_handler() { gpio_shared_handler(&gpio_a, state_a.signal); }
void gpio_b_handler() { gpio_shared_handler(&gpio_b, state_b.signal); }
void gpio_c_handler() { gpio_shared_handler(&gpio_c, state_c.signal); }
void gpio_d_handler() { gpio_shared_handler(&gpio_d, state_d.signal); }
void gpio_e_handler() { gpio_shared_handler(&gpio_e, state_e.signal); }

////////////////////////////////////////////////////////////////////////////////
int32_t ARM_GPIO_Initialize_Shared(const ARM_GPIO_CONFIG* port)
{
    ((uint32_t*)(SCB_VTOR))[port->irq_vector] = (uint32_t)port->irq_handler;
    __DSB();
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_Initialize_0	(ARM_GPIO_SignalEvent_t cb_event) { return state_a.signal = cb_event, ARM_GPIO_Initialize_Shared(&gpio_a); }
int32_t ARM_GPIO_Initialize_1	(ARM_GPIO_SignalEvent_t cb_event) { return state_b.signal = cb_event, ARM_GPIO_Initialize_Shared(&gpio_b); }
int32_t ARM_GPIO_Initialize_2	(ARM_GPIO_SignalEvent_t cb_event) { return state_c.signal = cb_event, ARM_GPIO_Initialize_Shared(&gpio_c); }
int32_t ARM_GPIO_Initialize_3	(ARM_GPIO_SignalEvent_t cb_event) { return state_d.signal = cb_event, ARM_GPIO_Initialize_Shared(&gpio_d); }
int32_t ARM_GPIO_Initialize_4	(ARM_GPIO_SignalEvent_t cb_event) { return state_e.signal = cb_event, ARM_GPIO_Initialize_Shared(&gpio_e); }

////////////////////////////////////////////////////////////////////////////////
int32_t ARM_GPIO_Uninitialize_Shared(const ARM_GPIO_CONFIG* cfg)
{
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_Uninitialize_0(void) { return ARM_GPIO_Uninitialize_Shared(&gpio_a); }
int32_t ARM_GPIO_Uninitialize_1(void) { return ARM_GPIO_Uninitialize_Shared(&gpio_b); }
int32_t ARM_GPIO_Uninitialize_2(void) { return ARM_GPIO_Uninitialize_Shared(&gpio_c); }
int32_t ARM_GPIO_Uninitialize_3(void) { return ARM_GPIO_Uninitialize_Shared(&gpio_d); }
int32_t ARM_GPIO_Uninitialize_4(void) { return ARM_GPIO_Uninitialize_Shared(&gpio_e); }

////////////////////////////////////////////////////////////////////////////////
int32_t ARM_GPIO_PowerControl_Shared(ARM_POWER_STATE state, const ARM_GPIO_CONFIG* cfg, uint32_t sim_scgc5_mask)
{
    switch (state)
    {
	// Most often ports should be always enabled because of other peripherals.
	case ARM_POWER_OFF : /*SIM_SCGC5 &= ~sim_scgc5_mask;*/ break;
	case ARM_POWER_FULL: /*SIM_SCGC5 |=  sim_scgc5_mask;*/ break;
	
	// POWER_LOW is unsupported
	default: return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_PowerControl_0(ARM_POWER_STATE state) { return ARM_GPIO_PowerControl_Shared(state, &gpio_a, SIM_SCGC5_PORTA_MASK); }
int32_t ARM_GPIO_PowerControl_1(ARM_POWER_STATE state) { return ARM_GPIO_PowerControl_Shared(state, &gpio_b, SIM_SCGC5_PORTB_MASK); }
int32_t ARM_GPIO_PowerControl_2(ARM_POWER_STATE state) { return ARM_GPIO_PowerControl_Shared(state, &gpio_c, SIM_SCGC5_PORTC_MASK); }
int32_t ARM_GPIO_PowerControl_3(ARM_POWER_STATE state) { return ARM_GPIO_PowerControl_Shared(state, &gpio_d, SIM_SCGC5_PORTD_MASK); }
int32_t ARM_GPIO_PowerControl_4(ARM_POWER_STATE state) { return ARM_GPIO_PowerControl_Shared(state, &gpio_e, SIM_SCGC5_PORTE_MASK); }

////////////////////////////////////////////////////////////////////////////////
int32_t ARM_GPIO_Control_Shared(uint32_t control, uint32_t arg, const ARM_GPIO_CONFIG* cfg)
{
    switch (control)
    {
        default: return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
//    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_Control_0(uint32_t control, uint32_t arg) { return ARM_GPIO_Control_Shared(control, arg, &gpio_a); }
int32_t ARM_GPIO_Control_1(uint32_t control, uint32_t arg) { return ARM_GPIO_Control_Shared(control, arg, &gpio_b); }
int32_t ARM_GPIO_Control_2(uint32_t control, uint32_t arg) { return ARM_GPIO_Control_Shared(control, arg, &gpio_c); }
int32_t ARM_GPIO_Control_3(uint32_t control, uint32_t arg) { return ARM_GPIO_Control_Shared(control, arg, &gpio_d); }
int32_t ARM_GPIO_Control_4(uint32_t control, uint32_t arg) { return ARM_GPIO_Control_Shared(control, arg, &gpio_e); }

////////////////////////////////////////////////////////////////////////////////
ARM_GPIO_STATUS ARM_GPIO_GetStatus_0(void) { return state_a.status; }
ARM_GPIO_STATUS ARM_GPIO_GetStatus_1(void) { return state_b.status; }
ARM_GPIO_STATUS ARM_GPIO_GetStatus_2(void) { return state_c.status; }
ARM_GPIO_STATUS ARM_GPIO_GetStatus_3(void) { return state_d.status; }
ARM_GPIO_STATUS ARM_GPIO_GetStatus_4(void) { return state_e.status; }

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_SetPort_0(uint32_t mask) { gpio_a.gpio->PSOR = mask; }
void ARM_GPIO_SetPort_1(uint32_t mask) { gpio_b.gpio->PSOR = mask; }
void ARM_GPIO_SetPort_2(uint32_t mask) { gpio_c.gpio->PSOR = mask; }
void ARM_GPIO_SetPort_3(uint32_t mask) { gpio_d.gpio->PSOR = mask; }
void ARM_GPIO_SetPort_4(uint32_t mask) { gpio_e.gpio->PSOR = mask; }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_ClearPort_0(uint32_t mask) { gpio_a.gpio->PCOR = mask; }
void ARM_GPIO_ClearPort_1(uint32_t mask) { gpio_b.gpio->PCOR = mask; }
void ARM_GPIO_ClearPort_2(uint32_t mask) { gpio_c.gpio->PCOR = mask; }
void ARM_GPIO_ClearPort_3(uint32_t mask) { gpio_d.gpio->PCOR = mask; }
void ARM_GPIO_ClearPort_4(uint32_t mask) { gpio_e.gpio->PCOR = mask; }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_TogglePort_0(uint32_t mask) { gpio_a.gpio->PTOR = mask; }
void ARM_GPIO_TogglePort_1(uint32_t mask) { gpio_b.gpio->PTOR = mask; }
void ARM_GPIO_TogglePort_2(uint32_t mask) { gpio_c.gpio->PTOR = mask; }
void ARM_GPIO_TogglePort_3(uint32_t mask) { gpio_d.gpio->PTOR = mask; }
void ARM_GPIO_TogglePort_4(uint32_t mask) { gpio_e.gpio->PTOR = mask; }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_WritePort_0(uint32_t values) { gpio_a.gpio->PDOR = values; }
void ARM_GPIO_WritePort_1(uint32_t values) { gpio_b.gpio->PDOR = values; }
void ARM_GPIO_WritePort_2(uint32_t values) { gpio_c.gpio->PDOR = values; }
void ARM_GPIO_WritePort_3(uint32_t values) { gpio_d.gpio->PDOR = values; }
void ARM_GPIO_WritePort_4(uint32_t values) { gpio_e.gpio->PDOR = values; }
////////////////////////////////////////////////////////////////////////////////
uint32_t ARM_GPIO_ReadPort_0() { return gpio_a.gpio->PDIR; }
uint32_t ARM_GPIO_ReadPort_1() { return gpio_b.gpio->PDIR; }
uint32_t ARM_GPIO_ReadPort_2() { return gpio_c.gpio->PDIR; }
uint32_t ARM_GPIO_ReadPort_3() { return gpio_d.gpio->PDIR; }
uint32_t ARM_GPIO_ReadPort_4() { return gpio_e.gpio->PDIR; }
////////////////////////////////////////////////////////////////////////////////
uint32_t ARM_GPIO_GetPortEvents_0() { return gpio_a.port->ISFR; }
uint32_t ARM_GPIO_GetPortEvents_1() { return gpio_b.port->ISFR; }
uint32_t ARM_GPIO_GetPortEvents_2() { return gpio_c.port->ISFR; }
uint32_t ARM_GPIO_GetPortEvents_3() { return gpio_d.port->ISFR; }
uint32_t ARM_GPIO_GetPortEvents_4() { return gpio_e.port->ISFR; }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_ClearPortEvents_0(uint32_t mask) { gpio_a.port->ISFR = mask; }
void ARM_GPIO_ClearPortEvents_1(uint32_t mask) { gpio_b.port->ISFR = mask; }
void ARM_GPIO_ClearPortEvents_2(uint32_t mask) { gpio_c.port->ISFR = mask; }
void ARM_GPIO_ClearPortEvents_3(uint32_t mask) { gpio_d.port->ISFR = mask; }
void ARM_GPIO_ClearPortEvents_4(uint32_t mask) { gpio_e.port->ISFR = mask; }
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int32_t ARM_GPIO_ControlPin_Config(const ARM_GPIO_CONFIG* cfg, uint32_t pin, uint32_t arg)
{
    uint32_t pcr = 0;
    
    if (arg & ARM_GPIO_PIN_CFG_ENABLED)
        pcr |= PORT_PCR_MUX(1);
	
    uint32_t tmp = (arg & ARM_GPIO_PIN_CFG_IRQ_Msk) >> ARM_GPIO_PIN_CFG_IRQ_Pos;
    switch (tmp)
    {
        case ARM_GPIO_PIN_IRQ_NONE:        tmp = 0x0; break;
        case ARM_GPIO_PIN_IRQ_RISING:      tmp = 0x9; break;
        case ARM_GPIO_PIN_IRQ_FALLING:     tmp = 0xA; break;
        case ARM_GPIO_PIN_IRQ_BOTH:        tmp = 0xB; break;
        case ARM_GPIO_PIN_IRQ_LEVEL_HIGH:  tmp = 0xC; break;
        case ARM_GPIO_PIN_IRQ_LEVEL_LOW:   tmp = 0x8; break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    pcr |= PORT_PCR_IRQC(tmp);
    
    tmp = (arg & ARM_GPIO_PIN_CFG_PULL_Msk) >> ARM_GPIO_PIN_CFG_PULL_Pos;
    switch (tmp)
    {
        case ARM_GPIO_PIN_PULL_NONE: break;
        case ARM_GPIO_PIN_PULL_UP:   pcr |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK); break;
        case ARM_GPIO_PIN_PULL_DOWN: pcr |= PORT_PCR_PE_MASK; break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    
    tmp = (arg & ARM_GPIO_PIN_CFG_SPEED_Msk) >> ARM_GPIO_PIN_CFG_SPEED_Pos;
    switch (tmp)
    {
        case ARM_GPIO_PIN_SPEED_LOW:
        case ARM_GPIO_PIN_SPEED_MEDIUM:
            pcr |= PORT_PCR_SRE_MASK;
            break;
        
        case ARM_GPIO_PIN_SPEED_HIGH:
        case ARM_GPIO_PIN_SPEED_VERY_HIGH:
            break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    
    if (arg & ARM_GPIO_PIN_CFG_OPEN_DRAIN)
        pcr |= PORT_PCR_ODE_MASK;
    
    if (arg & ARM_GPIO_PIN_CFG_DRIVE_STRENGTH)
        pcr |= PORT_PCR_DSE_MASK;
    
    cfg->port->PCR[pin] = pcr;
    
    // Set direction.
    if (arg & ARM_GPIO_PIN_CFG_OUTPUT)
        cfg->gpio->PDDR |=  (1u << pin);
    else
        cfg->gpio->PDDR &= ~(1u << pin);
    
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_ControlPin_State(volatile uint32_t* pcr, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_STATE_DISABLE: *pcr &= ~PORT_PCR_MUX_MASK;                           return ARM_DRIVER_OK;
        case ARM_GPIO_PIN_STATE_ENABLE:  *pcr = (*pcr & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(1); return ARM_DRIVER_OK;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
}

int32_t ARM_GPIO_ControlPin_Direction(const ARM_GPIO_CONFIG* cfg, uint32_t pin, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_DIRECTION_INPUT:  cfg->gpio->PDDR &= ~(1u << pin); break;
        case ARM_GPIO_PIN_DIRECTION_OUTPUT: cfg->gpio->PDDR |=  (1u << pin); break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_ControlPin_IRQ(volatile uint32_t* pcr, uint32_t arg)
{
    uint32_t irq;
    switch (arg)
    {
        case ARM_GPIO_PIN_IRQ_NONE:        irq = 0x0; break;
        case ARM_GPIO_PIN_IRQ_RISING:      irq = 0x9; break;
        case ARM_GPIO_PIN_IRQ_FALLING:     irq = 0xA; break;
        case ARM_GPIO_PIN_IRQ_BOTH:        irq = 0xB; break;
        case ARM_GPIO_PIN_IRQ_LEVEL_HIGH:  irq = 0xC; break;
        case ARM_GPIO_PIN_IRQ_LEVEL_LOW:   irq = 0x8; break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    
    *pcr = (*pcr & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(irq);
    return ARM_DRIVER_OK;
}


int32_t ARM_GPIO_ControlPin_Pull(volatile uint32_t* pcr, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_PULL_NONE: *pcr &= ~PORT_PCR_PE_MASK;                            break;
        case ARM_GPIO_PIN_PULL_UP:   *pcr |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);        break;
        case ARM_GPIO_PIN_PULL_DOWN: *pcr = (*pcr & ~PORT_PCR_PS_MASK) | PORT_PCR_PE_MASK; break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_ControlPin_Speed(volatile uint32_t* pcr, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_SPEED_LOW:
        case ARM_GPIO_PIN_SPEED_MEDIUM:
            *pcr |= PORT_PCR_SRE_MASK;
            break;
        
        case ARM_GPIO_PIN_SPEED_HIGH:
        case ARM_GPIO_PIN_SPEED_VERY_HIGH:
            *pcr &= ~PORT_PCR_SRE_MASK;
            break;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
    return ARM_DRIVER_OK;
}

int32_t ARM_GPIO_ControlPin_OpenDrain(volatile uint32_t* pcr, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_OPEN_DRAIN_DISABLE: *pcr &= ~PORT_PCR_ODE_MASK; return ARM_DRIVER_OK;
        case ARM_GPIO_PIN_OPEN_DRAIN_ENABLE:  *pcr |=  PORT_PCR_ODE_MASK; return ARM_DRIVER_OK;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
}

int32_t ARM_GPIO_ControlPin_DriveStrength(volatile uint32_t* pcr, uint32_t arg)
{
    switch (arg)
    {
        case ARM_GPIO_PIN_DRIVE_STRENGTH_LOW:  *pcr &= ~PORT_PCR_DSE_MASK; return ARM_DRIVER_OK;
        case ARM_GPIO_PIN_DRIVE_STRENGTH_HIGH: *pcr |=  PORT_PCR_DSE_MASK; return ARM_DRIVER_OK;
        
        default: return ARM_DRIVER_ERROR_PARAMETER;
    }
}


int32_t ARM_GPIO_ControlPin_Shared(uint32_t pin, uint32_t control, uint32_t arg, const ARM_GPIO_CONFIG* cfg)
{
    if (pin >= 32)
        return ARM_DRIVER_ERROR_PARAMETER;
	
    switch (control)
    {
        case ARM_GPIO_PIN_CFG:            return ARM_GPIO_ControlPin_Config        (cfg, pin, arg);
        case ARM_GPIO_PIN_STATE:          return ARM_GPIO_ControlPin_State         (&cfg->port->PCR[pin], arg);
        case ARM_GPIO_PIN_DIRECTION:      return ARM_GPIO_ControlPin_Direction     (cfg, pin, arg);
        case ARM_GPIO_PIN_IRQ:            return ARM_GPIO_ControlPin_IRQ           (&cfg->port->PCR[pin], arg);
        case ARM_GPIO_PIN_PULL:           return ARM_GPIO_ControlPin_Pull          (&cfg->port->PCR[pin], arg);
        case ARM_GPIO_PIN_SPEED:          return ARM_GPIO_ControlPin_Speed         (&cfg->port->PCR[pin], arg);
        case ARM_GPIO_PIN_OPEN_DRAIN:     return ARM_GPIO_ControlPin_OpenDrain     (&cfg->port->PCR[pin], arg);
        case ARM_GPIO_PIN_DRIVE_STRENGTH: return ARM_GPIO_ControlPin_DriveStrength (&cfg->port->PCR[pin], arg);
        
        default: return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

int32_t ARM_GPIO_ControlPin_0(uint32_t pin, uint32_t control, uint32_t arg) { return ARM_GPIO_ControlPin_Shared(pin, control, arg, &gpio_a); }
int32_t ARM_GPIO_ControlPin_1(uint32_t pin, uint32_t control, uint32_t arg) { return ARM_GPIO_ControlPin_Shared(pin, control, arg, &gpio_b); }
int32_t ARM_GPIO_ControlPin_2(uint32_t pin, uint32_t control, uint32_t arg) { return ARM_GPIO_ControlPin_Shared(pin, control, arg, &gpio_c); }
int32_t ARM_GPIO_ControlPin_3(uint32_t pin, uint32_t control, uint32_t arg) { return ARM_GPIO_ControlPin_Shared(pin, control, arg, &gpio_d); }
int32_t ARM_GPIO_ControlPin_4(uint32_t pin, uint32_t control, uint32_t arg) { return ARM_GPIO_ControlPin_Shared(pin, control, arg, &gpio_e); }

////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_SetPin_0(uint32_t pin) { gpio_a.gpio->PSOR = (1u << pin); }
void ARM_GPIO_SetPin_1(uint32_t pin) { gpio_b.gpio->PSOR = (1u << pin); }
void ARM_GPIO_SetPin_2(uint32_t pin) { gpio_c.gpio->PSOR = (1u << pin); }
void ARM_GPIO_SetPin_3(uint32_t pin) { gpio_d.gpio->PSOR = (1u << pin); }
void ARM_GPIO_SetPin_4(uint32_t pin) { gpio_e.gpio->PSOR = (1u << pin); }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_ClearPin_0(uint32_t pin) { gpio_a.gpio->PCOR = (1u << pin); }
void ARM_GPIO_ClearPin_1(uint32_t pin) { gpio_b.gpio->PCOR = (1u << pin); }
void ARM_GPIO_ClearPin_2(uint32_t pin) { gpio_c.gpio->PCOR = (1u << pin); }
void ARM_GPIO_ClearPin_3(uint32_t pin) { gpio_d.gpio->PCOR = (1u << pin); }
void ARM_GPIO_ClearPin_4(uint32_t pin) { gpio_e.gpio->PCOR = (1u << pin); }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_TogglePin_0(uint32_t pin) { gpio_a.gpio->PTOR = (1u << pin); }
void ARM_GPIO_TogglePin_1(uint32_t pin) { gpio_b.gpio->PTOR = (1u << pin); }
void ARM_GPIO_TogglePin_2(uint32_t pin) { gpio_c.gpio->PTOR = (1u << pin); }
void ARM_GPIO_TogglePin_3(uint32_t pin) { gpio_d.gpio->PTOR = (1u << pin); }
void ARM_GPIO_TogglePin_4(uint32_t pin) { gpio_e.gpio->PTOR = (1u << pin); }
////////////////////////////////////////////////////////////////////////////////
void ARM_GPIO_WritePin_0(uint32_t pin, uint32_t value) { *(value ? &gpio_a.gpio->PSOR : &gpio_a.gpio->PCOR) = (1u << pin); }
void ARM_GPIO_WritePin_1(uint32_t pin, uint32_t value) { *(value ? &gpio_b.gpio->PSOR : &gpio_b.gpio->PCOR) = (1u << pin); }
void ARM_GPIO_WritePin_2(uint32_t pin, uint32_t value) { *(value ? &gpio_c.gpio->PSOR : &gpio_c.gpio->PCOR) = (1u << pin); }
void ARM_GPIO_WritePin_3(uint32_t pin, uint32_t value) { *(value ? &gpio_d.gpio->PSOR : &gpio_d.gpio->PCOR) = (1u << pin); }
void ARM_GPIO_WritePin_4(uint32_t pin, uint32_t value) { *(value ? &gpio_e.gpio->PSOR : &gpio_e.gpio->PCOR) = (1u << pin); }
////////////////////////////////////////////////////////////////////////////////
uint32_t ARM_GPIO_ReadPin_0(uint32_t pin) { return (gpio_a.gpio->PDIR & (1u << pin)) ? 1u : 0; };
uint32_t ARM_GPIO_ReadPin_1(uint32_t pin) { return (gpio_b.gpio->PDIR & (1u << pin)) ? 1u : 0; };
uint32_t ARM_GPIO_ReadPin_2(uint32_t pin) { return (gpio_c.gpio->PDIR & (1u << pin)) ? 1u : 0; };
uint32_t ARM_GPIO_ReadPin_3(uint32_t pin) { return (gpio_d.gpio->PDIR & (1u << pin)) ? 1u : 0; };
uint32_t ARM_GPIO_ReadPin_4(uint32_t pin) { return (gpio_e.gpio->PDIR & (1u << pin)) ? 1u : 0; };
////////////////////////////////////////////////////////////////////////////////


// End GPIO Interface

ARM_DRIVER_GPIO Driver_GPIO0 = {
    ARM_GPIO_GetVersion_0,
    ARM_GPIO_GetCapabilities_0,
    
    ARM_GPIO_Initialize_0,
    ARM_GPIO_Uninitialize_0,
    ARM_GPIO_PowerControl_0,
    ARM_GPIO_Control_0,
    ARM_GPIO_GetStatus_0,
    
    ARM_GPIO_SetPort_0,
    ARM_GPIO_ClearPort_0,
    ARM_GPIO_TogglePort_0,
    ARM_GPIO_WritePort_0,
    ARM_GPIO_ReadPort_0,
    ARM_GPIO_GetPortEvents_0,
    ARM_GPIO_ClearPortEvents_0,
    
    ARM_GPIO_ControlPin_0,
    ARM_GPIO_SetPin_0,
    ARM_GPIO_ClearPin_0,
    ARM_GPIO_TogglePin_0,
    ARM_GPIO_WritePin_0,
    ARM_GPIO_ReadPin_0
};

ARM_DRIVER_GPIO Driver_GPIO1 = {
    ARM_GPIO_GetVersion_1,
    ARM_GPIO_GetCapabilities_1,
    
    ARM_GPIO_Initialize_1,
    ARM_GPIO_Uninitialize_1,
    ARM_GPIO_PowerControl_1,
    ARM_GPIO_Control_1,
    ARM_GPIO_GetStatus_1,
    
    ARM_GPIO_SetPort_1,
    ARM_GPIO_ClearPort_1,
    ARM_GPIO_TogglePort_1,
    ARM_GPIO_WritePort_1,
    ARM_GPIO_ReadPort_1,
    ARM_GPIO_GetPortEvents_1,
    ARM_GPIO_ClearPortEvents_1,
    
    ARM_GPIO_ControlPin_1,
    ARM_GPIO_SetPin_1,
    ARM_GPIO_ClearPin_1,
    ARM_GPIO_TogglePin_1,
    ARM_GPIO_WritePin_1,
    ARM_GPIO_ReadPin_1
};

ARM_DRIVER_GPIO Driver_GPIO2 = {
    ARM_GPIO_GetVersion_2,
    ARM_GPIO_GetCapabilities_2,
    
    ARM_GPIO_Initialize_2,
    ARM_GPIO_Uninitialize_2,
    ARM_GPIO_PowerControl_2,
    ARM_GPIO_Control_2,
    ARM_GPIO_GetStatus_2,
    
    ARM_GPIO_SetPort_2,
    ARM_GPIO_ClearPort_2,
    ARM_GPIO_TogglePort_2,
    ARM_GPIO_WritePort_2,
    ARM_GPIO_ReadPort_2,
    ARM_GPIO_GetPortEvents_2,
    ARM_GPIO_ClearPortEvents_2,
    
    ARM_GPIO_ControlPin_2,
    ARM_GPIO_SetPin_2,
    ARM_GPIO_ClearPin_2,
    ARM_GPIO_TogglePin_2,
    ARM_GPIO_WritePin_2,
    ARM_GPIO_ReadPin_2
};

ARM_DRIVER_GPIO Driver_GPIO3 = {
    ARM_GPIO_GetVersion_3,
    ARM_GPIO_GetCapabilities_3,
    
    ARM_GPIO_Initialize_3,
    ARM_GPIO_Uninitialize_3,
    ARM_GPIO_PowerControl_3,
    ARM_GPIO_Control_3,
    ARM_GPIO_GetStatus_3,
    
    ARM_GPIO_SetPort_3,
    ARM_GPIO_ClearPort_3,
    ARM_GPIO_TogglePort_3,
    ARM_GPIO_WritePort_3,
    ARM_GPIO_ReadPort_3,
    ARM_GPIO_GetPortEvents_3,
    ARM_GPIO_ClearPortEvents_3,
    
    ARM_GPIO_ControlPin_3,
    ARM_GPIO_SetPin_3,
    ARM_GPIO_ClearPin_3,
    ARM_GPIO_TogglePin_3,
    ARM_GPIO_WritePin_3,
    ARM_GPIO_ReadPin_3
};

ARM_DRIVER_GPIO Driver_GPIO4 = {
    ARM_GPIO_GetVersion_4,
    ARM_GPIO_GetCapabilities_4,
    
    ARM_GPIO_Initialize_4,
    ARM_GPIO_Uninitialize_4,
    ARM_GPIO_PowerControl_4,
    ARM_GPIO_Control_4,
    ARM_GPIO_GetStatus_4,
    
    ARM_GPIO_SetPort_4,
    ARM_GPIO_ClearPort_4,
    ARM_GPIO_TogglePort_4,
    ARM_GPIO_WritePort_4,
    ARM_GPIO_ReadPort_4,
    ARM_GPIO_GetPortEvents_4,
    ARM_GPIO_ClearPortEvents_4,
    
    ARM_GPIO_ControlPin_4,
    ARM_GPIO_SetPin_4,
    ARM_GPIO_ClearPin_4,
    ARM_GPIO_TogglePin_4,
    ARM_GPIO_WritePin_4,
    ARM_GPIO_ReadPin_4
};
