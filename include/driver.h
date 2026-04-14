/**********************************************************************************************************************
 * \file driver.h
 *
 * \author MarcoAAG
 *
 * \brief This file includes the OLED driver for the following controller:{SSD1315 SSD1309}
 *
 ******************************************************************************
 * @attention
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *********************************************************************************************************************/

#ifndef DRIVER_H
#define DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <config.h>
#include <stdint.h>

typedef enum
{
  MOD_OK            = 0,
  MOD_ERR           = -1,
  MOD_UNINITIALIZED = -2
} MOD_Status_t;

typedef int8_t (*MOD_InitFunction)(void);
typedef int8_t (*MOD_DeInitFunction)(void);
typedef int8_t (*MOD_GetTickFunction)(void);
typedef int8_t (*MOD_WriteRegFunction)(uint16_t, uint8_t*, uint16_t);

typedef struct
{
  MOD_InitFunction     init;
  MOD_DeInitFunction   deInit;
  MOD_GetTickFunction  getTick;
  MOD_WriteRegFunction writeReg;
} MOD_IO_t;

typedef struct
{
  MOD_IO_t io;
  uint8_t  isInitialized;
  uint8_t  backgroundColor;
#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 16
  uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#elif defined(__GNUC__) /* GNU Compiler */
  uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER] __attribute__((aligned(16u)));
#else                   /* ARM Compiler */
  __align(16) uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#endif                  /* __ICCARM__ */
} MOD_Object_t;

MOD_Status_t SSD1315_RegisterBusIO(MOD_Object_t* p_obj, MOD_IO_t* p_io);
MOD_Status_t SSD1315_Init(MOD_Object_t* p_obj);
MOD_Status_t SSD1315_DeInit(MOD_Object_t* p_obj);
MOD_Status_t SSD1315_DisplayOn(MOD_Object_t* p_obj);
MOD_Status_t SSD1315_DisplayOff(MOD_Object_t* p_obj);
MOD_Status_t SSD1315_Refresh(MOD_Object_t* p_obj);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_H */
