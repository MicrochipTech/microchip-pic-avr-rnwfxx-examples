/* 
 * File:   SST25WF080B_functions.h
 * Author: C03009
 *
 * Created on January 26, 2017, 9:55 AM
 */

#ifndef SST2X_DRIVER_H
#define	SST2X_DRIVER_H


#include "mcc_generated_files/system/pins.h"

#define SST25WF080B_ID          0x14

#define SST26VF064B_ID          0x43
#define SST26VF032B_ID          0x42
#define SST26VF016B_ID          0x51


#define FLASH_SECTOR_SIZE      0x1000
#define FLASH_PAGE_SIZE         0x100

unsigned char Read_Status_Register();

void WRSR(char byte);
void WREN();
void PD();
void EPD();
void WRDI();
unsigned char Read_ID(char ID_addr);
void Jedec_ID_Read(uint8_t *Manufacturer_Id, uint8_t *Device_Type, uint8_t *Device_Id);
unsigned char Read(unsigned long Dst); 
void Read_Cont(uint32_t Dst, uint32_t no_bytes, char *read_data);
unsigned char HighSpeed_Read(uint32_t Dst); 
void HighSpeed_Read_Cont(uint32_t Dst,uint32_t no_bytes, char *read_data);
void Page_Program(unsigned long Dst, char *Prog_data);
void Chip_Erase();
void Small_Sector_Erase(unsigned long Dst);
void Sector_Erase(unsigned long Dst);
void Wait_Busy();
void Sector_Program(uint32_t Dst, unsigned char *Prog_data, int length);
void SPI_Global_Block_Protection_Unlock();


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SST2X_DRIVER_H */

