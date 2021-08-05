#include "bsp_ws2812.h"                  // Device header

WS2812_USER_TYPE(WS2812_DEFAULT_NUM);


void ws2812_init(p_ws2812_control user_control,TIM_HandleTypeDef *timx, uint32_t Channelx)
{
    user_control->data_bytes       = (user_control->ws2812_num + 1) * 24 + 82;
    user_control->htim    = timx;
    user_control->Channel = Channelx;
    
    user_control->ws2812_color_updata_all   = ws2812_color_updata_all;
    user_control->ws2812_color_updata_index = ws2812_color_updata_index;
    user_control->ws2812_colors_updata      = ws2812_colors_updata;
    user_control->ws2812_updata             = ws2812_updata;
    
    user_control->ws2812_color_updata_all(0x000000,user_control);
    
    for(uint8_t i = 0; i < 8; i++)
    {
        user_control->ws2812_data[user_control->ws2812_num].g[i] = 8;
        user_control->ws2812_data[user_control->ws2812_num].r[i] = 8;
        user_control->ws2812_data[user_control->ws2812_num].b[i] = 8; 
    }
}

void ws2812_color_updata_all(uint32_t color,p_ws2812_control user_control)
{
    uint8_t g = (color >> 16) & 0xff;
    uint8_t r = (color >> 8) & 0xff;
    uint8_t b = (color) & 0xff;

    for(uint8_t index = 0; index < user_control->ws2812_num; index++)
    {
        for(uint8_t i = 0; i < 8; i++)
        {
            user_control->ws2812_data[index].g[i] = (((g & (0x80 >> i)) != 0)? (17) : (8));
            user_control->ws2812_data[index].r[i] = (((r & (0x80 >> i)) != 0)? (17) : (8));
            user_control->ws2812_data[index].b[i] = (((b & (0x80 >> i)) != 0)? (17) : (8)); 
        }
    }
}

void ws2812_colors_updata(uint32_t color[], uint32_t start_index, uint32_t len, p_ws2812_control user_control)
{
    uint8_t g, r, b;
    len = ((len > (user_control->ws2812_num - start_index)) ? (user_control->ws2812_num) : (start_index + len));
    for(uint8_t index = start_index; index < len; index++)
    {
            g = (color[index - start_index] >> 16) & 0xff;
            r = (color[index - start_index] >> 8) & 0xff;
            b = (color[index - start_index]) & 0xff;
        for(uint8_t i = 0; i < 8; i++)
        {
            user_control->ws2812_data[index].g[i] = (((g & (0x80 >> i)) != 0)? (17) : (8));
            user_control->ws2812_data[index].r[i] = (((r & (0x80 >> i)) != 0)? (17) : (8));
            user_control->ws2812_data[index].b[i] = (((b & (0x80 >> i)) != 0)? (17) : (8)); 
        }
    }
}

void ws2812_color_updata_index(uint32_t color, uint32_t index, p_ws2812_control user_control)
{
    uint8_t g = (color >> 16) & 0xff;
    uint8_t r = (color >> 8) & 0xff;
    uint8_t b = (color) & 0xff;
    
    if((index > user_control->ws2812_num) || (index == 0))
    {
        return;
    }
    index -= 1;
    for(uint8_t i = 0; i < 8; i++)
    {
        user_control->ws2812_data[index].g[i] = (((g & (0x80 >> i)) != 0)? (17) : (8));
        user_control->ws2812_data[index].r[i] = (((r & (0x80 >> i)) != 0)? (17) : (8));
        user_control->ws2812_data[index].b[i] = (((b & (0x80 >> i)) != 0)? (17) : (8)); 
    }
}

void ws2812_updata(p_ws2812_control user_control)
{
    HAL_TIM_PWM_Stop_DMA(user_control->htim,user_control->Channel);
    HAL_TIM_PWM_Start_DMA(user_control->htim,user_control->Channel,user_control->ws2812_send_data,user_control->data_bytes);    
}

