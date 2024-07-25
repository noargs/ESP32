**ESP32 Wrover** is used in this application    
      
## Pseudo RAM (PSRAM)     

Pseudo RAM is where certain modules like WROVER have got the capability of converting extra flash into RAM.   

`uxGetTaskHighWaterMark()` is used to get the current stack size.    

### How to tap into the extra memory         

Run `idf.py menuconfig` in the terminal, Go to _Serial flasher config_ **>** _Flash size_ and choose 4MB.       
      
Now go to _Component config_ **>** _ESP PSRAM_, select _Support for external, SPI-connected RAM_ and go to _SPI RAM config --->_ and pick following options:     

- _Initialise SPI RAM during startup_     
- tap into _SPI RAM access method_, _Integrate RAM into memory map_(we will have to Manually move items to memory that we want using pointers, generally **not** great option). _Make RAM allocatable using heap_caps_malloc()_(if we want to add something to our PSRAM we have to use special command `heap_caps_malloc()`). **Choose** _Make RAM allocatable using malloc() as well_(Both internal and additional external memory will be utilised if we use malloc)     
- _Run memory test on SPI RAM initialisation_       
- _Enable workaround for bug in SPI RAM cache for Rev1 ESP32s_ (If you have Rev0 you can't use PSRAM, If you have **Rev1**, you have to select this option. this option also chew up more IRAM, Rev2 you dont need to use that enabled.)      
- _Enable bank switching for >4MiB external RAM_ (Select this option, That's essentially how the chip moves thing in and out of memory.)      
- _Try to allocate memories of WiFi and LWIP in SPIRAM firstly, if failed, allocate internal memory_ (Select this option, this will put a lot of those configuration into PSRAM, hence freeup some of the internal RAM)       
- _Allow .bss segment placed in external memory_ (Essentially for globals, we can actually enable that as well, you have put special flag `EXT_RAM_ATTR` in front of your variables so to place those external variables into memory)  

**After** the above configuration, following ouput you will get **I (1260) memory: stack 2260, total ram 4491699, internal memory 363607, external memory 4191744** after running     
```c
ESP_LOGI("memory","stack %d, total ram %d, internal memory %d, external memory %d\n",
     uxTaskGetStackHighWaterMark(NULL), heap_caps_get_free_size(MALLOC_CAP_8BIT),
     heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
```                                  

**Before** above configuration, the output was **I (313) memory: stack 2252, total ram 300876**     
      
There are some [resitriction](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/external-ram.html) for external RAM:     

- When flash cache is disabled (for example, if the flash is being written to), the external RAM also becomes inaccessible. Any read operations from or write operations to it will lead to an illegal cache access exception. This is also the reason why ESP-IDF does not by default allocate any task stacks in external RAM (see below).      
      
- External RAM uses the same cache region as the external flash. This means that frequently accessed variables in external RAM can be read and modified almost as quickly as in internal RAM. However, when accessing large chunks of data (> 32 KB), the cache can be insufficient, and speeds will fall back to the access speed of the external RAM. Moreover, accessing large chunks of data can "push out" cached flash, possibly making the execution of code slower afterwards.      
      
- In general, **external RAM will not be used as task stack memory. `xTaskCreate()` and similar functions will always allocate internal memory for stack and task TCBs**.           
      

> [!NOTE]     
> The option [CONFIG_SPIRAM_ALLOW_STACK_EXTERNAL_MEMORY](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/kconfig.html#config-spiram-allow-stack-external-memory) can be used to allow placing task stacks into external memory. In these cases xTaskCreateStatic() must be used to specify a task stack buffer allocated from external memory, otherwise task stacks will still be allocated from internal memory.      
      
`char *buffer = malloc(10)` may create in the internal or external memory (unlike to `xTaskCreate()` which only creates in the Internal memory). However, if you wish to force it to go to PSRAM (external memory) then use `heap_caps_malloc(10, MALLOC_CAP_SPIRAM)`, or DMA ``heap_caps_malloc(10, MALLOC_CAP_DMA)` or keep it Internal ``heap_caps_malloc(10, MALLOC_CAP_INTERNAL)`  