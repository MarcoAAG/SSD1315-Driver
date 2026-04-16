/**********************************************************************************************************************
 * \file driver.c
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
#include <monoled-hal.h>
#include <reg.h>
#include <stddef.h>
#include <string.h>

static int32_t          WriteRegWrap(MONOLED_Object_t* p_obj, uint16_t u_reg, uint8_t* u_data, uint16_t u_length);
static void             Clear(MONOLED_Object_t* p_obj);
static MONOLED_Status_t Delay(MONOLED_Object_t* p_obj, uint32_t u_delay);

MONOLED_Status_t MONOLED_RegisterBusIO(MONOLED_Object_t* p_obj, MONOLED_IO_t* p_io)
{
  MONOLED_Status_t ret = MONOLED_OK;

  if((p_obj == NULL) || (p_io == NULL))
  {
    ret = MONOLED_ERR;
  }
  else
  {
    p_obj->io.init       = p_io->init;
    p_obj->io.deInit     = p_io->deInit;
    p_obj->io.writeReg   = p_io->writeReg;
    p_obj->io.getTick    = p_io->getTick;
    p_obj->isInitialized = 0u;
    if(p_obj->io.init != NULL)
    {
      ret = (MONOLED_Status_t)p_obj->io.init();
    }
    else
    {
      ret = MONOLED_ERR;
    }
  }

  return ret;
}

MONOLED_Status_t MONOLED_Init(MONOLED_Object_t* p_obj)
{
  MONOLED_Status_t ret = MONOLED_OK;
  uint8_t          data;

  if(p_obj != NULL)
  {
    if(p_obj->isInitialized == 0u)
    {
      p_obj->isInitialized = 1u;
#if defined(SSD1315) || defined(SSD1309)
      data = SSD1315_DISPLAY_ON;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      (void)Delay(p_obj, 100u);
#else
      /* Driving ability setting */
      data = SSD1315_READWRITE_CMD;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_CHARGE_PUMP_SETTING;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_MEMORY_ADRESS_MODE;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_LOWER_COLUMN_START_ADRESS;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_DISPLAY_START_LINE_1;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_REMAPPED_MODE;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_CONTRAST_CONTROL;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
      data = SSD1315_DISPLAY_ON;
      ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
#endif
      Clear(p_obj);
      ret += WriteRegWrap(p_obj, SSD1315_REG_DATA, p_obj->frameBuffer, COLUMN_NUMBER * PAGE_NUMBER);
    }
    else
    {
      ret = MONOLED_UNINITIALIZED;
    }
  }
  else
  {
    ret = MONOLED_ERR;
  }

  return ret;
}

MONOLED_Status_t MONOLED_DeInit(MONOLED_Object_t* p_obj)
{
  MONOLED_Status_t ret = MONOLED_OK;

  if(p_obj != NULL)
  {
    if(p_obj->isInitialized != 0U)
    {
      ret += MONOLED_DisplayOff(p_obj);
      p_obj->isInitialized = 0;
    }

    if(ret != MONOLED_OK)
    {
      ret = MONOLED_UNINITIALIZED;
    }
  }
  else
  {
    ret = MONOLED_ERR;
  }
  return ret;
}

MONOLED_Status_t MONOLED_DisplayOn(MONOLED_Object_t* p_obj)
{
  MONOLED_Status_t ret = MONOLED_OK;
  uint8_t          data;

  if(p_obj != NULL)
  {
    data = SSD1315_CHARGE_PUMP_SETTING;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_HIGHER_COLUMN_START_ADRESS_5;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_DISPLAY_ON;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);

    if(ret != MONOLED_OK)
    {
      ret = MONOLED_ERR;
    }
  }
  else
  {
    ret = MONOLED_ERR;
  }

  return ret;
}

MONOLED_Status_t MONOLED_DisplayOff(MONOLED_Object_t* p_obj)
{
  MONOLED_Status_t ret = MONOLED_OK;
  uint8_t          data;

  if(p_obj != NULL)
  {
    data = SSD1315_CHARGE_PUMP_SETTING;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_HIGHER_COLUMN_START_ADRESS_1;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_DISPLAY_OFF;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);

    if(ret != MONOLED_OK)
    {
      ret = MONOLED_ERR;
    }
  }
  else
  {
    ret = MONOLED_ERR;
  }
  return ret;
}

MONOLED_Status_t MONOLED_Refresh(MONOLED_Object_t* p_obj)
{
  MONOLED_Status_t ret = MONOLED_OK;
  uint8_t          data;

  if(p_obj != NULL)
  {
    data = SSD1315_DISPLAY_START_LINE_1;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_SET_COLUMN_ADRESS;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_LOWER_COLUMN_START_ADRESS;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_DISPLAY_START_LINE_64;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_SET_PAGE_ADRESS;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_LOWER_COLUMN_START_ADRESS;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    data = SSD1315_LOWER_COLUMN_START_ADRESS_15;
    ret += WriteRegWrap(p_obj, SSD1315_REG_CONTROL, &data, 1u);
    ret += WriteRegWrap(p_obj, SSD1315_REG_DATA, p_obj->frameBuffer, COLUMN_NUMBER * PAGE_NUMBER);

    if(ret != MONOLED_OK)
    {
      ret = MONOLED_ERR;
    }
  }
  else
  {
    ret = MONOLED_ERR;
  }

  return ret;
}

static void Clear(MONOLED_Object_t* p_obj)
{
  /* Check color */
  if(p_obj->backgroundColor == SSD1315_COLOR_WHITE)
  {
    memset(p_obj->frameBuffer, SSD1315_COLOR_WHITE, COLUMN_NUMBER * PAGE_NUMBER);
  }
  else
  {
    memset(p_obj->frameBuffer, SSD1315_COLOR_BLACK, COLUMN_NUMBER * PAGE_NUMBER);
  }
}

static int32_t WriteRegWrap(MONOLED_Object_t* p_obj, uint16_t u_reg, uint8_t* u_data, uint16_t u_length)
{
  return p_obj->io.writeReg(u_reg, u_data, u_length);
}

static MONOLED_Status_t Delay(MONOLED_Object_t* p_obj, uint32_t u_delay)
{
  uint32_t tickStart;
  tickStart = p_obj->io.getTick();
  while((p_obj->io.getTick() - tickStart) < u_delay)
  {
  }
  return MONOLED_OK;
}