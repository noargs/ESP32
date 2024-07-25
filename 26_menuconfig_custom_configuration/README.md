Your custom configuration can be created by creating a file `Kconfig.projbuild` in the `main` directory     
     
Type in terminal `idf.py menuconfig` and go to your menu, in our case it is `"My Custom Menu"` and change some of the settings hit `S` to save and then exit.      

Now run `idf.py build` and it will do full build as configuration has been changed.     

You can find your configuration will be generated at `build/config/sdkconfig.h` and names like `MY_STRING` will be prefixed with `CONFIG` (i.e. `#define CONFIG_MY_STRING`)    

You can reference these configuration from `main.c`      
```c
#include <stdio.h>
#include <stdbool.h>

#include "esp_log.h"

#define TAG       "CONFIG"

void app_main(void)
{
  ESP_LOGI(TAG, "MY_INT %d", CONFIG_MY_INT);
  ESP_LOGI(TAG, "MY_STRING %s", CONFIG_MY_STRING);

  bool my_bool = false;

#ifdef CONFIG_MY_BOOL
  my_bool = true;
#else
  my_bool = false;
#endif

  ESP_LOGI(TAG, "MY_BOOL %s", my_bool ? "yes" : "no");

  int option = 0;

#ifdef CONFIG_OPTION_1
  option = 1;
#elif CONFIG_OPTION_2
  option = 2;
#else
  option = 3;
#endif

  ESP_LOGI(TAG, "MY_OPTION %d", option);
}
```                     
