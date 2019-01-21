
#include <stdio.h>
#include <MK66F18.h>
#include <intrinsics.h>

#include "Driver_GPIO.h"

extern ARM_DRIVER_GPIO Driver_GPIO0;	// PORT A
extern ARM_DRIVER_GPIO Driver_GPIO1;	// PORT B
extern ARM_DRIVER_GPIO Driver_GPIO2;	// PORT C
extern ARM_DRIVER_GPIO Driver_GPIO3;	// PORT D
extern ARM_DRIVER_GPIO Driver_GPIO4;	// PORT E

// Board:
// pin B5 - input of 1PPS (1 Hz; 200 ms high, 800 ms low)
// pins E10, E11 - outputs

ARM_DRIVER_GPIO* port_in  = &Driver_GPIO1;
ARM_DRIVER_GPIO* port_out = &Driver_GPIO4;

#define PIN_INPUT_1 	5
#define PIN_OUTPUT_1 	10
#define PIN_OUTPUT_2	11

int pps_counter = 0;

void pps_handler()
{
	printf("1PPS %d\n", pps_counter++);
}

void port_b_callback(uint32_t event)
{
	// event = mask of pins, which interrupts are active
	if (event & (1 << 5))
		pps_handler();
}


int main()
{
	// Enable all ports: required for other peripherals, not only for GPIO.
	SIM_SCGC5 |= 
		SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
		SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
	asm("CPSIE i");
	
	ARM_DRIVER_VERSION    version;
    ARM_GPIO_CAPABILITIES drv_capabilities;
	
	version = port_out->GetVersion();
	printf("API version: %d\nDriver version: %d\n", version.api, version.drv);
	
	drv_capabilities = port_out->GetCapabilities();
	if (!drv_capabilities.both_edges)
		printf("Interrupts on both rising and falling edges are not supported!\n");
	
    port_out->Initialize(0);
    port_out->PowerControl(ARM_POWER_FULL);
	
//	port_out->Control(0, 0);
	
	port_out->ControlPin(PIN_OUTPUT_1, ARM_GPIO_PIN_DIRECTION, ARM_GPIO_PIN_DIRECTION_OUTPUT);
	port_out->ControlPin(PIN_OUTPUT_1, ARM_GPIO_PIN_STATE, ARM_GPIO_PIN_STATE_ENABLE);
	
	port_out->ControlPin(PIN_OUTPUT_2, ARM_GPIO_PIN_CFG, ARM_GPIO_PIN_CFG_ENABLED | ARM_GPIO_PIN_CFG_OUTPUT);
	
	port_out->SetPin(PIN_OUTPUT_1);
	port_out->ClearPin(PIN_OUTPUT_1);
	
	port_out->SetPort((1 << PIN_OUTPUT_1) | (1 << PIN_OUTPUT_2));
	printf("Read port: %08x\n", port_out->ReadPort());
	
	port_out->ClearPin(PIN_OUTPUT_2);
	
	port_out->TogglePort((1 << PIN_OUTPUT_1) | (1 << PIN_OUTPUT_2));
	printf("Read port: %08x\n", port_out->ReadPort());
	
	port_in->Initialize(port_b_callback);
    port_in->PowerControl(ARM_POWER_FULL);
	port_in->ControlPin(PIN_INPUT_1, ARM_GPIO_PIN_CFG, 
						ARM_GPIO_PIN_CFG_ENABLED | ARM_GPIO_PIN_CFG_IRQ_RISING);
	
	NVIC_ISER((INT_PORTB - 16) >> 5) = (1 << ((INT_PORTB - 16) & 0x1F));
	__DSB();
	__ISB();
	
	while (1);
}
