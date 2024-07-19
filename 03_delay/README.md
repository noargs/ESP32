- `vTaskDelay()` a RTOS function which takes `const TickType_t` which is defined as `typedef uint32_t TickType_t`     
- When you provide `1000` to `vTaskDelay()` it corrspond to ticks, not millisecond, frequency of the actual chip. To convert that into milliseconds, divide `1000` with `portTICK_PERIOD_MS`.     
- `portTICK_PERIOD_MS` defaulted to how fast the chip is running. `1000` divide by amount of ticks that the chip is running, will give us milliseconds.       
     
Run `idf.py flash monitor` to run the app in your board. Dont forget to start the idf environment by calling `get_idf` environment variable in the terminal.     
