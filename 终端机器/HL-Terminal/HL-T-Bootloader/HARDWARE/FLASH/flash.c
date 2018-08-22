#include "flash.h"

/* read flash half word data */
u16 FLASH_ReadHalfWord(u32 addr)
{
	return (*(vu16 *)addr);
}

/* write flash long data */
void FLASH_Write(u32 addr, u8 *tmp, u32 len)
{
	u8 buf[FLASH_SECTOR_SIZE] = {0};
	u16 block_use_sum = 0;
	u16 first_block_offset = 0;
	u16 i, cnt;
	u8 *p = buf;
	
	block_use_sum = len / FLASH_SECTOR_SIZE + 1;    //È·±£ >= 1
	first_block_offset = len % FLASH_SECTOR_SIZE;
	
	//determine whether the address is legal.
	if(addr < FLASH_APP_BASH || addr > FLASH_APP_BASH + 1024*FLASH_SECTOR_SIZE) return ;
	
	FLASH_Unlock();	
	for(i = 0; i < block_use_sum; i++)
	{
		u32 cur_block_addr = FLASH_APP_BASH + 2048*i;
		u32 a, b;
		
		//Read data block to buffer.
		FLASH_Read(cur_block_addr, buf, FLASH_SECTOR_SIZE);
		
		//determine whether the block nees erase. 
		for(cnt = 0; cnt < FLASH_SECTOR_SIZE; cnt++)
		{
			//Erase data block.
			if(buf[cnt] != 0xff)
			{
				FLASH_ErasePage(cur_block_addr);
				break;
			}
		}

		//user data write in buffer
		if(i == 0) a = first_block_offset; else a = 0;
		if(len < FLASH_SECTOR_SIZE) b = len; 
		else 
		{
			b = FLASH_SECTOR_SIZE;
			len -= FLASH_SECTOR_SIZE;
		}
		for(cnt = a; cnt < b; cnt++)
			buf[cnt] = *p++;
		
		//Write buffer data to data block.
		for(cnt = 0; cnt < FLASH_SECTOR_SIZE; cnt += 2)
			FLASH_ProgramHalfWord(cur_block_addr + cnt, *(u16 *)(buf + cnt));
	}
	FLASH_Lock();
}

/* read flash long data */
void FLASH_Read(u32 addr, u8 *tmp, u32 len)
{
	u32 i;
	
	for(i = 0; i < len; i += 2)
		*(u16 *)(tmp + i) = FLASH_ReadHalfWord(addr + i);
}
