#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <u8g2.h>  

#include "u8g2_esp32_hal.h"

/** SPI */
#define PIN_CLK    14
#define PIN_MOSI   13
#define PIN_DC     27
#define PIN_CS     15
#define PIN_RES    33

/** I2C */
#define PIN_SDA      26
#define PIN_SCL      25
#define LM75A_ADDRESS 0x48
#define I2C_NUM       I2C_NUM_0

static char tag[] = "Ibn_SSD1306";
uint8_t raw[2];
char display_data[20];
u8g2_t u8g2;
void out_static_display(void);

void app_main(void)
{
  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
  u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
  u8g2_esp32_hal.bus.spi.cs   = PIN_CS;
  u8g2_esp32_hal.dc           = PIN_DC;
  u8g2_esp32_hal.reset        = PIN_RES;
  u8g2_esp32_hal_init(u8g2_esp32_hal);

  /** a structure to contain all the data for a single display */
  

  /** init u8g2 structure */
  u8g2_Setup_ssd1306_128x64_noname_f(
    &u8g2,
    U8G2_R0,
    u8g2_esp32_spi_byte_cb,
    u8g2_esp32_gpio_and_delay_cb
  );

  /** send init sequence to the display, as in sleep mode after this */
  u8g2_InitDisplay(&u8g2);

  /** wakeup display */
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearBuffer(&u8g2);


    i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = PIN_SDA,
    .scl_io_num = PIN_SCL,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000,
  };
  i2c_param_config(I2C_NUM, &i2c_config);
  i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0);

  
  i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
  i2c_master_start(cmd_handle);
  i2c_master_write_byte(cmd_handle, (LM75A_ADDRESS << 1) | I2C_MASTER_READ, true);
  i2c_master_read(cmd_handle, (uint8_t*)&raw, 2, I2C_MASTER_ACK);
  i2c_master_stop(cmd_handle);

  out_static_display();

  while (true)
  {
    i2c_master_cmd_begin(I2C_NUM, cmd_handle, 1000/portTICK_PERIOD_MS);
    bool isNeg = false;
    if (raw[0] & 0x80)
    {
      isNeg = true;
      raw[0] = raw[0] & 0x7f;
    }
    int16_t data = (raw[0] << 8 | raw[1]) >> 5;
    float temperature = (data * 0.125) * (isNeg ? -1 : 1);
    u8g2_SetFont(&u8g2, u8g2_font_cupcakemetoyourleader_tu);
    sprintf(display_data, "%.1f", temperature);
    printf(display_data);
    u8g2_DrawUTF8(&u8g2, 0, 15, display_data);
    u8g2_SendBuffer(&u8g2);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
  
  i2c_cmd_link_delete(cmd_handle);

  ESP_LOGD(tag, "All done!");
}

void out_static_display(void)
{
  u8g2_SetFont(&u8g2, u8g2_font_unifont_h_symbols);
  u8g2_DrawUTF8(&u8g2, 37, 12, "°");
  u8g2_SetFont(&u8g2, u8g2_font_cupcakemetoyourleader_tu);
  u8g2_DrawStr(&u8g2, 44, 15, "C");

  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_symbols);  // TODO: Sun/Moon/Cloudy
  u8g2_DrawGlyph(&u8g2, 58, 15, 0x2600 );
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_77);
  u8g2_DrawGlyph(&u8g2, 59, 15, 0x26ef );

  u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);
  u8g2_DrawStr(&u8g2, 88, 7, "CARRICK");              // TODO: GPS locatioon
  u8g2_DrawStr(&u8g2, 88, 15, "01:24 PM");            // TODO: RTC time goes here 

  
  u8g2_SetFont(&u8g2, u8g2_font_ciircle13_tr);
  u8g2_DrawUTF8(&u8g2, 0, 28, "Aslamualikum");
  u8g2_DrawStr(&u8g2, 0, 44, "Thanks");
  u8g2_SetFont(&u8g2, u8g2_font_unifont_t_78_79);     // emoji
  u8g2_DrawGlyph(&u8g2, 52, 44, 0x2764);
  u8g2_SetFont(&u8g2, u8g2_font_streamline_all_t);    // ship
  u8g2_DrawGlyph(&u8g2, 85, 53, 0x0050 );
  u8g2_SetFont(&u8g2, u8g2_font_streamline_all_t);    // tortoise
  u8g2_DrawGlyph(&u8g2, 75, 70, 0x0252 );
  u8g2_SetFont(&u8g2, u8g2_font_streamline_all_t);    // palm tree
  u8g2_DrawGlyph(&u8g2, 107, 60, 0x02a4 );
  u8g2_SetFont(&u8g2, u8g2_font_ciircle13_tr);
  u8g2_DrawStr(&u8g2, 0, 59, "You well?");
  u8g2_SendBuffer(&u8g2);
}
