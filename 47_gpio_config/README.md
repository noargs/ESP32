Simpliset way to configure GPIOs instead of calling each and our API to use the functionality    
     
```c
gpio_config_t config = {
  .intr_type =  GPIO_INTR_POSEDGE,
  .mode = GPIO_MODE_INPUT,
  .pull_down_en = 1,
  .pull_up_en = 0,
  .pin_bit_mask = ((1ULL << PIN_SWITCH) | (1ULL << 12)),
};

gpio_config(&config);


// Instead use `gpio_config_t`
// gpio_set_direction(PIN_SWITCH, GPIO_MODE_INPUT);
// gpio_pulldown_en(PIN_SWITCH);
// gpio_pullup_dis(PIN_SWITCH);
// gpio_set_intr_type(PIN_SWITCH, GPIO_INTR_POSEDGE);
```     