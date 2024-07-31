You can either use JDY-31 or USB-TTL dongle to get the reception.     
    
### JDY-31 and Connection details    
Pair the JDY-31 to your PC. Now run `idf.py build`. Find out the Outgoing port name of JDY-31 in your PC `idf.py -p /dev/cu.JDY-31-SPP monitor`    
     
| ESP32 | JDY-31 |
|:-------|:--------|       
| 3.3v | VCC |
| GND | GND |
| 4 | Tx |    
| 5 | Rx |        

