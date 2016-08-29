/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : vicap_drv.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/04/19
  Last Modified : 
  Description   :  Hi3518e video capture interface driver
  History       : 
  1.Date        : 2016/04/19
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include <hi3518e.h>
#include <vicap_drv.h>

static volatile hi_vi_regs_s * volatile vi_reg = NULL;
//static hi_vi_regs_s *vi_reg = NULL;
static volatile hi_pinmux_regs_s * volatile pinmux_reg = NULL;
//static hi_pinmux_regs_s *pinmux_reg = NULL;

#ifdef HI3518EV100
void vicap_set_y_faddr(unsigned int addr)
{
	vi_reg->des_y_addr = addr;
}

void vicap_set_y_width(unsigned int width)
{
	unsigned int val = 0, w;

	w= width*16/128-1;
	
	val = vi_reg->des_y_size;
	val = (val&0xFFFFF000)|(w&0xFFF);
	vi_reg->des_y_size = val;
}

void vicap_set_y_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->des_y_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->des_y_size = val;
}

void vicap_set_y_stride(unsigned int stride)
{
	vi_reg->des_y_stride = stride;
}

void vicap_set_c_faddr(unsigned int addr)
{
	vi_reg->des_c_addr = addr;
}

void vicap_set_c_width(unsigned int width)
{
	unsigned int val = 0, w;

	w= width*16/128-1;

	val = vi_reg->des_c_size;
	val = (val&0xFFFFF000)|(w&0xFFF);
	vi_reg->des_c_size = val;
}

void vicap_set_c_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->des_c_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->des_c_size = val;
}

void vicap_set_c_stride(unsigned int stride)
{
	vi_reg->des_c_stride = stride;
}

void vicap_set_hact(unsigned int hact)
{
	vi_reg->hact = hact;
}

void vicap_set_vact(unsigned int vact)
{
	vi_reg->vact = vact;
}

void vicap_set_crop_width(unsigned int width)
{
	unsigned int val = 0;
	
	val = vi_reg->crop_size;
	val = (val&0xFFFF0000)|(width&0xFFFF);
	vi_reg->crop_size = val;
}

void vicap_set_crop_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->crop_size;
	val = (val&0x0000FFFF)|((height&0xFFFF)<<16);
	vi_reg->crop_size = val;
}

void vicap_set_pack_y_width(unsigned int width)
{
	vi_reg->pack_y_width = width;
}

void vicap_set_pack_c_width(unsigned int width)
{
	vi_reg->pack_c_width = width;
}

void vicap_reg_newer()
{
	vi_reg->ch_newer = 0x1;
}

unsigned int vicap_get_ch_int_en()
{
	return vi_reg->ch_int_en;
}

unsigned int vicap_get_ch_int_status()
{
	return vi_reg->ch_int;
}

unsigned int vicap_get_pt_height()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF0000)>>16;
}

unsigned int vicap_get_pt_width()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF);
}


void vicap_clear_ch_int(unsigned int int_mask)
{
	vi_reg->ch_int = int_mask;
}

#else defined HI3518EV200
void vicap_set_y_faddr(unsigned int addr)
{
	vi_reg->ch_y_faddr = addr;
}

void vicap_set_y_width(unsigned int width)
{
	unsigned int val = 0;

	val = vi_reg->ch_y_size;
	val = ((val&0xFFFFFE000)|((width-1)&0x1FFF));
	vi_reg->ch_y_size = val;
	
	val = vi_reg->lb_width;
	val = ((val&0xF000F000)|((width-1)&0xFFF)|(((width-1)&0xFFF)<<16));
	vi_reg->lb_width = val;
}


void vicap_set_y_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->ch_y_size;
	val = ((val&0xE000FFFF)|((height&0x1FFF)<<16));
	vi_reg->ch_y_size = val;

	val = vi_reg->lb_height;
	val = ((val&0xFFFFF000)|(height&0xFFF));
	vi_reg->lb_height = val;
}


void vicap_set_y_stride(unsigned int stride)
{
	vi_reg->ch_y_stride = stride;
}

void vicap_set_c_faddr(unsigned int addr)
{
	vi_reg->ch_c_faddr = addr;
}

void vicap_set_c_width(unsigned int width)
{
	unsigned int val = 0;

	val = vi_reg->ch_c_size;
	val = (val&0xFFFFFE000)|((width-1)&0x1FFF);
	vi_reg->ch_c_size = val;
}

void vicap_set_c_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->ch_c_size;
	val = (val&0xE000FFFF)|((height&0x1FFF)<<16);
	vi_reg->ch_c_size = val;
}

void vicap_set_c_stride(unsigned int stride)
{
	vi_reg->ch_c_stride = stride;
}
void vicap_set_hact(unsigned int hact)
{
	vi_reg->hact = hact;
}

void vicap_set_vact(unsigned int vact)
{
	vi_reg->vact = vact;
}

void vicap_set_crop_width(unsigned int width)
{
	unsigned int val = 0;
	
	val = vi_reg->des_crop_size;
	val = (val&0xFFFFFE000)|(width&0x1FFF);
	vi_reg->des_crop_size = val;

	val = vi_reg->lb_crop_size;
	val = (val&0xFFFFFE000)|(width&0x1FFF);
	vi_reg->lb_crop_size = val;
}


void vicap_set_crop_height(unsigned int height)
{
	unsigned int val = 0;

	val = vi_reg->des_crop_size;
	val = (val&0xE000FFFF)|((height&0x1FFF)<<16);
	vi_reg->des_crop_size = val;

	val = vi_reg->lb_crop_size;
	val = (val&0xE000FFFF)|((height&0x1FFF)<<16);
	vi_reg->lb_crop_size = val;
}


void vicap_set_pack_y_width(unsigned int width)
{
	
}

void vicap_set_pack_c_width(unsigned int width)
{
	
}

void vicap_reg_newer()
{
	vi_reg->lb_newer = 0x1;
}

unsigned int vicap_get_ch_int_en()
{
	
}

unsigned int vicap_get_ch_int_status()
{
	return vi_reg->ch_int;
}

unsigned int vicap_get_pt_height()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF0000)>>16;
}

unsigned int vicap_get_pt_width()
{
	unsigned int val;

	val = vi_reg->pt_size;
	
	return (val&0xFFFF);
}


void vicap_clear_ch_int(unsigned int int_mask)
{
	vi_reg->ch_int = int_mask;
}

static void mipi_phy_init()
{
	HI_REG_WR(0x20670008, 0x1);
	HI_REG_WR(0x20670008, 0x0);
	HI_REG_WR(0x2068101C, HI_REG_RD(0x2068101C) | (1<<0));
	usleep(20);
	HI_REG_WR(0x2068101C, HI_REG_RD(0x2068101C) | (1<<4));
	HI_REG_WR(0x20681034, HI_REG_RD(0x20681034) | (1<<0));
	HI_REG_WR(0x20120004, (HI_REG_RD(0x20120004) & ~(0x3<<20)) | (2<<20));
	HI_REG_WR(0x20670004, 0x1);
	usleep(10000);
	HI_REG_WR(0x20670004, 0x0);
	HI_REG_WR(0x20681018, (HI_REG_RD(0x20681018) & (~0x7)) | 0x4);
	HI_REG_WR(0x20681028, ((HI_REG_RD(0x20681028) & (~(1<<8))) & (~(1<<12))) | (1<<8));
	HI_REG_WR(0x20680000, HI_REG_RD(0x20680000) | (1<<0));
}
#endif

void vicap_pin_init(sensor_type_e sns_type)
{
	pinmux_reg =  (hi_pinmux_regs_s *)PINMUX_REG_BASE;
#ifdef HI3518EV100
	/* VICAP clk */
	HI_REG_WR(CRG_REG_BASE+CRG_VICAP_CLK,  0xa);//VICAP, unreset & clock enable

	pinmux_reg->vi_clk = 0x0;
	pinmux_reg->vi_vs = 0x0;
	pinmux_reg->vi_hs = 0x0;
	if(sns_type == SENSOR_OV9712){
		pinmux_reg->vi_dat11 = 0x0;
		pinmux_reg->vi_dat10 = 0x0;
		pinmux_reg->vi_dat9 = 0x0;
		pinmux_reg->vi_dat8 = 0x0;
		pinmux_reg->vi_dat7 = 0x0;
		pinmux_reg->vi_dat6 = 0x0;
		pinmux_reg->vi_dat5 = 0x0;
		pinmux_reg->vi_dat4 = 0x0;
		pinmux_reg->vi_dat3 = 0x0;
		pinmux_reg->vi_dat2 = 0x0;
		pinmux_reg->vi_dat1 = 0x0;
		pinmux_reg->vi_dat0 = 0x0;
	}
	else if(sns_type == SENSOR_POA030R){
		pinmux_reg->vi_dat8 = 0x0;
		pinmux_reg->vi_dat7 = 0x0;
		pinmux_reg->vi_dat6 = 0x0;
		pinmux_reg->vi_dat5 = 0x0;
		pinmux_reg->vi_dat4 = 0x0;
		pinmux_reg->vi_dat3 = 0x0;
		pinmux_reg->vi_dat2 = 0x0;
		pinmux_reg->vi_dat1 = 0x0;
		pinmux_reg->vi_dat0 = 0x0;

		/* special pin */
		pinmux_reg->spi0_sdi = 0x0;
		pinmux_reg->gpio0_7 = 0x1;
	}
#else defined HI3518EV200
	/* special pin */
	pinmux_reg->sns_rstn = 0x1;	//GPIO0_5
	pinmux_reg->spi0_sdi = 0x0;		//GPIO3_5
	pinmux_reg->gpio1_4 = 0x2;		//vi  vs
	pinmux_reg->gpio1_5 = 0x2;		//vi hs

	mipi_phy_init();
#endif
}

void vicap_reg_init(sensor_type_e sns_type)
{
	vi_reg = (hi_vi_regs_s *)VI_REG_BASE;
	
#ifdef HI3518EV100
	/* relation reg config start*/
	vi_reg->lowpower = 0x1;
	vi_reg->apb = 0x80000100;
	if(sns_type == SENSOR_OV9712){
		vi_reg->pt_offset0 = 0xFFC00000;
		vi_reg->pt_offset1 = 0xFFF00010;
		vi_reg->pt_offset2 = 0xFFF00020;
	}
	else if(sns_type == SENSOR_POA030R){
		vi_reg->pt_offset0 = 0xFF000000;
		vi_reg->pt_offset1 = 0xFFF00010;
		vi_reg->pt_offset2 = 0xFFF00020;		
	}
	vi_reg->timing_cfg = 0x82001;
	vi_reg->data_cfg = 0x4;
	vi_reg->hfb = 0x0;//0x198;
	vi_reg->hbb = 0x0;
	vi_reg->vfb = 0x0;//0x6;
	vi_reg->vbb = 0x0;//0x6;
	vi_reg->vbfb = 0x0;
	vi_reg->vbact = 0x0;
	vi_reg->vbbb = 0x0;
	vi_reg->adapter = 0x3;
	vi_reg->pack_y = 0x10;
	vi_reg->crop_cfg = 0x1;
	/* relation reg config end */

	/* config AXI_CFG */
	vi_reg->axi = 0x3;

	/* port config */
	vi_reg->pt_sel = 0x0;

	/* channel config */
	vi_reg->ch_sel = 0x0;
	vi_reg->ch_int_en = CC_INT_EN|FSTART_EN;

	/* vicap global interrupt config */
	vi_reg->g_int_en = CH0_INT_EN;

	/* interface config */
	vi_reg->pt_mode = PT_EN;

	/* enable channel */
	vi_reg->ch_ctrl = CH_EN;
#else defined HI3518EV200
	/* axi_bus */
	vi_reg->axi = 0x10103030;
	vi_reg->vi_int_en = 0x2;
	vi_reg->pt_mode = PT_EN;
	vi_reg->pt_int_en = 0x0;
	vi_reg->pt_offset0 = 0xFFC00000;
	vi_reg->pt_offset1 = 0xFFF00010;
	vi_reg->pt_offset2 = 0xFFF00020;
	vi_reg->timing_cfg = 0x82001;
	vi_reg->data_cfg = 0x4;
	vi_reg->hfb = 0x0;
	vi_reg->hbb = 0x0;
	vi_reg->vfb = 0x0;
	vi_reg->vbb = 0x0;
	vi_reg->vbfb = 0x0;
	vi_reg->vbact = 0x0;
	vi_reg->vbbb = 0x0;
	vi_reg->des_crop_cfg = 0x1;
	vi_reg->buf_mode = 0x0;
	vi_reg->lb_cfg = 0x80000000;
	vi_reg->lb_id = 0x1;
	vi_reg->lb_crop_cfg = 0x1;
	vi_reg->ch_y_cfg = 0x80000000;
	vi_reg->ch_c_cfg = 0x80000000;
	vi_reg->ch_int_en = FSTART_EN|CC_INT_EN;
#endif
}