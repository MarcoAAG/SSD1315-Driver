/**********************************************************************************************************************
 * \file ssd1315.h
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

#ifndef SSD1315_H
#define SSD1315_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <stdint.h>

typedef int32_t (*SSD1315_WriteFunc)(void*, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  SSD1315_WriteFunc WriteReg;
  void*             handle;
} SSD1315_CTX_t;

typedef int8_t (*SSD1315_InitFunction)(void);
typedef int8_t (*SSD1315_DeInitFunction)(void);
typedef int8_t (*SSD1315_GetTickFunction)(void);
typedef int8_t (*SSD1315_WriteRegFunction)(uint16_t, uint8_t*, uint16_t);

typedef struct
{
  SSD1315_InitFunction     init;
  SSD1315_DeInitFunction   deInit;
  SSD1315_GetTickFunction  getTick;
  SSD1315_WriteRegFunction writeReg;
} SSD1315_IO_t;

typedef struct
{
  SSD1315_IO_t  io;
  SSD1315_CTX_t ctx;
  uint8_t       isInitialized;
  uint8_t       backgroundColor;
} SSD1315_Object_t;

int32_t SSD1315_RegisterBusIO(SSD1315_Object_t* p_obj, SSD1315_IO_t* p_io);
int8_t  SSD1315_Init(SSD1315_Object_t* p_obj);
int32_t SSD1315_DeInit(SSD1315_Object_t* p_obj);
int32_t SSD1315_DisplayOn(SSD1315_Object_t* p_obj);
int32_t SSD1315_DisplayOff(SSD1315_Object_t* p_obj);
int32_t SSD1315_Refresh(SSD1315_Object_t* p_obj);

#ifdef __cplusplus
}
#endif

#endif /* SSD1315_H */
