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

#if defined(__ICCARM__) /* IAR Compiler */
#pragma data_alignment = 16
uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#elif defined(__GNUC__) /* GNU Compiler */
uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER] __attribute__((aligned(16u)));
#else                   /* ARM Compiler */
__align(16) uint8_t PhysFrameBuffer[COLUMN_NUMBER * PAGE_NUMBER];
#endif                  /* __ICCARM__ */

static int32_t WriteReg(SSD1315_CTX_t* p_ctx, uint16_t u_reg, uint8_t* u_data, uint16_t u_length);
static int32_t WriteRegWrap(void* p_handle, uint16_t u_reg, uint8_t* p_data, uint16_t u_length);
static void    Clear(uint16_t u_colorCode);
static int32_t Delay(SSD1315_Object_t* p_obj, uint32_t u_delay);

SSD1315_Status_t SSD1315_RegisterBusIO(SSD1315_Object_t* p_obj, SSD1315_IO_t* p_io)
{
  SSD1315_Status_t ret = SSD1315_OK;

  if(p_obj == NULL)
  {
    ret = SSD1315_ERR;
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
    ret = SSD1315_ERR;
  }
  return ret;
}

SSD1315_Status_t SSD1315_Init(SSD1315_Object_t* p_obj)
{
  SSD1315_Status_t ret = SSD1315_OK;
  uint8_t          data;

  if(p_obj->isInitialized == 0u)
  {
    p_obj->isInitialized = 1u;
    (void)Delay(p_obj, 100u);
    /* Driving ability setting */
    data = SSD1315_READWRITE_CMD;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_CHARGE_PUMP_SETTING;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_MEMORY_ADRESS_MODE;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_LOWER_COLUMN_START_ADRESS;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_DISPLAY_START_LINE_1;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_REMAPPED_MODE;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_CONTRAST_CONTROL;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_DISPLAY_ON;
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
    Clear(p_obj->backgroundColor);
    ret += WriteReg(&p_obj->ctx, SSD1315_REG_DATA, PhysFrameBuffer, COLUMN_NUMBER * PAGE_NUMBER);
  }
  else
  {
    ret = SSD1315_UNINITIALIZED;
  }

  return ret;
}

SSD1315_Status_t SSD1315_DeInit(SSD1315_Object_t* p_obj)
{
  SSD1315_Status_t ret = SSD1315_OK;

  if(p_obj->isInitialized != 0U)
  {
    ret += SSD1315_DisplayOff(p_obj);

    p_obj->isInitialized = 0;
  }

  if(ret != SSD1315_OK)
  {
    ret = SSD1315_UNINITIALIZED;
  }

  return ret;
}

SSD1315_Status_t SSD1315_DisplayOn(SSD1315_Object_t* p_obj)
{
  SSD1315_Status_t ret = SSD1315_OK;
  uint8_t          data;

  data = SSD1315_CHARGE_PUMP_SETTING;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_DISPLAY_ON;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);

  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERR;
  }
  return ret;
}

SSD1315_Status_t SSD1315_DisplayOff(SSD1315_Object_t* p_obj)
{
  SSD1315_Status_t ret = SSD1315_OK;
  uint8_t          data;

  data = SSD1315_CHARGE_PUMP_SETTING;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_HIGHER_COLUMN_START_ADRESS_1;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_DISPLAY_OFF;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);

  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERR;
  }
  return ret;
}

SSD1315_Status_t SSD1315_Refresh(SSD1315_Object_t* p_obj)
{
  SSD1315_Status_t ret = SSD1315_OK;
  uint8_t          data;

  data = SSD1315_DISPLAY_START_LINE_1;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_SET_COLUMN_ADRESS;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_DISPLAY_START_LINE_64;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_SET_PAGE_ADRESS;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_LOWER_COLUMN_START_ADRESS;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  data = SSD1315_LOWER_COLUMN_START_ADRESS_15;
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_CONTROL, &data, 1u);
  ret += WriteReg(&p_obj->ctx, SSD1315_REG_DATA, PhysFrameBuffer, COLUMN_NUMBER * PAGE_NUMBER);

  if(ret != SSD1315_OK)
  {
    ret = SSD1315_ERR;
  }
  return ret;
}

static void Clear(uint16_t u_colorCode)
{
  /* Check color */
  if(u_colorCode == SSD1315_COLOR_WHITE)
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_WHITE, COLUMN_NUMBER * PAGE_NUMBER);
  }
  else
  {
    memset(PhysFrameBuffer, SSD1315_COLOR_BLACK, COLUMN_NUMBER * PAGE_NUMBER);
  }
}

static int32_t WriteReg(SSD1315_CTX_t* p_ctx, uint16_t u_reg, uint8_t* u_data, uint16_t u_length)
{
  return p_ctx->WriteReg(p_ctx->handle, u_reg, u_data, u_length);
}

static int32_t WriteRegWrap(void* p_handle, uint16_t u_reg, uint8_t* p_data, uint16_t u_length)
{
  SSD1315_Object_t* pobj = (SSD1315_Object_t*)p_handle;

  return pobj->io.writeReg(u_reg, p_data, u_length);
}

static int32_t Delay(SSD1315_Object_t* p_obj, uint32_t u_delay)
{
  uint32_t tickStart;
  tickStart = p_obj->io.getTick();
  while((p_obj->io.getTick() - tickStart) < u_delay)
  {
  }
  return SSD1315_OK;
}