/**
 * @file        md_gpio.c
 * @project		MD_F2833x_LIB
 *
 * @date        25 Apr 2017
 * @author      Manuel Del Basso (mainster)
 * @email       manuel.delbasso@gmail.com
 *
 * @ide         Code Composer Studio Version: 7.1.0.00015
 * @license		GNU GPL v3
 *
   @verbatim

	------------------------------------------------------------------------

	Copyright (C) 2017  Manuel Del Basso

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	------------------------------------------------------------------------

   @endverbatim
 *
 */
#include <DSP28x_Project.h>
#include "md_globals.h"
#include "md_gpio.h"
//#include "md_config.h"

/**
 * @brief      Initialize each MD_GPIO_TypeDef_t
 */
void MD_GPIO_InitAll(const MD_GPIO_TypeDef_t *IO, short size) {
	EALLOW;

	while (size--) {
		uint16_t PinNum = IO->GPIO_Pin;
		if (PinNum > MD_GPIO63) {
			if (IO->GPIO_Mode == GPIO_Mode_OUT)
				GpioCtrlRegs.GPCDIR.all |= (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);
			if (IO->GPIO_Mode == GPIO_Mode_IN)
				GpioCtrlRegs.GPCDIR.all &= ~((uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64));

			if (IO->GPIO_InitVal == GPIO_ON)
				GpioDataRegs.GPCSET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);
			else
				GpioDataRegs.GPCCLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);

			IO++;
			continue;
		}

		if ((PinNum <= MD_GPIO63) && (PinNum > MD_GPIO31)) {
			if (IO->GPIO_Mode == GPIO_Mode_IN)
				GpioCtrlRegs.GPBDIR.all &= ~((uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32));
			if (IO->GPIO_Mode == GPIO_Mode_OUT)
				GpioCtrlRegs.GPBDIR.all |= (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);

			if (IO->GPIO_InitVal == GPIO_ON)
				GpioDataRegs.GPBSET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);
			else
				GpioDataRegs.GPBCLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);

			IO++;
			continue;
		}

		if (PinNum < MD_GPIO32) {
			uint16_t PinMode = (uint16_t) IO->GPIO_Mode;

			/* GPIO pin mode */
			switch (IO->GPIO_Mode) {
				case GPIO_Mode_OUT:
					GpioCtrlRegs.GPADIR.all |=  ((uint32_t)(1 << PinNum-(uint16_t)MD_GPIO0)); break;

				case GPIO_Mode_IN:
					GpioCtrlRegs.GPADIR.all &= ~((uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0)); break;

				case GPIO_Mode_AF3:
				case GPIO_Mode_AF2:
				case GPIO_Mode_AF1:
					(PinNum < MD_GPIO16) ?
							(GpioCtrlRegs.GPAMUX1.all |= ((uint32_t)PinMode << 2*(PinNum-(uint16_t)MD_GPIO0))) :
							(GpioCtrlRegs.GPAMUX2.all |= ((uint32_t)PinMode << 2*(PinNum-(uint16_t)MD_GPIO0))); break;
				default: break;
			}

			/* GPIO initial pin level */
			(IO->GPIO_InitVal == GPIO_ON) ?
				(GpioDataRegs.GPASET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0)) :
				(GpioDataRegs.GPACLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0));
			IO++;
			continue;
		}
	}

	EDIS;
}

/**
 * @brief      Initialize GPIO pin registers.
 * @param[in]  Name         The name
 * @param[in]  Pin          The pin
 * @param[in]  Mode         The mode
 * @param[in]  DriverType   The driver type
 * @param[in]  PuPd         The pu pd
 * @param[in]  InitVal      The initialize value
 */
void MD_GPIO_Init(MD_GPIO_Name_t Name, MD_GPIO_Pin_t Pin,
                  MD_GPIO_Mode_t Mode, MD_GPIO_OType_t DriverType,
                  MD_GPIO_PuRe_t PuRe, MD_GPIO_State_t InitVal) {
}

/**
 * @brief      Set/Clear GPIO pin
 * @param[in]  name       The name
 * @param[in]  newState   New pin state.
 */
void MD_GPIO_Switch(const MD_GPIO_TypeDef_t *IO, MD_GPIO_Name_t name, MD_GPIO_State_t newState) {
	if (newState == GPIO_OFF)
		MD_GPIO_Off(IO, name);
	else
		MD_GPIO_On(IO, name);
}

/**
 * @brief      Enable GPIO
 * @param[in]  name   GPIO name enumerated by MD_GPIO_Name_t
 */
void MD_GPIO_On(const MD_GPIO_TypeDef_t *IO, MD_GPIO_Name_t name) {
	uint16_t PinNum = (uint16_t) IO[name].GPIO_Pin;

	if (PinNum > MD_GPIO63)
		GpioDataRegs.GPCSET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);
	if ((PinNum <= MD_GPIO63) && (PinNum > MD_GPIO31))
		GpioDataRegs.GPBSET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);
	if (PinNum < MD_GPIO32)
		GpioDataRegs.GPASET.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0);
}

/**
 * @brief      Disable GPIO
 * @param[in]  name   GPIO name enumerated by MD_GPIO_Name_t
 */
void MD_GPIO_Off(const MD_GPIO_TypeDef_t *IO, MD_GPIO_Name_t name) {
	uint16_t PinNum = (uint16_t) IO[name].GPIO_Pin;

	if (PinNum > MD_GPIO63)
		GpioDataRegs.GPCCLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);
	if ((PinNum <= MD_GPIO63) && (PinNum > MD_GPIO31))
		GpioDataRegs.GPBCLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);
	if (PinNum < MD_GPIO32)
		GpioDataRegs.GPACLEAR.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0);
}


/**
 * @brief      Toggle GPIO pin
 * @param[in]  name   The name
 */
void MD_GPIO_Toggle(MD_GPIO_Name_t name) {
	uint16_t PinNum = 0; //(uint16_t) MD_Gpios[name].GPIO_Pin;

	if (PinNum > MD_GPIO63)
		GpioDataRegs.GPCTOGGLE.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO64);
	if ((PinNum <= MD_GPIO63) && (PinNum > MD_GPIO31))
		GpioDataRegs.GPBTOGGLE.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO32);
	if (PinNum < MD_GPIO32)
		GpioDataRegs.GPATOGGLE.all = (uint16_t)(1 << PinNum-(uint16_t)MD_GPIO0);
}






// void MD_GPIO_Init(MD_GPIO_Name_t Name, MD_GPIO_Pin_t Pin, MD_GPIO_Mode_t Mode,
//                   MD_GPIO_OType_t DriverType, MD_GPIO_PuRe_t PuPd, MD_GPIO_State_t InitVal);
// 	/* Check input */
// 	if (GPIO_Pin == 0x00) {
// 		return;
// 	}
	
// 	/* Enable clock for GPIO */
// 	MD_GPIO_INT_EnableClock(GPIOx);
	
// 	/* Do initialization */
// 	MD_GPIO_INT_Init(GPIOx, GPIO_Pin, GPIO_Mode, GPIO_OType, GPIO_PuPd, GPIO_Speed);
// }

// void MD_GPIO_InitAlternate(GPIO_t* GPIOx, uint16_t GPIO_Pin, MD_GPIO_OType_t GPIO_OType, MD_GPIO_PuRe_t GPIO_PuPd, MD_GPIO_Speed_t GPIO_Speed, uint8_t Alternate) {
// 	uint32_t pinpos;

// 	/* Check input */
// 	if (GPIO_Pin == 0x00) {
// 		return;
// 	}
	
// 	/* Enable GPIOx clock */
// 	MD_GPIO_INT_EnableClock(GPIOx);
	
// 	/* Set alternate functions for all pins */
// 	for (pinpos = 0; pinpos < 0x10; pinpos++) {
// 		/* Check pin */
// 		if ((GPIO_Pin & (1 << pinpos)) == 0) {
// 			continue;
// 		}
		
// 		/* Set alternate function */
// 		GPIOx->AFR[pinpos >> 0x03] = (GPIOx->AFR[pinpos >> 0x03] & ~(0x0F << (4 * (pinpos & 0x07)))) | (Alternate << (4 * (pinpos & 0x07)));
// 	}
	
// 	/* Do initialization */
// 	MD_GPIO_INT_Init(GPIOx, GPIO_Pin, MD_GPIO_Mode_AF, GPIO_OType, GPIO_PuPd, GPIO_Speed);
// }

// void MD_GPIO_DeInit(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint8_t i;
// 	uint8_t ptr = MD_GPIO_GetPortSource(GPIOx);
	
// 	/* Go through all pins */
// 	for (i = 0x00; i < 0x10; i++) {
// 		/* Pin is set */
// 		if (GPIO_Pin & (1 << i)) {
// 			/* Set 11 bits combination for analog mode */
// 			GPIOx->MODER |= (0x03 << (2 * i));
			
// 			/* Pin is not used */
// 			GPIO_UsedPins[ptr] &= ~(1 << i);
// 		}
// 	}
// }

// void MD_GPIO_SetPinAsInput(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint8_t i;
// 	/* Go through all pins */
// 	for (i = 0x00; i < 0x10; i++) {
// 		/* Pin is set */
// 		if (GPIO_Pin & (1 << i)) {
// 			/* Set 00 bits combination for input */
// 			GPIOx->MODER &= ~(0x03 << (2 * i));
// 		}
// 	}
// }

// void MD_GPIO_SetPinAsOutput(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint8_t i;
// 	/* Go through all pins */
// 	for (i = 0x00; i < 0x10; i++) {
// 		/* Pin is set */
// 		if (GPIO_Pin & (1 << i)) {
// 			/* Set 01 bits combination for output */
// 			GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x01 << (2 * i));
// 		}
// 	}
// }

// void MD_GPIO_SetPinAsAnalog(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint8_t i;
// 	/* Go through all pins */
// 	for (i = 0x00; i < 0x10; i++) {
// 		/* Pin is set */
// 		if (GPIO_Pin & (1 << i)) {
// 			/* Set 11 bits combination for analog mode */
// 			GPIOx->MODER |= (0x03 << (2 * i));
// 		}
// 	}
// }

// void MD_GPIO_SetPinAsAlternate(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint8_t i;
	
// 	/* Set alternate functions for all pins */
// 	for (i = 0; i < 0x10; i++) {
// 		/* Check pin */
// 		if ((GPIO_Pin & (1 << i)) == 0) {
// 			continue;
// 		}
		
// 		/* Set alternate mode */
// 		GPIOx->MODER = (GPIOx->MODER & ~(0x03 << (2 * i))) | (0x02 << (2 * i));
// 	}
// }

// void MD_GPIO_SetPullResistor(GPIO_t* GPIOx, uint16_t GPIO_Pin, MD_GPIO_PuRe_t GPIO_PuPd) {
// 	uint8_t pinpos;
	
// 	/* Go through all pins */
// 	for (pinpos = 0; pinpos < 0x10; pinpos++) {
// 		/* Check if pin available */
// 		if ((GPIO_Pin & (1 << pinpos)) == 0) {
// 			continue;
// 		}

// 		/* Set GPIO PUPD register */
// 		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));
// 	}
// }

// void MD_GPIO_Lock(GPIO_t* GPIOx, uint16_t GPIO_Pin) {
// 	uint32_t d;
	
// 	/* Set GPIO pin with 16th bit set to 1 */
// 	d = 0x00010000 | GPIO_Pin;
	
// 	/* Write to LCKR register */
// 	GPIOx->LCKR = d;
// 	GPIOx->LCKR = GPIO_Pin;
// 	GPIOx->LCKR = d;
	
// 	/* Read twice */
// 	(void)GPIOx->LCKR;
// 	(void)GPIOx->LCKR;
// }

// uint16_t MD_GPIO_GetPinSource(uint16_t GPIO_Pin) {
// 	uint16_t pinsource = 0;
	
// 	/* Get pinsource */
// 	while (GPIO_Pin > 1) {
// 		/* Increase pinsource */
// 		pinsource++;
// 		/* Shift right */
// 		GPIO_Pin >>= 1;
// 	}
	
// 	/* Return source */
// 	return pinsource;
// }

// uint16_t MD_GPIO_GetPortSource(GPIO_t* GPIOx) {
// 	/* Get port source number */
// 	/* Offset from GPIOA                       Difference between 2 GPIO addresses */
// 	return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
// }

// /* Private functions */
// void MD_GPIO_INT_EnableClock(GPIO_t* GPIOx) {
// 	/* Set bit according to the 1 << portsourcenumber */
// 	RCC->AHB1ENR |= (1 << MD_GPIO_GetPortSource(GPIOx));
// }

// void MD_GPIO_INT_DisableClock(GPIO_t* GPIOx) {
// 	/* Clear bit according to the 1 << portsourcenumber */
// 	RCC->AHB1ENR &= ~(1 << MD_GPIO_GetPortSource(GPIOx));
// }

// void MD_GPIO_INT_Init(GPIO_t* GPIOx, uint16_t GPIO_Pin, MD_GPIO_Mode_t GPIO_Mode, MD_GPIO_OType_t GPIO_OType, MD_GPIO_PuRe_t GPIO_PuPd, MD_GPIO_Speed_t GPIO_Speed) {
// 	uint8_t pinpos;
// 	uint8_t ptr = MD_GPIO_GetPortSource(GPIOx);
	
// 	/* Go through all pins */
// 	for (pinpos = 0; pinpos < 0x10; pinpos++) {
// 		/* Check if pin available */
// 		if ((GPIO_Pin & (1 << pinpos)) == 0) {
// 			continue;
// 		}
		
// 		/* Pin is used */
// 		GPIO_UsedPins[ptr] |= 1 << pinpos;
		
// 		/* Set GPIO PUPD register */
// 		GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << (2 * pinpos))) | ((uint32_t)(GPIO_PuPd << (2 * pinpos)));
		
// 		/* Set GPIO MODE register */
// 		GPIOx->MODER = (GPIOx->MODER & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Mode << (2 * pinpos)));
		
// 		/* Set only if output or alternate functions */
// 		if (GPIO_Mode == MD_GPIO_Mode_OUT || GPIO_Mode == MD_GPIO_Mode_AF) {		
// 			/* Set GPIO OTYPE register */
// 			GPIOx->OTYPER = (GPIOx->OTYPER & ~(uint16_t)(0x01 << pinpos)) | ((uint16_t)(GPIO_OType << pinpos));
			
// 			/* Set GPIO OSPEED register */
// 			GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~((uint32_t)(0x03 << (2 * pinpos)))) | ((uint32_t)(GPIO_Speed << (2 * pinpos)));
// 		}
// 	}
// }

// uint16_t MD_GPIO_GetUsedPins(GPIO_t* GPIOx) {
// 	/* Return used */
// 	return GPIO_UsedPins[MD_GPIO_GetPortSource(GPIOx)];
// }

// uint16_t MD_GPIO_GetFreePins(GPIO_t* GPIOx) {
// 	/* Return free pins */
// 	return ~GPIO_UsedPins[MD_GPIO_GetPortSource(GPIOx)];
// }
