### Configure SSID and PASSWORD    
Run `idf.py menuconfig` and go to:     
`Example Connection Configuration >  WiFi SSID`   
`Example Connection Configuration >  WiFi Password`     
      
### Add ESP-IDF example app      
We added external component in the root `CMakeLists.txt` file as IDF external component, be sure to alter the path wherever you have downloaded ESP-IDF  
```cmake
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common)
```   
    
> [!NOTE]   
> We must initialise the non-volatile storage i.e. NVS `nvs_flash_init()`, Whenever we want to connect to the WiFi.   
> You will always have to include these 3 header whenever you want to work with WiFi. `#include "nvs_flash.h"`, `#include "esp_wifi.h"`, and `#include "esp_event.h"`    
     

