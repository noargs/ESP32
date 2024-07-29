ESP IDF supports two 8 bit [DACs](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/dac.html) (Digital to Analogue converters), Pin 25 and 26, which allows us to change the voltage levels coming through to our LED.    
    
GPIO25 (Channel 1) and GPIO26 (Channel 2)    

255 in `dac_output_voltage(DAC_CHANNEL_1, 255)` correspond to 3.3v in ESP32 as it's 8 bit hence 255 and can go all the way to 0.

### Connection details   
   
| ESP32 | LED |    
|:--------|:--------|    
| 25   | One end  |      
| GND   | Other end  |  