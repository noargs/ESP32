Get the log via JDY-31 Bluetooth without connecting board directly to our PC serial port.    
     
### Connection details   
| ESP32 | JDY-31 |
|:-------|:--------|       
| 3.3v | VCC |
| GND | GND |
| 19 | Tx |    
| 21 | Rx |   
    
Connect JDY-31 to PC, it may ask for PIN (0000 or 1234)    

Run `idf.py menuconfig`:   
- Serial flasher config > idf.py monitor baud rate > 9600       
- Component config > Common ESP-related > UART for console output > Custom 
- Component config > Common ESP-related > UART peripheral to use for console output > UART1    
- Component config > Common ESP-related > UART RX on GPIO# > 21    
- Component config > Common ESP-related > UART TX on GPIO# > 19    
- Component config > Common ESP-related > UART console baud rate > 9600