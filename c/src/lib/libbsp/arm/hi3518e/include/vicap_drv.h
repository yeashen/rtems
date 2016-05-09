#ifndef _VICAP_DRV_H
#define _VICAP_DRV_H

#include <platform.h>

void vicap_set_y_faddr(unsigned int addr);

void vicap_set_y_width(unsigned int width);

void vicap_set_y_height(unsigned int height);

void vicap_set_y_stride(unsigned int stride);

void vicap_set_c_faddr(unsigned int addr);

void vicap_set_c_width(unsigned int width);

void vicap_set_c_height(unsigned int height);

void vicap_set_c_stride(unsigned int stride);

void vicap_set_hact(unsigned int hact);

void vicap_set_vact(unsigned int vact);

void vicap_set_crop_width(unsigned int width);

void vicap_set_crop_height(unsigned int height);

void vicap_set_pack_y_width(unsigned int width);

void vicap_set_pack_c_width(unsigned int width);


unsigned int vicap_get_ch_int_status(void);

void vicap_reg_newer(void);

unsigned int vicap_get_pt_height();

unsigned int vicap_get_pt_width();

void vicap_pin_init();

void vicap_reg_init(void);

static inline void vicap_clear_src_int(){
	INT_WR_REG(REG_INTC_SOFTINTCLEAR, CFG_VICAP_IRQEN);
};

static inline void vicap_clear_ch_int(unsigned int int_mask){
	VICAP_WR_REG(VICAP_CH_INT_STA, int_mask);
};


#endif /* _VICAP_DRV_H */
