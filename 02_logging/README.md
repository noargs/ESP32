- run `get_idf` on new terminal to setup and use _idf environment_.
- By default `ESP_LOGD` and `ESP_LOGV` turned off, you can turn them on with **menuconfig**      
- Run `idf.py menuconfig` and go to `Component config` and scroll down to `Log output -->` and select `verbose` 
- Logs are in following order, if you wish to get debug log `ESP_LOGD` then it can log from `ESP_LOGE` till `ESP_LOGD`. Therefore `ESP_LOGV` will not run.    
     
```c
  ESP_LOGE("LOG", "This is an error");
  ESP_LOGW("LOG", "This is a warning");
  ESP_LOGI("LOG", "This is an info");
  ESP_LOGD("LOG", "This is a Debug");
  ESP_LOGV("LOG", "This is Verbose");
```       
    
- Beside configuring the Log settings in _menuconfig_ you can also configure it through code `esp_log_level_set("LOG", ESP_LOG_INFO)`. since we gave the tag `LOG` in `esp_log_level_set` hence tag with `LOG` will run upto `ESP_LOGI`. Other tags will have _menuconfig_ configuration.       
- After Tag (i.e. _"LOG"_) string (i.e. _"This is an error"_) comes, is basically formatted string. Hence you can use _format specifier_ in it.       
      
```c        
ESP_LOGE("LOG", "This is an error %d", error_number);
```      
     
> [!IMPORTANT]     
> Dont forget to include `esp_log.h` on top	 
     

