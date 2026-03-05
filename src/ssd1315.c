/**********************************************************************************************************************
 * \file ssd1315.c
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

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <config.h>
#include <ssd1315.h>
#include <ssd1315_reg.h>
#include <stddef.h>
#include <string.h>

#define SSD1315_OK    (0)
#define SSD1315_ERROR (-1)

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 16
uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#elif defined(__GNUC__) /* GNU Compiler */
uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER] __attribute__((aligned(16)));
#else                   /* ARM Compiler */
__align(16) uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#endif                  /* __ICCARM__ */

static int32_t WriteReg(SSD1315_CTX_t* ctx, uint16_t reg, uint8_t* data, uint16_t length);
static int32_t WriteRegWrap(void* handle, uint16_t Reg, uint8_t* pData, uint16_t Length);
static void    Clear(uint16_t colorCode);
static int32_t Delay(SSD1315_Object_t* p_obj, uint32_t delay);

int32_t SSD1315_RegisterBusIO(SSD1315_Object_t* p_obj, SSD1315_IO_t* p_io)
{
  int32_t ret = SSD1315_OK;

  if(p_obj == NULL)
  {
    ret = SSD1315_ERROR;
  }
  else
  {
    p_obj->io.init     = p_io->init;
    p_obj->io.deInit   = p_io->deInit;
    p_obj->io.writeReg = p_io->writeReg;
    p_obj->io.getTick  = p_io->getTick;

    p_obj->ctx.WriteReg = WriteRegWrap;
    p_obj->ctx.handle   = p_obj;
  }

  if(p_obj->io.init != NULL)
  {
    ret = p_obj->io.init();
  }
  else
  {
    ret = SSD1315_ERROR;
  }
  return ret;
}

int8_t SSD1315_Init(SSD1315_Object_t* p_obj)
{
  int8_t  ret = SSD1315_OK;
  uint8_t data;

  if(p_obj->isInitialized == 0)
  {
    p_obj->isInitialized = 1;
    (void)Delay(p_obj, 100);
    /* Driving ability setting */
    data = SSD1315_READWRITE_CMD;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_CHARGE_PUMP_SETTING;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_MEMORY_ADRESS_MODE;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_LOWER_COLUMN_START_ADRESS;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_DISPLAY_START_LINE_1;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_REMAPPED_MODE;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_CONTRAST_CONTROL;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    data = SSD1315_DISPLAY_ON;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1);
    Clear(p_obj->backgroundColor);
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_DATA, PhysFrameBuffer, COLUMN_NUMBER * PAGE_NUMBER);
  }
  else
  {
    ret = SSD1315_ERROR;
  }

  return ret;
}

static void Clear(uint16_t colorCode)
{
  /* Check color */
  if(colorCode == SSD1315_COLOR_WHITE)
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_WHITE, COLUMN_NUMBER * PAGE_NUMBER);
  }
  else
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_BLACK, COLUMN_NUMBER * PAGE_NUMBER);
  }
}

static int32_t WriteReg(SSD1315_CTX_t* ctx, uint16_t reg, uint8_t* data, uint16_t length)
{
  return ctx->WriteReg(ctx->handle, reg, data, length);
}

static int32_t WriteRegWrap(void* handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  SSD1315_Object_t* pObj = (SSD1315_Object_t*)handle;

  return pObj->io.writeReg(Reg, pData, Length);
}

static int32_t Delay(SSD1315_Object_t* p_obj, uint32_t delay)
{
  uint32_t tickstart;
  tickstart = p_obj->io.getTick();
  while((p_obj->io.getTick() - tickstart) < delay)
  {
  }
  return SSD1315_OK;
}