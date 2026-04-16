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

#ifndef MONOLED_HAL_H
#define MONOLED_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <display-config.h>
#include <stdint.h>

typedef enum
{
  MONOLED_OK            = 0,
  MONOLED_ERR           = -1,
  MONOLED_UNINITIALIZED = -2
} MONOLED_Status_t;

typedef int8_t (*MONOLED_InitFunction)(void);
typedef int8_t (*MONOLED_DeInitFunction)(void);
typedef int8_t (*MONOLED_GetTickFunction)(void);
typedef int8_t (*MONOLED_WriteRegFunction)(uint16_t, uint8_t*, uint16_t);

typedef struct
{
  MONOLED_InitFunction     init;
  MONOLED_DeInitFunction   deInit;
  MONOLED_GetTickFunction  getTick;
  MONOLED_WriteRegFunction writeReg;
} MONOLED_IO_t;

typedef struct
{
  MONOLED_IO_t io;
  uint8_t      isInitialized;
  uint8_t      backgroundColor;
#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 16
  uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#elif defined(__GNUC__) /* GNU Compiler */
  uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER] __attribute__((aligned(16u)));
#else                   /* ARM Compiler */
  __align(16) uint8_t frameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#endif                  /* __ICCARM__ */
} MONOLED_Object_t;

MONOLED_Status_t MONOLED_RegisterBusIO(MONOLED_Object_t* p_obj, MONOLED_IO_t* p_io);
MONOLED_Status_t MONOLED_Init(MONOLED_Object_t* p_obj);
MONOLED_Status_t MONOLED_DeInit(MONOLED_Object_t* p_obj);
MONOLED_Status_t MONOLED_DisplayOn(MONOLED_Object_t* p_obj);
MONOLED_Status_t MONOLED_DisplayOff(MONOLED_Object_t* p_obj);
MONOLED_Status_t MONOLED_Refresh(MONOLED_Object_t* p_obj);

#ifdef __cplusplus
}
#endif

#endif /* MONOLED_HAL_H */
