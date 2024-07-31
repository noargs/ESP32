This project uses [u8g2](https://github.com/olikraus/u8g2) library, consult [Reference Manual](https://github.com/olikraus/u8g2/wiki/u8g2reference) for details.    

As u8g2 is a generic library hence we need a mapping to allow it to work with ESP32 (ESP32 Dev kitC Wrover or Wroom) and that was taken from [u8g2-hal-esp-idf](https://github.com/mkfrey/u8g2-hal-esp-idf)      

Both libraries can be found in the [components](./components/) directory     

> [!NOTE]     
> If you newly intergrated these projects straight from github don't forget to initialise `dev_config` as it cause a [bug](https://github.com/nkolban/esp32-snippets/issues/1170) `spi_device_interface_config_t dev_config = {0};` in `u8g2_esp32_hal.c`.         
     




### Connection details    
        
| ESP32 | SSD1306 |   
|:---------|:---------|    
| GND | GND |   
| 3.3v | VCC |
| 14 | D0 (SCK) |
| 13 | D1 (MOSI) |   
| 27 | DC (Data command) |    
| 15 | CS (Chip/Slave Select) |    
| 33 | RES (RESET) |     
      
| ESP32 | LM75A |
|:--------|:--------|
| 3.3v | VCC |
| GND | GND |
| 26 | SDA |
| 25 | SCL |     

