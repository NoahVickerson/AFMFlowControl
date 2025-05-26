
#include "lcd.hpp"

#include "esp_log.h"
#include "driver/i2c.h"
#include <hd44780.h>
#include <pcf8574.h>
#include <sys/time.h>
#include <freertos/FreeRTOS.h>
#include <string.h>

static uint32_t get_time_sec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}

static const uint8_t char_data[] = {
    0x04, 0x0e, 0x0e, 0x0e, 0x1f, 0x00, 0x04, 0x00,
    0x1f, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x1f, 0x00
};

LCD::LCD(){
    lcd_conf = {
        .write_cb = write_lcd_data, // use callback to send data to LCD by I2C GPIO expander
        .pins = {
            .rs = 0,
            .e  = 2,
            .d4 = 4,
            .d5 = 5,
            .d6 = 6,
            .d7 = 7,
            .bl = 3
        },
        .font = HD44780_FONT_5X8,
        .lines = 2,
    };

    memset(&pcf8574, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(pcf8574_init_desc(&pcf8574, LCD_I2C_ADDR, I2C_MASTER_NUM, I2C_MASTER_SDA_IO,  I2C_MASTER_SCL_IO));

    ESP_ERROR_CHECK(hd44780_init(&lcd_conf));

    hd44780_switch_backlight(&lcd_conf, true);

    hd44780_upload_character(&lcd_conf, 0, char_data);
    hd44780_upload_character(&lcd_conf, 1, char_data + 8);

    hd44780_gotoxy(&lcd_conf, 0, 0);
    hd44780_puts(&lcd_conf, "\x08 Hello world!");
    hd44780_gotoxy(&lcd_conf, 0, 1);
    hd44780_puts(&lcd_conf, "\x09 ");
}

void LCD::print(const char* message, uint8_t x_loc, uint8_t y_loc){
        hd44780_gotoxy(&lcd_conf, x_loc, y_loc);

        hd44780_puts(&lcd_conf, message);
        return;
}

void lcd_test(void *pvParameters)
{

    hd44780_t lcd = {
        .write_cb = write_lcd_data, // use callback to send data to LCD by I2C GPIO expander
        .pins = {
            .rs = 0,
            .e  = 2,
            .d4 = 4,
            .d5 = 5,
            .d6 = 6,
            .d7 = 7,
            .bl = 3
        },
        .font = HD44780_FONT_5X8,
        .lines = 2,
    };

    memset(&pcf8574, 0, sizeof(i2c_dev_t));

    ESP_ERROR_CHECK(pcf8574_init_desc(&pcf8574, LCD_I2C_ADDR, I2C_MASTER_NUM, I2C_MASTER_SDA_IO,  I2C_MASTER_SCL_IO));

    ESP_LOGI("3", "third check");
    ESP_ERROR_CHECK(hd44780_init(&lcd));
    ESP_LOGI("4", "fourth check");

    hd44780_switch_backlight(&lcd, true);

    hd44780_upload_character(&lcd, 0, char_data);
    hd44780_upload_character(&lcd, 1, char_data + 8);

    hd44780_gotoxy(&lcd, 0, 0);
    hd44780_puts(&lcd, "\x08 Hello world!");
    hd44780_gotoxy(&lcd, 0, 1);
    hd44780_puts(&lcd, "\x09 ");

    char time[16];

    while (1)
    {
        hd44780_gotoxy(&lcd, 2, 1);

        snprintf(time, 7, "%" PRIu32 "  ", get_time_sec());
        time[sizeof(time) - 1] = 0;

        hd44780_puts(&lcd, time);

        ESP_LOGI("lcd", "testing lcd");
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
    vTaskDelete(NULL);
}
