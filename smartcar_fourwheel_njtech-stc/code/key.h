/*
 * key.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#ifndef _KEY_H
#define _KEY_H

#include "zf_common_headfile.h"
void key_init(void);
void key_Handle(void);
int get_key_value(void);
uint8 get_is_editing_flag(void);
void set_key_value(uint8 val);


#endif
