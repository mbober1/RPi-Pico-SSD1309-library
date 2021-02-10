#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../logo.hpp"
#include "../GFX.hpp"


#define RESET_PIN 17

int main() {

    //setup
    stdio_init_all();
    i2c_init(i2c1, 400000);                 //Initialize I2C on i2c1 port with 400kHz
    gpio_set_function(2, GPIO_FUNC_I2C);    //Use GPIO2 as I2C
    gpio_set_function(3, GPIO_FUNC_I2C);    //Use GPIO3 as I2C
    gpio_pull_up(2);                        //Pull up GPIO2
    gpio_pull_up(3);                        //Pull up GPIO3

    GFX oled(0x3C, 128, 64, i2c1, RESET_PIN); //Declare oled instance

    oled.display(logo);
    
    sleep_ms(1000);        

    oled.clear();
    oled.drawString(22, 0, "Raspberry Pico");
    oled.drawString(18, 16, "SSD1309 library");
    oled.drawString(10, 35, "github.com/mbober1");
    oled.drawProgressBar(4, oled.getHeight()-10, oled.getWidth() -4, 10, 69);
    oled.display();
    oled.scrollHorizontalRight(0, 127, 3, 5, scrollInterval::FRAMES_1);

    return 0;
}