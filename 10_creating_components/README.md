You can have a module (or miniature type of project) outside `main` which can be used in your other projects.      
     
Create _components_ directory and add your module/project (i.e. `calc`) in there.  

Create one `CMakeLists.txt` for _calc_ project as follows    
```cmake
idf_component_register(
  SRCS 
      "calc.c"
                    
  INCLUDE_DIRS 
      "."
)      
```      
     
To get _include_ capability (i.e. `#include "driver/gpio.h"`) to flash an LED or write to display etc in the _calc_ project. Add `REQUIRES` to _CMakeLists.txt_ and to get the name of library/component, you can go to the place where ESP was installed (i.e. `esp/esp-idf/components`) on your PC and search for the name (i.e. `driver`) and provide it in _CMakeLists.txt_ as follows     
```cmake
idf_component_register(
  SRCS 
      "calc.c"
                    
  INCLUDE_DIRS 
      "."
  REQUIRES
      "driver" 
)                   
```      
     
Now you can safely include `#include "driver/gpio.h"` to your `calc` project use the required functionality from the component pre-bundled with ESP-IDF (i.e. `esp/esp-idf/components`)     