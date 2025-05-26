#pragma once

#define I2C_MASTER_SCL_IO    GPIO_NUM_14
#define I2C_MASTER_SDA_IO    GPIO_NUM_15
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   100000
#define LCD_I2C_ADDR         0x27  // or 0x3F depending on your module
                                   
#include <hd44780.h>
#include <pcf8574.h>

static i2c_dev_t pcf8574;

void lcd_test(void *pvParameters);

static esp_err_t write_lcd_data(const hd44780_t *lcd, uint8_t data)
{
    return pcf8574_port_write(&pcf8574, data);
}

class LCD{
    private:
        hd44780_t lcd_conf;
    public:
        LCD(void);
        void print(const char* message, uint8_t x_loc, uint8_t y_loc);
};
