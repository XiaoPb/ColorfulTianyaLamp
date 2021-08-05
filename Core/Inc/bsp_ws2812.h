#ifndef __BSP_WS2812_H__
#define __BSP_WS2812_H__

#include "main.h"
#include "stdint.h"

#define WS2812_DEFAULT_NUM 7

typedef struct _ws2812_color{
    uint32_t g[8];
    uint32_t r[8];
    uint32_t b[8];
}ws2812_color;


typedef struct _ws2812_control{
    ws2812_color* ws2812_data;
    uint32_t*     ws2812_send_data;
    uint32_t ws2812_num;
    uint32_t data_bytes;
    TIM_HandleTypeDef *htim; 
    uint32_t Channel;
    
    void (*ws2812_color_updata_all)(uint32_t, struct _ws2812_control*);
    void (*ws2812_colors_updata)(uint32_t*, uint32_t, uint32_t, struct _ws2812_control*);
    void (*ws2812_color_updata_index)(uint32_t, uint32_t, struct _ws2812_control*);
    void (*ws2812_updata)(struct _ws2812_control*);
}ws2812_control;

typedef ws2812_control* p_ws2812_control;

#define _WS2812_USER_STRCT(x)                    \
     typedef struct _ws2812_bytes##x {           \
        uint32_t rst_1[41];                      \
        ws2812_color ws2812[x + 1];              \
        uint32_t rst_2[42];                      \
      }ws2812_bytes##x;                          \
    extern ws2812_bytes##x user_ws2812_data##x ; \
    extern ws2812_control user_ws2812_control##x

#define WS2812_USER_STRCT(x)  _WS2812_USER_STRCT(x)
    
#define _WS2812_USER_TYPE(x)                     \
    ws2812_bytes##x user_ws2812_data##x = {      \
    .rst_1[0]  = 1,                              \
    .rst_2[0]  = 1,                              \
    .rst_2[41] = 25};                            \
    ws2812_control  user_ws2812_control##x = {   \
    .ws2812_num  = x,                            \
    .ws2812_send_data  = (uint32_t*)&user_ws2812_data##x, \
    .ws2812_data = user_ws2812_data##x.ws2812}
    
#define WS2812_USER_TYPE(x)  _WS2812_USER_TYPE(x)    

#define WS2812_GRB(g,r,b) (((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | ((uint32_t)(b)))    
    
WS2812_USER_STRCT(WS2812_DEFAULT_NUM);

void ws2812_init(p_ws2812_control user_control, TIM_HandleTypeDef *timx, uint32_t Channelx);
void ws2812_color_updata_all(uint32_t color,p_ws2812_control user_control);
void ws2812_colors_updata(uint32_t color[], uint32_t start_index, 
                    uint32_t len, p_ws2812_control user_control);
void ws2812_color_updata_index(uint32_t color, uint32_t index, p_ws2812_control user_control);
void ws2812_updata(p_ws2812_control user_control);

#endif
