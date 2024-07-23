For IDF 4 `#include "esp_system.h"`. For IDF 5 `#include "esp_timer.h"`    

`esp_timer.h` (or `esp_system.h` for IDF 4) is used to get the time as how long the time been since the chip is on         

_Second parameter_ is amount of time/ticks it is going to take before timer kicks in. _Third paramter_ to `xTimerCreate()` whether we want this to repeat (i.e. auto reload `true`) or only fired it at once `false`, _Fourth parameter_ is to give the timer an ID. _Last parameter_ is callback to run when the actual time is reach.

`esp_timer_get_time()` gives you time in micro which can be converted to milli by dividing with `1000`    

> [!NOTE]      
> Timer itself is called by idle task and we do not want to delay it (putting any wait with `vTaskDelay()` or waiting for any queues). We really want to fire this and get out of here.    
> However it is good for firing semaphores, or send queues on particular schedule. Keep it unblocked and dont put too much memory constraint on this callback function `on_timer()`,      
> **Disadvantage** It has resolution of only 1 millisecond and callback task is low priority task. Lastly we have to care about not blocking. **In such cases you have the choice to use ESP-IDF timer (next project: 22_high_resolution_timer)**