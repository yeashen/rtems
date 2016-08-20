/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : poa030r.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/05/14
  Last Modified : 
  Description   : sensor POA030R driver 
  History       : 
  1.Date        : 2016/05/14
   Author       : Juven
   Modification : Created file

******************************************************************************/
#include <rtems.h>
#include <rtems/libi2c.h>

#include <libchip/poa030r.h>
#include <i2c.h>

#include <rtems/libio.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static SENSOR_DATA_S poa030r_init_para_640_480_30fps[] = {
	/* start up */
	{0x03, 0x00}, {0x59, 0x00}, {0x21, 0x10}, {0x5E, 0x18}, {0x60, 0x40}, {0x62, 0x40},  {0x63, 0x02},
	/* Bank A */
	{0x03, 0x00}, {0x64, 0x0F}, {0x68, 0x00}, {0x80, 0x00},  {0x81, 0x00}, {0x4E, 0x00}, {0x4F, 0x64},
	{0x50, 0x02}, {0x51, 0x1C}, {0x52, 0x00}, {0x53, 0x32},  {0x54, 0x01}, {0x55, 0xAE}, {0xF6, 0x86},
	{0xF7, 0x85}, {0xF8, 0x84}, {0xF9, 0x00}, {0xFA, 0x00}, {0xFB, 0x00}, {0xFC, 0xFF}, {0xFD, 0x7F}, {0x5C, 0x1D},
	/* Bank B */
	{0x03, 0x01}, {0x04, 0x00}, {0x06, 0x60}, {0x40, 0x04}, {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00}, {0x5E, 0x30},
	{0x5F, 0x10}, {0x62, 0x45}, {0x63, 0x9C}, {0x64, 0x89}, {0x65, 0x87}, {0x66, 0x30}, {0x67, 0x89}, {0x68, 0x00},
	{0x69, 0x9C}, {0x6A, 0x3C}, {0x6E, 0x03}, {0x6F, 0x0C}, {0x70, 0x19}, {0x71, 0x26}, {0x72, 0x3F}, {0x73, 0x52},
	{0x74, 0x6E}, {0x75, 0x82}, {0x76, 0xA1}, {0x77, 0xB9}, {0x78, 0xCE}, {0x79, 0xE0}, {0x7A, 0xF0}, {0x7B, 0xFF},
	{0x7F, 0x0C}, {0x80, 0x19}, {0x81, 0x22}, {0x82, 0x32}, {0x83, 0x48}, {0x84, 0x5E}, {0x85, 0x79}, {0x86, 0x8F},
	{0x87, 0xB1}, {0x88, 0xCB}, {0x89, 0xDA}, {0x8A, 0xE6}, {0x8B, 0xF2}, {0x8C, 0xFF}, {0xC5, 0x0F}, {0xC6, 0x00},
	{0xC7, 0x40}, {0xC8, 0x00}, {0xCB, 0x04}, {0xCC, 0x04}, {0xCD, 0x04}, {0xCE, 0x10}, {0xCF, 0x1F}, {0xD0, 0x04},
	{0xD3, 0x7F}, {0xD4, 0x7F}, {0xD5, 0x7F}, {0xD6, 0x30}, {0xD7, 0x1F}, {0xD8, 0x1F}, {0xD9, 0x1F}, {0xE6, 0x08},
	{0xF0, 0x00}, {0xF1, 0x83}, {0xF2, 0x1E}, {0xF3, 0x00}, {0xF4, 0x9D}, {0xF5, 0x57}, {0x4E, 0x13},{0x05, 0x03}, 
	{0x58, 0x00}, {0xB7, 0x01},
	/* Bank C */
	{0x03, 0x02}, {0x05, 0x7D}, {0x06, 0x84}, {0x16, 0x02}, {0x17, 0x0C}, {0x18, 0x04}, {0x19, 0x18}, {0x1A, 0x10},
	{0x1B, 0x60}, {0x1C, 0x00}, {0x1D, 0x00}, {0x1E, 0x0C}, {0x1F, 0x01}, {0x20, 0x06}, {0x21, 0x00}, {0x22, 0x01},
	{0x23, 0x06}, {0x24, 0x00}, {0x2B, 0xE6}, {0x2C, 0xE6}, {0x2D, 0x02}, {0x30, 0x20}, {0x31, 0x20}, {0x33, 0x10},
	{0x34, 0x40}, {0x35, 0x80}, {0x36, 0xFF}, {0x3E, 0x20}, {0x3F, 0x30}, {0x3A, 0xAA}, {0x3B, 0x90}, {0x3C, 0x90},
	{0x3D, 0x90}, {0x40, 0x04}, {0x41, 0x08}, {0x42, 0x1A}, {0x43, 0x25}, {0x44, 0x08}, {0x4A, 0x06}, {0x56, 0x26},
	{0x57, 0x40}, {0x58, 0x2F}, {0x59, 0x29}, {0x5A, 0x07}, {0x5B, 0x0C}, {0x5C, 0x9F}, {0x5D, 0x2A}, {0x5E, 0x29},
	{0x5F, 0x50}, {0x60, 0x20}, {0x61, 0x10}, {0x62, 0x09}, {0x63, 0x10}, {0x64, 0xF0}, {0x65, 0x08}, {0x66, 0x10},
	{0x68, 0x15}, {0x69, 0x2B}, {0x78, 0x0A}, {0x79, 0x04}, {0x6C, 0x7E}, {0x6D, 0x7C}, {0x6E, 0x80}, {0x6F, 0x80},
	{0x70, 0x80}, {0x71, 0x80}, {0x7B, 0x48}, {0x7C, 0x34}, {0x7D, 0x60}, {0x7E, 0x68}, {0x7F, 0x48}, {0x80, 0x40},
	{0x81, 0x78}, {0x82, 0x7B}, {0x83, 0x00}, {0x84, 0x08}, {0x85, 0x00}, {0x86, 0x40}, {0x88, 0x3E}, {0x89, 0x8C},
	{0x8A, 0x94}, {0x8B, 0x2C}, {0x8C, 0x21}, {0x8D, 0x84}, {0x8E, 0x04}, {0x8F, 0x27}, {0x90, 0x1D}, {0x91, 0x04},
	{0x92, 0x00}, {0x93, 0x1D}, {0x94, 0x04}, {0x95, 0x00}, {0x96, 0x00}, {0x97, 0x00}, {0x98, 0x04}, {0x99, 0x00},
	{0x9A, 0x34}, {0x9B, 0x55}, {0x9C, 0x64}, {0xA3, 0x00}, {0xA4, 0x00}, {0xA5, 0x00}, {0xA6, 0x10}, {0xA7, 0x18},
	{0xA8, 0x1F}, {0xAA, 0x00}, {0xAB, 0x08}, {0xAC, 0x1A}, {0xAD, 0x28}, {0xAE, 0x2F}, {0xAF, 0x3F}, {0xB1, 0x00},
	{0xB2, 0x08}, {0xB3, 0x10}, {0xB4, 0x18}, {0xB5, 0x3F}, {0xB6, 0x10}, {0xC6, 0x00}, {0xC7, 0x00}, {0xC8, 0x00},
	{0xC9, 0x10}, {0xCA, 0x10}, {0xCB, 0x10}, {0xCD, 0x3F}, {0xCE, 0x3F}, {0xCF, 0x3F}, {0xD0, 0x20}, {0xD1, 0x10},
	{0xD2, 0x00}, {0xD4, 0x3F}, {0xD5, 0x3F}, {0xD6, 0x3F}, {0xD7, 0x20}, {0xD8, 0x10}, {0xD9, 0x00}, {0xDB, 0x00},
	{0xDC, 0x00}, {0xDD, 0x00}, {0xDE, 0x0A}, {0xDF, 0x1A}, {0xE0, 0x00}, {0xE2, 0x00}, {0xE3, 0x04}, {0xE4, 0x08},
	{0xE5, 0x18}, {0xE6, 0x1F}, {0xE7, 0x1F}, {0xE9, 0x00}, {0xEA, 0x00}, {0xEB, 0x04}, {0xEC, 0x0E}, {0xED, 0x18},
	{0xEE, 0x18}, {0xF0, 0x00}, {0xF1, 0x00}, {0xF2, 0x04}, {0xF3, 0x10}, {0xF4, 0x18}, {0xF5, 0x18}, {0xF7, 0x08},
	{0xF8, 0x08}, {0xF9, 0x08}, {0xFA, 0x14}, {0xFB, 0x1C}, {0xFC, 0x08}, {0xB8, 0x00}, {0xB9, 0x00}, {0xBA, 0x00},
};

static SENSOR_DATA_S poa030r_init_para_320_240_30fps[] = {
	/* start up */
	{0x03, 0x00}, {0x59, 0x00}, {0x21, 0x10}, {0x5E, 0x18}, {0x60, 0x40}, {0x62, 0x40},  {0x63, 0x02},
	/* Bank A */
	{0x03, 0x00}, {0x09, 0x03}, {0x0B, 0x03}, {0x0C, 0x01}, {0x0D, 0x42}, {0x0E, 0x00}, {0x0F, 0xF2}, {0x36, 0x00}, {0x37, 0x03}, {0x38, 0x01}, {0x39, 0x42},
	{0x3A, 0x00}, {0x3B, 0x03}, {0x3C, 0x00}, {0x3D, 0xF2}, {0x3E, 0x00}, {0x3F, 0x6D}, {0x40, 0x00}, {0x41, 0xD8}, {0x42, 0x00},
	{0x43, 0x53}, {0x44, 0x00}, {0x45, 0xA2}, {0x64, 0x0F}, {0x68, 0x00}, {0x80, 0x00},  {0x81, 0x00}, {0x4E, 0x00}, {0x4F, 0x64},
	{0x50, 0x02}, {0x51, 0x1C}, {0x52, 0x00}, {0x53, 0x32},  {0x54, 0x01}, {0x55, 0xAE}, {0xF6, 0x86},
	{0xF7, 0x85}, {0xF8, 0x84}, {0xF9, 0x00}, {0xFA, 0x00}, {0xFB, 0x00}, {0xFC, 0xFF}, {0xFD, 0x7F}, {0x5C, 0x1D},
	/* Bank B */
	{0x03, 0x01}, {0x04, 0xF7}, {0x06, 0x60}, {0x40, 0x04}, {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00}, {0x5E, 0x30},
	{0x5F, 0x10}, {0x62, 0x45}, {0x63, 0x9C}, {0x64, 0x89}, {0x65, 0x87}, {0x66, 0x30}, {0x67, 0x89}, {0x68, 0x00},
	{0x69, 0x9C}, {0x6A, 0x3C}, {0x6E, 0x03}, {0x6F, 0x0C}, {0x70, 0x19}, {0x71, 0x26}, {0x72, 0x3F}, {0x73, 0x52},
	{0x74, 0x6E}, {0x75, 0x82}, {0x76, 0xA1}, {0x77, 0xB9}, {0x78, 0xCE}, {0x79, 0xE0}, {0x7A, 0xF0}, {0x7B, 0xFF},
	{0x7F, 0x0C}, {0x80, 0x19}, {0x81, 0x22}, {0x82, 0x32}, {0x83, 0x48}, {0x84, 0x5E}, {0x85, 0x79}, {0x86, 0x8F},
	{0x87, 0xB1}, {0x88, 0xCB}, {0x89, 0xDA}, {0x8A, 0xE6}, {0x8B, 0xF2}, {0x8C, 0xFF}, {0xC5, 0x0F}, {0xC6, 0x00},
	{0xC7, 0x40}, {0xC8, 0x00}, {0xCB, 0x04}, {0xCC, 0x04}, {0xCD, 0x04}, {0xCE, 0x10}, {0xCF, 0x1F}, {0xD0, 0x04},
	{0xD3, 0x7F}, {0xD4, 0x7F}, {0xD5, 0x7F}, {0xD6, 0x30}, {0xD7, 0x1F}, {0xD8, 0x1F}, {0xD9, 0x1F}, {0xE6, 0x08},
	{0xF0, 0x00}, {0xF1, 0x83}, {0xF2, 0x1E}, {0xF3, 0x00}, {0xF4, 0x9D}, {0xF5, 0x57},{0x4E, 0x13},{0x05, 0x03}, 
	{0x58, 0x00}, {0xB7, 0x01},{0x93, 0x40}, {0x94, 0x40}, {0x95, 0x01}, {0x96, 0x40},
	/* Bank C */
	{0x03, 0x02}, {0x05, 0x7D}, {0x06, 0x84}, {0x16, 0x02}, {0x17, 0x0C}, {0x18, 0x04}, {0x19, 0x18}, {0x1A, 0x10},
	{0x1B, 0x60}, {0x1C, 0x00}, {0x1D, 0x00}, {0x1E, 0x0C}, {0x1F, 0x01}, {0x20, 0x06}, {0x21, 0x00}, {0x22, 0x01},
	{0x23, 0x06}, {0x24, 0x00}, {0x2B, 0xE6}, {0x2C, 0xE6}, {0x2D, 0x02}, {0x30, 0x20}, {0x31, 0x20}, {0x33, 0x10},
	{0x34, 0x40}, {0x35, 0x80}, {0x36, 0xFF}, {0x3E, 0x20}, {0x3F, 0x30}, {0x3A, 0xAA}, {0x3B, 0x90}, {0x3C, 0x90},
	{0x3D, 0x90}, {0x40, 0x04}, {0x41, 0x08}, {0x42, 0x1A}, {0x43, 0x25}, {0x44, 0x08}, {0x4A, 0x06}, {0x56, 0x26},
	{0x57, 0x40}, {0x58, 0x2F}, {0x59, 0x29}, {0x5A, 0x07}, {0x5B, 0x0C}, {0x5C, 0x9F}, {0x5D, 0x2A}, {0x5E, 0x29},
	{0x5F, 0x50}, {0x60, 0x20}, {0x61, 0x10}, {0x62, 0x09}, {0x63, 0x10}, {0x64, 0xF0}, {0x65, 0x08}, {0x66, 0x10},
	{0x68, 0x15}, {0x69, 0x2B}, {0x78, 0x0A}, {0x79, 0x04}, {0x6C, 0x7E}, {0x6D, 0x7C}, {0x6E, 0x80}, {0x6F, 0x80},
	{0x70, 0x80}, {0x71, 0x80}, {0x7B, 0x48}, {0x7C, 0x34}, {0x7D, 0x60}, {0x7E, 0x68}, {0x7F, 0x48}, {0x80, 0x40},
	{0x81, 0x78}, {0x82, 0x7B}, {0x83, 0x00}, {0x84, 0x08}, {0x85, 0x00}, {0x86, 0x40}, {0x88, 0x3E}, {0x89, 0x8C},
	{0x8A, 0x94}, {0x8B, 0x2C}, {0x8C, 0x21}, {0x8D, 0x84}, {0x8E, 0x04}, {0x8F, 0x27}, {0x90, 0x1D}, {0x91, 0x04},
	{0x92, 0x00}, {0x93, 0x1D}, {0x94, 0x04}, {0x95, 0x00}, {0x96, 0x00}, {0x97, 0x00}, {0x98, 0x04}, {0x99, 0x00},
	{0x9A, 0x34}, {0x9B, 0x55}, {0x9C, 0x64}, {0xA3, 0x00}, {0xA4, 0x00}, {0xA5, 0x00}, {0xA6, 0x10}, {0xA7, 0x18},
	{0xA8, 0x1F}, {0xAA, 0x00}, {0xAB, 0x08}, {0xAC, 0x1A}, {0xAD, 0x28}, {0xAE, 0x2F}, {0xAF, 0x3F}, {0xB1, 0x00},
	{0xB2, 0x08}, {0xB3, 0x10}, {0xB4, 0x18}, {0xB5, 0x3F}, {0xB6, 0x10}, {0xC6, 0x00}, {0xC7, 0x00}, {0xC8, 0x00},
	{0xC9, 0x10}, {0xCA, 0x10}, {0xCB, 0x10}, {0xCD, 0x3F}, {0xCE, 0x3F}, {0xCF, 0x3F}, {0xD0, 0x20}, {0xD1, 0x10},
	{0xD2, 0x00}, {0xD4, 0x3F}, {0xD5, 0x3F}, {0xD6, 0x3F}, {0xD7, 0x20}, {0xD8, 0x10}, {0xD9, 0x00}, {0xDB, 0x00},
	{0xDC, 0x00}, {0xDD, 0x00}, {0xDE, 0x0A}, {0xDF, 0x1A}, {0xE0, 0x00}, {0xE2, 0x00}, {0xE3, 0x04}, {0xE4, 0x08},
	{0xE5, 0x18}, {0xE6, 0x1F}, {0xE7, 0x1F}, {0xE9, 0x00}, {0xEA, 0x00}, {0xEB, 0x04}, {0xEC, 0x0E}, {0xED, 0x18},
	{0xEE, 0x18}, {0xF0, 0x00}, {0xF1, 0x00}, {0xF2, 0x04}, {0xF3, 0x10}, {0xF4, 0x18}, {0xF5, 0x18}, {0xF7, 0x08},
	{0xF8, 0x08}, {0xF9, 0x08}, {0xFA, 0x14}, {0xFB, 0x1C}, {0xFC, 0x08}, {0xB8, 0x00}, {0xB9, 0x00}, {0xBA, 0x00},
};

static SENSOR_DATA_S poa030r_init_para_640_480_60fps[] = {
	/* start up */
	{0x03, 0x00}, {0x59, 0x00}, {0x21, 0x10}, {0x5E, 0x18}, {0x60, 0x40}, {0x62, 0x40},  {0x63, 0x02},
	/* Bank A */
	{0x03, 0x00}, {0x64, 0x0F}, {0x68, 0x00}, {0x80, 0x00},  {0x81, 0x00}, {0x4E, 0x00}, {0x4F, 0x64},
	{0x50, 0x02}, {0x51, 0x1C}, {0x52, 0x00}, {0x53, 0x32},  {0x54, 0x01}, {0x55, 0xAE}, {0xF6, 0x86},
	{0xF7, 0x85}, {0xF8, 0x84}, {0xF9, 0x00}, {0xFA, 0x00}, {0xFB, 0x00}, {0xFC, 0xFF}, {0xFD, 0x7F}, {0x5C, 0x1D},
	/* Bank B */
	{0x03, 0x01}, {0x04, 0x00}, {0x06, 0x60}, {0x40, 0x04}, {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00}, {0x5E, 0x30},
	{0x5F, 0x10}, {0x62, 0x45}, {0x63, 0x9C}, {0x64, 0x89}, {0x65, 0x87}, {0x66, 0x30}, {0x67, 0x89}, {0x68, 0x00},
	{0x69, 0x9C}, {0x6A, 0x3C}, {0x6E, 0x03}, {0x6F, 0x0C}, {0x70, 0x19}, {0x71, 0x26}, {0x72, 0x3F}, {0x73, 0x52},
	{0x74, 0x6E}, {0x75, 0x82}, {0x76, 0xA1}, {0x77, 0xB9}, {0x78, 0xCE}, {0x79, 0xE0}, {0x7A, 0xF0}, {0x7B, 0xFF},
	{0x7F, 0x0C}, {0x80, 0x19}, {0x81, 0x22}, {0x82, 0x32}, {0x83, 0x48}, {0x84, 0x5E}, {0x85, 0x79}, {0x86, 0x8F},
	{0x87, 0xB1}, {0x88, 0xCB}, {0x89, 0xDA}, {0x8A, 0xE6}, {0x8B, 0xF2}, {0x8C, 0xFF}, {0xC5, 0x0F}, {0xC6, 0x00},
	{0xC7, 0x40}, {0xC8, 0x00}, {0xCB, 0x04}, {0xCC, 0x04}, {0xCD, 0x04}, {0xCE, 0x10}, {0xCF, 0x1F}, {0xD0, 0x04},
	{0xD3, 0x7F}, {0xD4, 0x7F}, {0xD5, 0x7F}, {0xD6, 0x30}, {0xD7, 0x1F}, {0xD8, 0x1F}, {0xD9, 0x1F}, {0xE6, 0x08},
	{0xF0, 0x00}, {0xF1, 0x83}, {0xF2, 0x1E}, {0xF3, 0x00}, {0xF4, 0x9D}, {0xF5, 0x57}, {0x4E, 0x13},{0x05, 0x03}, 
	{0x58, 0x01}, {0xB7, 0x00},
	/* Bank C */
	{0x03, 0x02}, {0x05, 0x7D}, {0x06, 0x84}, {0x16, 0x02}, {0x17, 0x0C}, {0x18, 0x04}, {0x19, 0x18}, {0x1A, 0x10},
	{0x1B, 0x60}, {0x1C, 0x00}, {0x1D, 0x00}, {0x1E, 0x0C}, {0x1F, 0x01}, {0x20, 0x06}, {0x21, 0x00}, {0x22, 0x01},
	{0x23, 0x06}, {0x24, 0x00}, {0x2B, 0xE6}, {0x2C, 0xE6}, {0x2D, 0x02}, {0x30, 0x20}, {0x31, 0x20}, {0x33, 0x10},
	{0x34, 0x40}, {0x35, 0x80}, {0x36, 0xFF}, {0x3E, 0x20}, {0x3F, 0x30}, {0x3A, 0xAA}, {0x3B, 0x90}, {0x3C, 0x90},
	{0x3D, 0x90}, {0x40, 0x04}, {0x41, 0x08}, {0x42, 0x1A}, {0x43, 0x25}, {0x44, 0x08}, {0x4A, 0x06}, {0x56, 0x26},
	{0x57, 0x40}, {0x58, 0x2F}, {0x59, 0x29}, {0x5A, 0x07}, {0x5B, 0x0C}, {0x5C, 0x9F}, {0x5D, 0x2A}, {0x5E, 0x29},
	{0x5F, 0x50}, {0x60, 0x20}, {0x61, 0x10}, {0x62, 0x09}, {0x63, 0x10}, {0x64, 0xF0}, {0x65, 0x08}, {0x66, 0x10},
	{0x68, 0x15}, {0x69, 0x2B}, {0x78, 0x0A}, {0x79, 0x04}, {0x6C, 0x7E}, {0x6D, 0x7C}, {0x6E, 0x80}, {0x6F, 0x80},
	{0x70, 0x80}, {0x71, 0x80}, {0x7B, 0x48}, {0x7C, 0x34}, {0x7D, 0x60}, {0x7E, 0x68}, {0x7F, 0x48}, {0x80, 0x40},
	{0x81, 0x78}, {0x82, 0x7B}, {0x83, 0x00}, {0x84, 0x08}, {0x85, 0x00}, {0x86, 0x40}, {0x88, 0x3E}, {0x89, 0x8C},
	{0x8A, 0x94}, {0x8B, 0x2C}, {0x8C, 0x21}, {0x8D, 0x84}, {0x8E, 0x04}, {0x8F, 0x27}, {0x90, 0x1D}, {0x91, 0x04},
	{0x92, 0x00}, {0x93, 0x1D}, {0x94, 0x04}, {0x95, 0x00}, {0x96, 0x00}, {0x97, 0x00}, {0x98, 0x04}, {0x99, 0x00},
	{0x9A, 0x34}, {0x9B, 0x55}, {0x9C, 0x64}, {0xA3, 0x00}, {0xA4, 0x00}, {0xA5, 0x00}, {0xA6, 0x10}, {0xA7, 0x18},
	{0xA8, 0x1F}, {0xAA, 0x00}, {0xAB, 0x08}, {0xAC, 0x1A}, {0xAD, 0x28}, {0xAE, 0x2F}, {0xAF, 0x3F}, {0xB1, 0x00},
	{0xB2, 0x08}, {0xB3, 0x10}, {0xB4, 0x18}, {0xB5, 0x3F}, {0xB6, 0x10}, {0xC6, 0x00}, {0xC7, 0x00}, {0xC8, 0x00},
	{0xC9, 0x10}, {0xCA, 0x10}, {0xCB, 0x10}, {0xCD, 0x3F}, {0xCE, 0x3F}, {0xCF, 0x3F}, {0xD0, 0x20}, {0xD1, 0x10},
	{0xD2, 0x00}, {0xD4, 0x3F}, {0xD5, 0x3F}, {0xD6, 0x3F}, {0xD7, 0x20}, {0xD8, 0x10}, {0xD9, 0x00}, {0xDB, 0x00},
	{0xDC, 0x00}, {0xDD, 0x00}, {0xDE, 0x0A}, {0xDF, 0x1A}, {0xE0, 0x00}, {0xE2, 0x00}, {0xE3, 0x04}, {0xE4, 0x08},
	{0xE5, 0x18}, {0xE6, 0x1F}, {0xE7, 0x1F}, {0xE9, 0x00}, {0xEA, 0x00}, {0xEB, 0x04}, {0xEC, 0x0E}, {0xED, 0x18},
	{0xEE, 0x18}, {0xF0, 0x00}, {0xF1, 0x00}, {0xF2, 0x04}, {0xF3, 0x10}, {0xF4, 0x18}, {0xF5, 0x18}, {0xF7, 0x08},
	{0xF8, 0x08}, {0xF9, 0x08}, {0xFA, 0x14}, {0xFB, 0x1C}, {0xFC, 0x08}, {0xB8, 0x00}, {0xB9, 0x00}, {0xBA, 0x00},
};

static SENSOR_DATA_S poa030r_init_para_320_240_60fps[] = {
	/* start up */
	{0x03, 0x00}, {0x59, 0x00}, {0x21, 0x10}, {0x5E, 0x18}, {0x60, 0x40}, {0x62, 0x40},  {0x63, 0x02},
	/* Bank A */
	{0x03, 0x00}, {0x09, 0x03}, {0x0B, 0x03}, {0x0C, 0x01}, {0x0D, 0x42}, {0x0E, 0x00}, {0x0F, 0xF2}, {0x36, 0x00}, {0x37, 0x03}, {0x38, 0x01}, {0x39, 0x42},
	{0x3A, 0x00}, {0x3B, 0x03}, {0x3C, 0x00}, {0x3D, 0xF2}, {0x3E, 0x00}, {0x3F, 0x6D}, {0x40, 0x00}, {0x41, 0xD8}, {0x42, 0x00},
	{0x43, 0x53}, {0x44, 0x00}, {0x45, 0xA2}, {0x64, 0x0F}, {0x68, 0x00}, {0x80, 0x00},  {0x81, 0x00}, {0x4E, 0x00}, {0x4F, 0x64},
	{0x50, 0x02}, {0x51, 0x1C}, {0x52, 0x00}, {0x53, 0x32},  {0x54, 0x01}, {0x55, 0xAE}, {0xF6, 0x86},
	{0xF7, 0x85}, {0xF8, 0x84}, {0xF9, 0x00}, {0xFA, 0x00}, {0xFB, 0x00}, {0xFC, 0xFF}, {0xFD, 0x7F}, {0x5C, 0x1D},
	/* Bank B */
	{0x03, 0x01}, {0x04, 0xF7}, {0x06, 0x60}, {0x40, 0x04}, {0x41, 0x00}, {0x42, 0x00}, {0x43, 0x00}, {0x5E, 0x30},
	{0x5F, 0x10}, {0x62, 0x45}, {0x63, 0x9C}, {0x64, 0x89}, {0x65, 0x87}, {0x66, 0x30}, {0x67, 0x89}, {0x68, 0x00},
	{0x69, 0x9C}, {0x6A, 0x3C}, {0x6E, 0x03}, {0x6F, 0x0C}, {0x70, 0x19}, {0x71, 0x26}, {0x72, 0x3F}, {0x73, 0x52},
	{0x74, 0x6E}, {0x75, 0x82}, {0x76, 0xA1}, {0x77, 0xB9}, {0x78, 0xCE}, {0x79, 0xE0}, {0x7A, 0xF0}, {0x7B, 0xFF},
	{0x7F, 0x0C}, {0x80, 0x19}, {0x81, 0x22}, {0x82, 0x32}, {0x83, 0x48}, {0x84, 0x5E}, {0x85, 0x79}, {0x86, 0x8F},
	{0x87, 0xB1}, {0x88, 0xCB}, {0x89, 0xDA}, {0x8A, 0xE6}, {0x8B, 0xF2}, {0x8C, 0xFF}, {0xC5, 0x0F}, {0xC6, 0x00},
	{0xC7, 0x40}, {0xC8, 0x00}, {0xCB, 0x04}, {0xCC, 0x04}, {0xCD, 0x04}, {0xCE, 0x10}, {0xCF, 0x1F}, {0xD0, 0x04},
	{0xD3, 0x7F}, {0xD4, 0x7F}, {0xD5, 0x7F}, {0xD6, 0x30}, {0xD7, 0x1F}, {0xD8, 0x1F}, {0xD9, 0x1F}, {0xE6, 0x08},
	{0xF0, 0x00}, {0xF1, 0x83}, {0xF2, 0x1E}, {0xF3, 0x00}, {0xF4, 0x9D}, {0xF5, 0x57},{0x4E, 0x13},{0x05, 0x03}, 
	{0x58, 0x01}, {0xB7, 0x00},{0x93, 0x40}, {0x94, 0x40}, {0x95, 0x01}, {0x96, 0x40}, 
	/* Bank C */
	{0x03, 0x02}, {0x05, 0x7D}, {0x06, 0x84}, {0x16, 0x02}, {0x17, 0x0C}, {0x18, 0x04}, {0x19, 0x18}, {0x1A, 0x10},
	{0x1B, 0x60}, {0x1C, 0x00}, {0x1D, 0x00}, {0x1E, 0x0C}, {0x1F, 0x01}, {0x20, 0x06}, {0x21, 0x00}, {0x22, 0x01},
	{0x23, 0x06}, {0x24, 0x00}, {0x2B, 0xE6}, {0x2C, 0xE6}, {0x2D, 0x02}, {0x30, 0x20}, {0x31, 0x20}, {0x33, 0x10},
	{0x34, 0x40}, {0x35, 0x80}, {0x36, 0xFF}, {0x3E, 0x20}, {0x3F, 0x30}, {0x3A, 0xAA}, {0x3B, 0x90}, {0x3C, 0x90},
	{0x3D, 0x90}, {0x40, 0x04}, {0x41, 0x08}, {0x42, 0x1A}, {0x43, 0x25}, {0x44, 0x08}, {0x4A, 0x06}, {0x56, 0x26},
	{0x57, 0x40}, {0x58, 0x2F}, {0x59, 0x29}, {0x5A, 0x07}, {0x5B, 0x0C}, {0x5C, 0x9F}, {0x5D, 0x2A}, {0x5E, 0x29},
	{0x5F, 0x50}, {0x60, 0x20}, {0x61, 0x10}, {0x62, 0x09}, {0x63, 0x10}, {0x64, 0xF0}, {0x65, 0x08}, {0x66, 0x10},
	{0x68, 0x15}, {0x69, 0x2B}, {0x78, 0x0A}, {0x79, 0x04}, {0x6C, 0x7E}, {0x6D, 0x7C}, {0x6E, 0x80}, {0x6F, 0x80},
	{0x70, 0x80}, {0x71, 0x80}, {0x7B, 0x48}, {0x7C, 0x34}, {0x7D, 0x60}, {0x7E, 0x68}, {0x7F, 0x48}, {0x80, 0x40},
	{0x81, 0x78}, {0x82, 0x7B}, {0x83, 0x00}, {0x84, 0x08}, {0x85, 0x00}, {0x86, 0x40}, {0x88, 0x3E}, {0x89, 0x8C},
	{0x8A, 0x94}, {0x8B, 0x2C}, {0x8C, 0x21}, {0x8D, 0x84}, {0x8E, 0x04}, {0x8F, 0x27}, {0x90, 0x1D}, {0x91, 0x04},
	{0x92, 0x00}, {0x93, 0x1D}, {0x94, 0x04}, {0x95, 0x00}, {0x96, 0x00}, {0x97, 0x00}, {0x98, 0x04}, {0x99, 0x00},
	{0x9A, 0x34}, {0x9B, 0x55}, {0x9C, 0x64}, {0xA3, 0x00}, {0xA4, 0x00}, {0xA5, 0x00}, {0xA6, 0x10}, {0xA7, 0x18},
	{0xA8, 0x1F}, {0xAA, 0x00}, {0xAB, 0x08}, {0xAC, 0x1A}, {0xAD, 0x28}, {0xAE, 0x2F}, {0xAF, 0x3F}, {0xB1, 0x00},
	{0xB2, 0x08}, {0xB3, 0x10}, {0xB4, 0x18}, {0xB5, 0x3F}, {0xB6, 0x10}, {0xC6, 0x00}, {0xC7, 0x00}, {0xC8, 0x00},
	{0xC9, 0x10}, {0xCA, 0x10}, {0xCB, 0x10}, {0xCD, 0x3F}, {0xCE, 0x3F}, {0xCF, 0x3F}, {0xD0, 0x20}, {0xD1, 0x10},
	{0xD2, 0x00}, {0xD4, 0x3F}, {0xD5, 0x3F}, {0xD6, 0x3F}, {0xD7, 0x20}, {0xD8, 0x10}, {0xD9, 0x00}, {0xDB, 0x00},
	{0xDC, 0x00}, {0xDD, 0x00}, {0xDE, 0x0A}, {0xDF, 0x1A}, {0xE0, 0x00}, {0xE2, 0x00}, {0xE3, 0x04}, {0xE4, 0x08},
	{0xE5, 0x18}, {0xE6, 0x1F}, {0xE7, 0x1F}, {0xE9, 0x00}, {0xEA, 0x00}, {0xEB, 0x04}, {0xEC, 0x0E}, {0xED, 0x18},
	{0xEE, 0x18}, {0xF0, 0x00}, {0xF1, 0x00}, {0xF2, 0x04}, {0xF3, 0x10}, {0xF4, 0x18}, {0xF5, 0x18}, {0xF7, 0x08},
	{0xF8, 0x08}, {0xF9, 0x08}, {0xFA, 0x14}, {0xFB, 0x1C}, {0xFC, 0x08}, {0xB8, 0x00}, {0xB9, 0x00}, {0xBA, 0x00},
};



static int i2c_minor = 0;

/* download poa030r settings to sensor through i2c */
static int poa030r_download_firmware(SENSOR_DATA_S *pModeSetting, int ArySize)
{
	int i, ret;
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;

	for(i = 0; i < ArySize; ++i, ++pModeSetting){
		i2c_para.reg_addr = pModeSetting->reg_addr;
		i2c_para.data = pModeSetting->data;
		ret = hi_i2c_write(i2c_minor, &i2c_para);
		if(ret < 0){
			printk("i2c write addr=0x%x val=0x%x error!\n", i2c_para.reg_addr, i2c_para.data);
			return ret;
		}
	}

	return ret;
}

int sensor_poa030r_init(SENSOR_RES_E mode)
{
	SENSOR_DATA_S *pModeSetting = NULL;
	int ArySize = 0, ret = 0;

	hi_i2c_init(i2c_minor);

	switch(mode){
		case MODE_640x480_30FPS:
			pModeSetting = poa030r_init_para_640_480_30fps;
			ArySize = ARRAY_SIZE(poa030r_init_para_640_480_30fps);
			break;
		case MODE_640x480_60FPS:
			pModeSetting = poa030r_init_para_640_480_60fps;
			ArySize = ARRAY_SIZE(poa030r_init_para_640_480_60fps);
			break;
		case MODE_320x240_30FPS:
			pModeSetting = poa030r_init_para_320_240_30fps;
			ArySize = ARRAY_SIZE(poa030r_init_para_320_240_30fps);
			break;
		case MODE_320x240_60FPS:
			pModeSetting = poa030r_init_para_320_240_60fps;
			ArySize = ARRAY_SIZE(poa030r_init_para_320_240_60fps);
			break;
		default:
			printk("invalid sensor mode, please check again!\n");
			return -1;
	}
	ret = poa030r_download_firmware(pModeSetting, ArySize);
	if (ret < 0){
		printk("poa030r_download_firmware failed! try again\n");
		return -1;
	}

	return 0;
}

int sensor_poa030r_read(SENSOR_DATA_S *sensor_data)
{
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_addr = sensor_data->reg_addr;
	i2c_para.reg_byte = 1;
	i2c_para.data_byte = 1;
	hi_i2c_read(i2c_minor, &i2c_para);

	return i2c_para.data;
}


int sensor_poa030r_write(SENSOR_DATA_S *sensor_data)
{
	i2c_para_s i2c_para;

	i2c_para.dev_addr = SENSOR_I2C_ADDR;
	i2c_para.reg_addr = sensor_data->reg_addr;
	i2c_para.reg_byte = 1;
	i2c_para.data = sensor_data->data;
	i2c_para.data_byte = 1;

	return hi_i2c_write(i2c_minor, &i2c_para);
} 

