`cJSON_GetErrorPtr()` will return the pointer at the string position where it failed to parse. There could be a reason other than invalid JSON and that is insufficient memory which you can get to know by `heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)`   
     
There are two solutions,   
- Amongst that simplest one is to cut down the amount of JSON that is coming. As in the URL `https://weatherapi-com.p.rapidapi.com/forecast.json?q=Carrick-on-suir&days=3` you see we are desiring 3 days (i.e.`&days=3`) forecast. If you change it to 2 days it may work as less amount of JSON will be coming.     
     
- Another option is if you are using a chip where PSRAM is available (i.e. WROVER etc) and do the following   
```c
void *json_malloc(size_t size) { return heap_caps_malloc(size, MALLOC_CAP_SPIRAM); }
void json_free(void *ptr) { heap_caps_free(ptr); }

esp_err_t parse_weather(char *weather_str)
{
  cJSON_Hooks hooks = {
    .malloc_fn = json_malloc,
    .free_fn = json_free,
  };
  cJSON_InitHooks(&hooks);

  ...
  ...
}
```     
     
Now type in the terminal `idf.py menuconfig` to go to menuconfig and then turn on/select following options:     
Component config > ESP PSRAM > Support for external, SPI-connected RAM
Component config > ESP PSRAM > Support for external, SPI-connected RAM > SPI RAM config > SPI RAM access method () ---> > Make RAM allocatable using heap_caps_malloc(...,MALLOC_CAP_SPIRAM)     
    
You can even now go up till `&days=6`    
