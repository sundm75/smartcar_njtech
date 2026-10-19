/*
 * ui.h
 *
 *  Created on: 2026Äê5ÔÂ7ÈÕ
 *      Author: sundm
 */

#ifndef CODE_UI_H_
#define CODE_UI_H_
#include "zf_common_headfile.h"

typedef struct MenuItems
{
    float step_level;
    float max_value;
    float min_value;
    uint8 *title;
} MenuItem;

int get_param_index(void);
void display_Handle(void);
void key_process(void);

void SMARTCAR_Init(void);


#endif /* CODE_UI_H_ */