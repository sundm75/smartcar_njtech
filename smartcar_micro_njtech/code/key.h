/*
 * key.h
 *
 *  Created on: 2023Äê10ÔÂ19ÈÕ
 *      Author: sundm
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_
#include "zf_common_headfile.h"
#include "zf_common_typedef.h"

void usr_key_init(void);
void key_Handle();
int get_key_value(void)  ;
int get_sw_value(void);
int get_hall_value(void);
uint8 get_is_editing_flag(void);


#endif /* CODE_KEY_H_ */
