#include "ccb_def.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "bsp.h"

int main(void)
{   
    ULONG i;

    BSP_ApiDeLayMs(500);

	BSP_ApiInit();

	CMD_Init();

	while(1) {
		BSP_GpioSet(BSP_GPIO_LED_0, 1);
		BSP_ApiDeLayMs(200);
		BSP_GpioSet(BSP_GPIO_LED_0, 0);
		BSP_ApiDeLayMs(200);
	}
}


