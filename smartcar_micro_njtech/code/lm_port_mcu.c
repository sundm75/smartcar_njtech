/*
 * 提供 `lm_port.h` 里那几个实现
 */

// 2个 out inmge

#include "lm_port.h"

#define GRAYTORGB16(t) ((t >> 3)|((t & ~3) << 3)|((t & ~7) << 8)) // 压缩彩色

uint8 gray_image[LM_OUT_IMAGE_H][LM_OUT_IMAGE_W];
static uint8* const s_out_base =
    &gray_image[LM_OUT_IMAGE_Y][LM_OUT_IMAGE_X];


void LM_OutImageSet (int x, int y, uint8 value)
{
    if (x >= 0 && x < LM_OUT_IMAGE_W && y >= 0 && y < LM_OUT_IMAGE_H)
    {
        s_out_base[(size_t)y * LM_IMAGE_W + x] = (value);
    }
}

uint8 LM_OutImageGet (int x, int y)
{
    if (x >= 0 && x < LM_OUT_IMAGE_W && y >= 0 && y < LM_OUT_IMAGE_H)
    {
        return (uint8)s_out_base[(size_t)y * LM_IMAGE_W + x];
    }
    return 0;
}


//3个 draw
void        LM_DrawPoint              (int x, int y, unsigned int color)     // 画点
{

}
void        LM_DrawLine               (int x0, int y0, int x1, int y1,
                                         unsigned int color)                   // 画线（斜线也行）
{

}
void        LM_DrawRect               (int x0, int y0, int x1, int y1,
                                         unsigned int color)                   // 画矩形边框
{

}

//3个 log
#include <stdarg.h>   // 必须，va系列宏

void LM_Log(LM_log_level_enum level, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    // 直接输出，不带级别前缀
    vprintf(fmt, ap);

    va_end(ap);
}

void        LM_LogAt                  (int x, int y, LM_log_level_enum level, const char* fmt, ...)
{

}
void        LM_LogClear               (void)
{

}

