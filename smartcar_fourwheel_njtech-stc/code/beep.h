/*
 * beep.h
 *
 *  Created on: 2026-3-09
 *      Author: sundm
 */

#ifndef CODE_BEEP_H_
#define CODE_BEEP_H_

#include "zf_common_headfile.h"

void beep_init(void);
void beep_handle(void);
void beep(uint32 nums, uint32 period, uint32 percent);

#endif /* CODE_BEEP_H_ */
