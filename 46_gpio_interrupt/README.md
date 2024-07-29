`while(true)` uses cpu cycles when theres no need for it.     
    
`GPIO_INTR_POSEDGE` means interrupt on positive edge.   

`IRAM_ATTR` special compiler directive which instruct the compiler, whenever you run the code in the function body, don't run it from standard ram instead run it from dram (dynamic ram).     
     
Rule of thumb keep things simple, minimilistic in interrupt, don't branch out into too many code streams. try and exit out of interrupt as soon as we can.    

There are special limitations that you have to apply when running code within Dynamic RAM. If somethin doesn't work, try to get out of it and not to do any logic inside interrupt as much as possible.             





### Connection details   
   
| ESP32 | Push Button |    
|:--------|:--------|    
| 26   | One end  |      
| 3.3v   | Other end  |      
    