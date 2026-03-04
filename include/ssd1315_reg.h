/**
 ******************************************************************************
 * @file    ssd1315_reg.h
 * @author  MarcoAAG
 * @version 1.0.0
 * @brief   This file includes the OLED driver for the following controller:{SSD1315}
 ******************************************************************************
 * @attention
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SSD1315_REG_H
#define SSD1315_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */

// SSD1315 Colors
#define SSD1315_COLOR_WHITE 0xFF
#define SSD1315_COLOR_BLACK 0x00

// SSD1315 Scrolling
#define SSD1315_SCROLL_RIGHT          0x26
#define SSD1315_SCROLL_LEFT           0x27
#define SSD1315_ACTIVATE_SCROLL       0x2F
#define SSD1315_DESACTIVATE_SCROLL    0x2E

#define SSD1315_SCROLL_FREQ_2FRAMES   0x07
#define SSD1315_SCROLL_FREQ_3FRAMES   0x04
#define SSD1315_SCROLL_FREQ_4FRAMES   0x05
#define SSD1315_SCROLL_FREQ_5FRAMES   0x06
#define SSD1315_SCROLL_FREQ_6FRAMES   0x00
#define SSD1315_SCROLL_FREQ_32FRAMES  0x01
#define SSD1315_SCROLL_FREQ_64FRAMES  0x02
#define SSD1315_SCROLL_FREQ_128FRAMES 0x03

// SSD1315 Commands
#define SSD1315_CHARGE_PUMP_SETTING          0x8D
#define SSD1315_READWRITE_CMD                0x80
#define SSD1315_HIGHER_COLUMN_START_ADRESS_1 0x10
#define SSD1315_HIGHER_COLUMN_START_ADRESS_2 0x11
#define SSD1315_HIGHER_COLUMN_START_ADRESS_3 0x12
#define SSD1315_HIGHER_COLUMN_START_ADRESS_4 0x13
#define SSD1315_HIGHER_COLUMN_START_ADRESS_5 0x14
#define SSD1315_HIGHER_COLUMN_START_ADRESS_6 0x15
#define SSD1315_HIGHER_COLUMN_START_ADRESS_7 0x16
#define SSD1315_HIGHER_COLUMN_START_ADRESS_8 0x17
#define SSD1315_LOWER_COLUMN_START_ADRESS    0x00
#define SSD1315_LOWER_COLUMN_START_ADRESS_15 0x0F
#define SSD1315_MEMORY_ADRESS_MODE           0x20
#define SSD1315_SET_COLUMN_ADRESS            0x21
#define SSD1315_SET_PAGE_ADRESS              0x22
#define SSD1315_DISPLAY_START_LINE_1         0x40
#define SSD1315_DISPLAY_START_LINE_32        0x1F
#define SSD1315_DISPLAY_START_LINE_64        0x7F
#define SSD1315_REMAPPED_MODE                0xC8
#define SSD1315_CONTRAST_CONTROL             0xA1
#define SSD1315_CONTRAST_CONTROL_2           0xFF
#define SSD1315_DISPLAY_ON                   0xAF
#define SSD1315_DISPLAY_OFF                  0xAE
#define SSD1315_SET_PAGE_START_ADRESS        0xB0

// SSD1315 REG COMMAND
#define SSD1315_REG_CONTROL 0x00u
#define SSD1315_REG_DATA    0x40u

#ifdef __cplusplus
}
#endif

#endif /* SSD1315_REG_H */
