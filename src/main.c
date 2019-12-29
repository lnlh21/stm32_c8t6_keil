#include "ccb_def.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "bsp.h"
#include "hw_config.h"

__IO uint8_t PrevXferComplete = 1;

int main(void)
{   
    ULONG i;
	uint8_t v[] = { 3, 5, 4, 2};
	uint32_t count = 0; 
    
    BSP_ApiDeLayMs(500);
	BSP_ApiInit();
    USB_Interrupts_Config();

	CMD_Init();

	while(1) {
		BSP_GpioSet(BSP_GPIO_LED_0, 1);
		BSP_ApiDeLayMs(100);
		BSP_GpioSet(BSP_GPIO_LED_0, 0);
		BSP_ApiDeLayMs(100);
	}
}


