#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

#define FLASH_APP_BASH 0X08010000
#define FLASH_SECTOR_SIZE 2048

u16 FLASH_ReadHalfWord(u32 addr);
void FLASH_Read(u32 addr, u8 *tmp, u32 len);
void FLASH_Write(u32 addr, u8 *tmp, u32 len);

#endif
