/**
 * SPI0 Generated Driver API Header File
 *
 * @file spi0.h
 *
 * @defgroup spi0 SPI0
 *
 * @brief This header file provides API prototypes for the SPI0 driver.
 *
 * @version SPI0 Driver Version 3.0.0
*/
/*
? [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef SPI0_H
#define SPI0_H

#include <stdbool.h>
#include "../system/utils/compiler.h"
#include "spi_interface.h"

/**
 * @ingroup spi0
 * @struct SPI_INTERFACE SPI0
 * @brief SPI driver interface object.
 */               
extern const struct SPI_INTERFACE SPI0_s;


/**
 * @ingroup spi0
 * @typedef enum SPI0_configuration_name_t
 * @brief Enumeration for the different configurations supported by the driver. 
 * A configuration is specified as parameter to SPI0_Open()
 * and is used by the function to set SPI parameters as specified by the configuration.
 */
typedef enum 
{
    SPI0_DEFAULT
} SPI0_configuration_name_t;

/**
 * @ingroup spi0
 * @brief Initializes the SPI0 module.
 * @param None.
 * @return None.
 */
void SPI0_Initialize(void);

/**
 * @ingroup spi0
 * @brief Deinitializes the SPI0 module
 * @param None.
 * @return None.
 */
void SPI0_Deinitialize(void);

/**
 * @ingroup spi0
 * @brief Enables the SPI0 module with the configurations passed as the parameter.
 * See spi0_configuration_name_t for the available configurations.
 * @param spiConfigIndex The Configuration index
 * @retval True SPI0 module is enabled successfully with the chosen configuration
 * @retval False SPI0 module is already open with another configuration
 */
bool SPI0_Open(uint8_t spiConfigIndex);

/**
 * @ingroup spi0
 * @brief Closes the active configuration of the SPI0 module.
 * @param None.
 * @return None.
 */
void SPI0_Close(void);

/**
 * @ingroup spi0
 * @brief Exchanges one byte using the SPI protocol. This function is blocking.
 * @param byteData The byte to be written
 * @return Received data byte
 */
uint8_t SPI0_ByteExchange(uint8_t byteData);

/**
 * @ingroup spi0
 * @brief Exchanges the buffer using the SPI protocol. This function is blocking in Polling mode.
 * @param [in,out] *bufferData Buffer address of the data to be exchanged
 * @param [in] bufferSize Size of the data in bytes
 * @return None.
 */
void SPI0_BufferExchange(void * bufferData, size_t bufferSize);

/**
 * @ingroup spi0
 * @brief Writes a buffer using the SPI protocol. This function is blocking in Polling mode.
 * @param [in] *bufferData Buffer address of the data to be written
 * @param [in] bufferSize Size of the data in bytes
 * @return None.
 */
void SPI0_BufferWrite(void * bufferData, size_t bufferSize);

/**
 * @ingroup spi0
 * @brief Reads a buffer using the SPI protocol. This function is blocking in Polling mode.
 * @param [out] *bufferData Buffer address of the data to be read
 * @param [in] bufferSize Size of the data in bytes
 * @return None.
 */
void SPI0_BufferRead(void * bufferData, size_t bufferSize);

/**
 * @ingroup spi0
 * @brief Writes one byte using the SPI protocol. This function is blocking in Polling mode.
 * @param byteData The byte to be written
 * @return None.
 */
void SPI0_ByteWrite(uint8_t byteData);

/**
 * @ingroup spi0
 * @brief Receives one byte using SPI protocol. This function is blocking.
 * @param None.
 * @return Received data byte
 */
uint8_t SPI0_ByteRead(void);

/**
 * @ingroup spi0
 * @brief Checks if the SPI0 module is ready to read data.
 * @param None.
 * @retval True SPI0 module has data ready in buffer
 * @retval False SPI0 module is not ready to read data
 */
bool SPI0_IsRxReady(void);

/**
 * @ingroup spi0
 * @brief Checks if the SPI0 module is ready to write data.
 * @param None.
 * @retval True SPI0 module is ready to write data
 * @retval False SPI0 module is not ready to write data
 */
bool SPI0_IsTxReady(void);


#endif /* SPI0_H */
