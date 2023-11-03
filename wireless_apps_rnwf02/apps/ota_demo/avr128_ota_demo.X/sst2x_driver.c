
#include <stdio.h>
#include "sst2x_driver.h"
#include "mcc_generated_files/spi/spi0.h"


/************************************************************************/
/* PROCEDURE: Read_Status_Register										*/
/*																		*/
/* This procedure read the status register and returns the byte.		*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/************************************************************************/
unsigned char Read_Status_Register()
{
	unsigned char byte = 0;
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x05);		/* send RDSR command */
	byte = SPI0_s.ByteExchange(0xFF);		/* receive byte */
	CE_SetHigh();			/* disable device */
	return byte;
}


/************************************************************************/
/* PROCEDURE: WRSR														*/
/*																		*/
/* This procedure writes a byte to the Status Register.					*/
/*																		*/
/* Input:																*/
/*		byte															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WRSR(char byte)
{
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x01);		/* select write to status register */
	SPI0_s.ByteExchange(byte);		/* data that will change the status of BPx 
					   or BPL (only bits 2,3,4,5,7 can be written) */
	CE_SetHigh();			/* disable the device */
}

/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WREN()
{
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x06);		/* send WREN command */
	CE_SetHigh();			/* disable device */
}

/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void PD()
{
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0xB9);		/* send WREN command */
	CE_SetHigh();			/* disable device */
}

/************************************************************************/
/* PROCEDURE: WREN														*/
/*																		*/
/* This procedure enables the Write Enable Latch.  It can also be used 	*/
/* to Enables Write Status Register.									*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void EPD()
{
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0xAB);		/* send WREN command */
	CE_SetHigh();			/* disable device */
}

/************************************************************************/
/* PROCEDURE: WRDI														*/
/*																		*/
/* This procedure disables the Write Enable Latch.						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void WRDI()
{
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x04);		/* send WRDI command */
	CE_SetHigh();			/* disable device */
}


/************************************************************************/
/* PROCEDURE: Read_ID													*/
/*																		*/
/* This procedure Reads the manufacturer's ID and device ID.  It will 	*/
/* use 90h or ABh as the command to read the ID (90h in this sample).   */
/* It is up to the user to give the last byte ID_addr to determine      */
/* whether the device outputs manufacturer's ID first, or device ID 	*/
/* first.  Please see the product datasheet for details.  Returns ID in */
/* variable byte.														*/
/*																		*/
/* Input:																*/
/*		ID_addr															*/
/*																		*/
/* Returns:																*/
/*		byte:	ID1(Manufacture's ID = BFh or Device ID = 03h)			*/
/*																		*/
/************************************************************************/
unsigned char Read_ID(char ID_addr)
{
	unsigned char byte;
	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0xAB);		/* send read ID command (90h or ABh) */
    SPI0_s.ByteExchange(0x00);		/* send address */
	SPI0_s.ByteExchange(0x00);		/* send address */
	SPI0_s.ByteExchange(ID_addr);		/* send address - either 00H or 01H */
	byte = SPI0_s.ByteExchange(0xFF);		/* receive byte */
	CE_SetHigh();			/* disable device */
	return byte;
}

/************************************************************************/
/* PROCEDURE: Jedec_ID_Read												*/
/*																		*/
/* This procedure Reads the manufacturer's ID (BFh), memory type (25h)  */
/* and device ID (03h).  It will use 9Fh as the JEDEC ID command.    	*/
/* Please see the product datasheet for details.  						*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		IDs_Read:ID1(Manufacture's ID = BFh, Memory Type (25h), 		*/
/*		 and Device ID (03h)											*/
/*																		*/
/************************************************************************/
void Jedec_ID_Read(uint8_t *Manufacturer_Id, uint8_t *Device_Type, uint8_t *Device_Id)
{	
	CE_SetLow();			 /* enable device */
	SPI0_s.ByteExchange(0x9F);		 /* send JEDEC ID command (9Fh) */
    *Manufacturer_Id = SPI0_s.ByteExchange(0xFF);       /* receive byte */
  	*Device_Type = SPI0_s.ByteExchange(0xFF);           /* receive byte */
  	*Device_Id = SPI0_s.ByteExchange(0xFF);             /* receive byte */
    CE_SetHigh();			 /* disable device */

}

/************************************************************************/
/* PROCEDURE:	Read													*/
/*																		*/		
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.											*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:	Destination Address 000000H - 3FFFFH					*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/*																		*/
/************************************************************************/
unsigned char Read(unsigned long Dst) 
{
	unsigned char byte = 0;	

	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x03); 		/* read command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
	byte = SPI0_s.ByteExchange(0xFF);
	CE_SetHigh();			/* disable device */
	return byte;			/* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	Read_Cont												*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*      	no_bytes	Number of bytes to read	(max = 128)				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Read_Cont(uint32_t Dst, uint32_t no_bytes, char *read_data)
{
	uint8_t i = 0;    
	CE_SetLow();				/* enable device */
	SPI0_s.ByteExchange(0x03); 			/* read command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
	for (i = 0; i < no_bytes; i++)		/* read until no_bytes is reached */
	{
		read_data[i] = SPI0_s.ByteExchange(0xFF);	/* receive byte and store at address 80H - FFH */        
    }
	CE_SetHigh();				/* disable device */

}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read											*/
/*																		*/		
/* This procedure reads one address of the device.  It will return the 	*/
/* byte read in variable byte.											*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:	Destination Address 000000H - 3FFFFH					*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		byte															*/
/*																		*/
/************************************************************************/
unsigned char HighSpeed_Read(unsigned long Dst) 
{
	unsigned char byte = 0;	

	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x0B); 		/* read command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
	SPI0_s.ByteExchange(0xFF);		/*dummy byte*/
	byte = SPI0_s.ByteExchange(0xFF);
	CE_SetHigh();			/* disable device */
	return byte;			/* return one byte read */
}

/************************************************************************/
/* PROCEDURE:	HighSpeed_Read_Cont										*/
/*																		*/		
/* This procedure reads multiple addresses of the device and stores		*/
/* data into 128 byte buffer. Maximum byte that can be read is 128 bytes*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*      	no_bytes	Number of bytes to read	(max = 128)				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void HighSpeed_Read_Cont(uint32_t Dst, uint32_t no_bytes, char *read_data)
{
	uint8_t i = 0;
    uint32_t read_bytes, offset = 0;
    while(no_bytes > 0)
    {
        read_bytes = (no_bytes > 128)?128:no_bytes;
        CE_SetLow();				/* enable device */
        SPI0_s.ByteExchange(0x0B); 			/* read command */
        SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
        SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
        SPI0_s.ByteExchange(Dst & 0xFF);
        SPI0_s.ByteExchange(0xFF);			/*dummy byte*/
        for (i = 0; i < read_bytes; i++)		/* read until no_bytes is reached */
        {
            read_data[i + offset] = SPI0_s.ByteExchange(0xFF);	/* receive byte and store at address 80H - FFH */        
        }
        CE_SetHigh();				/* disable device */
        no_bytes -= read_bytes;
        offset += read_bytes;
        Dst+=read_bytes;
    }
}



/************************************************************************/
/* PROCEDURE:	Byte_Program											*/
/*																		*/
/* This procedure programs one address of the device.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.												*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*		byte:		byte to be programmed								*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Page_Program(uint32_t Dst, char *Prog_data)
{
    int i =0;
    CE_SetLow();				/* enable device */    
	SPI0_s.ByteExchange(0x02); 			/* send Byte Program command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
    for(i=0;i<256;i++)
    {
        SPI0_s.ByteExchange(*Prog_data);
        Prog_data++;
    }
    CE_SetHigh();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: Chip_Erase												*/
/*																		*/
/* This procedure erases the entire Chip.								*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Chip_Erase()
{						
	CE_SetLow();				/* enable device */
    SPI0_s.ByteExchange(0xC7);
//	 SPI0_s.ByteExchange(0x60);			/* send Chip Erase command (60h or C7h) */
	CE_SetHigh();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: Small_Sector_Erase												*/
/*																		*/
/* This procedure Sector Erases the Chip.								*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Small_Sector_Erase(unsigned long Dst)
{


	CE_SetLow();				/* enable device */
	SPI0_s.ByteExchange(0x20);			/* send Sector Erase command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
	CE_SetHigh();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: Sector_Erase											*/
/*																		*/
/* This procedure Block Erases 64 KByte of the Chip.					*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Sector_Erase(unsigned long Dst)
{
	CE_SetLow();				/* enable device */
	SPI0_s.ByteExchange(0xD8);			/* send 64KByte Block Erase command */
	SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16)); 	/* send 3 address bytes */
	SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
	SPI0_s.ByteExchange(Dst & 0xFF);
	CE_SetHigh();				/* disable device */
}

/************************************************************************/
/* PROCEDURE: Wait_Busy													*/
/*																		*/
/* This procedure waits until device is no longer busy (can be used by	*/
/* Byte-Program, Sector-Erase, Block-Erase, Chip-Erase).				*/
/*																		*/
/* Input:																*/
/*		None															*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/************************************************************************/
void Wait_Busy()
{
	while ((Read_Status_Register() & 0x03) == 0x03);	/* waste time until not busy */
		//Read_Status_Register();
}


/************************************************************************/
/* PROCEDURE:	Byte_Program											*/
/*																		*/
/* This procedure programs one address of the device.					*/
/* Assumption:  Address being programmed is already erased and is NOT	*/
/*		block protected.												*/
/*																		*/
/*																		*/
/*																		*/
/* Input:																*/
/*		Dst:		Destination Address 000000H - 3FFFFH				*/
/*		byte:		byte to be programmed								*/
/*      																*/
/*																		*/
/* Returns:																*/
/*		Nothing															*/
/*																		*/
/************************************************************************/
void Sector_Program(uint32_t Dst, unsigned char *Prog_data, int length)
{    
    int i;
    volatile uint16_t pages = (length/FLASH_PAGE_SIZE);
    volatile uint8_t bytes = (length%FLASH_PAGE_SIZE);    
                
    while(pages--)
    {        
        WREN();
        while(!(Read_Status_Register() & 0x02));
        CE_SetLow();				/* enable device */        
        SPI0_s.ByteExchange(0x02); 			/* send Byte Program command */
        SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
        SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
        SPI0_s.ByteExchange(Dst & 0xFF);
        for(i=0;i<256;i++)
        {
            SPI0_s.ByteExchange(*Prog_data);
            Prog_data++;
        }
        CE_SetHigh();				/* disable device */
        while(Read_Status_Register() & 0x01);        
        Dst += FLASH_PAGE_SIZE;        
    }
    if(bytes)
    {
        WREN();
        while(!(Read_Status_Register() & 0x02));
        CE_SetLow();				/* enable device */
        SPI0_s.ByteExchange(0x02); 			/* send Byte Program command */
        SPI0_s.ByteExchange(((Dst & 0xFFFFFF) >> 16));	/* send 3 address bytes */
        SPI0_s.ByteExchange(((Dst & 0xFFFF) >> 8));
        SPI0_s.ByteExchange(Dst & 0xFF);
        for(i=0;i<bytes;i++)
        {
            SPI0_s.ByteExchange(*Prog_data);
            Prog_data++;
        }
        CE_SetHigh();				/* disable device */   
        while(Read_Status_Register() & 0x01);
    }               
}
/************************************************************************/
/* PROCEDURE:	Global Block Protection Unlock				*/
/*									*/
/* This procedure clears all block protection				*/
/************************************************************************/
void SPI_Global_Block_Protection_Unlock()
{

	CE_SetLow();			/* enable device */
	SPI0_s.ByteExchange(0x98); 		/* read command */
	CE_SetHigh();			/* disable device */
}