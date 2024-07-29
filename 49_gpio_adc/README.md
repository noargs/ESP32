ESP32 has 2 [ADCs](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/peripherals/adc.html), ADC1 with 8 channels and ADC2 with 10 channels    
    
### ADC Limitations:    
Some of the ADC2 pins are used as strapping pins (GPIO 0, 2, 15) thus cannot be used freely. Such is the case in the following official Development Kits:     
    
**ESP32 DevKitC**: GPIO 0 cannot be used due to external auto program circuits.     
     
**ESP-WROVER-KIT**: GPIO 0, 2, 4 and 15 cannot be used due to external connections for different purposes.     
     
Since the ADC2 module is also used by the Wi-Fi, only one of them could get the preemption when using together, which means the adc2_get_raw() may get blocked until Wi-Fi stops, and vice versa.    

Internal Hall sensor of ESP32 uses channels 0 and 3 of ADC1 (GPIO 36 and 39). Do not connect anything else to these pins and do not change their configuration. Otherwise it may affect the measurement of low value signal from the sensor.    
     
In this example we will use a Light senstive resistor to tell us if light is on or of. To do that we need voltage divider. Light sensitive resistor is generally 10 kiloohms in darkness and in light its about 5000 ohms. If we know that voltage source is 3.3v and light sensor in darkness is 10,000 ohms then we need to find out how much resistance do i need to get about 1.1v out.     
     
### Connection details    

3.3v <-> Light sensitive resistor
GND <-> Light sensitive resistor <-> Static resistor  <-> ADC1_CHANNEL_0     

We aren't using Hall sensor hence its safe to use ADC1 CHANNEL0   
    
          