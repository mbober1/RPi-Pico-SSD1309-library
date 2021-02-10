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

    oled.display(logo);                     //Display bitmap

    while(true) 
    {
        sleep_ms(1000);
        oled.clear();                       //Clear buffer
        oled.drawString(0, 0, "Raspberry Pico");
        oled.drawString(0, 10, "Oled Example");
        oled.drawString(0, 20, "Have fun!");
        oled.drawProgressBar(0, oled.getHeight()-10, oled.getWidth(), 10, rand() % 100 + 1);
        oled.display();                     //Send buffer to the screen
    }
    return 0;
}