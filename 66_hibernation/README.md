  Following code shows hibernation as we have turned of Peripheral, Slow as well as Fast memory.      
  ```c
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  ```    
      
However you will see that `timesWokenUp` will not presist anymore as it lived in Slow Memory which also tunred off and hence contain corrupt data.      
