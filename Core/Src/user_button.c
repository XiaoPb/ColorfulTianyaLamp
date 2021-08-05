#include "wifi.h"                  // Device header


Button user_button;

static uint8_t light_state = 0;
static uint8_t light_color = 0;

static uint8_t light_rgb[][3] =
{
    {255,255,255},
    {255,0,0},
    {0,255,0},
    {0,0,255},
    {255,255,0},
    {255,0,255},
    {0,255,255},
};
    
uint8_t user_button_value(void)
{
    return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
}

void user_button_single(void* btn)
{
    switch(light_state)
    {
        case 0:{
            if(user_light_control.light_sw == 1)
            {
                user_light_control.light_sw = 0;
            }
            else
            {
                user_light_control.light_sw = 1;
            }
        }break;
        case 1:{
            if( user_light_control.light_bright == 100)
            {
                user_light_control.light_bright = 5;
            }
            else if( user_light_control.light_bright >= 90)
            {
                user_light_control.light_bright = 100;
            }
            else
            {
                user_light_control.light_bright += 10;
            }
        }break; 
        case 2:{
            light_color ++;
            if(light_color == 7)
            {
                light_color = 0;
            }
            user_light_control.new_color_rgb[0] = light_rgb[light_color][0];
            user_light_control.new_color_rgb[1] = light_rgb[light_color][1];
            user_light_control.new_color_rgb[2] = light_rgb[light_color][2];
        }break;
    }

    user_light_control.light_flag = 1;
    all_data_update();
}

void user_button_double(void* btn)
{
    light_state ++;
    if(light_state == 3)
    {
        light_state = 0;
    }
    
    if(light_state != 0)
    {
        user_light_control.light_mode = (light_state - 1);
        if(user_light_control.light_mode == 1)
        {
            user_light_control.new_color_rgb[0] = light_rgb[light_color][0];
            user_light_control.new_color_rgb[1] = light_rgb[light_color][1];
            user_light_control.new_color_rgb[2] = light_rgb[light_color][2];
        }
    }
    user_light_control.light_flag = 1;
    all_data_update();
}

void user_button_long(void* btn)
{
    mcu_set_wifi_mode(1);
}

void user_button_init(void)
{
    button_init(&user_button,user_button_value,0);
    
    button_attach(&user_button,SINGLE_CLICK,user_button_single);
    button_attach(&user_button,DOUBLE_CLICK,user_button_double);
    button_attach(&user_button,LONG_PRESS_START,user_button_long);
    
    button_start(&user_button);
}
