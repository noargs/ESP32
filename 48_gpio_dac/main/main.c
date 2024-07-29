#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"

void app_main(void)
{
  dac_output_enable(DAC_CHANNEL_1);

  int value = 190;
  int increament = 1;

  while (true) 
  {
    if (increament)
    {
     dac_output_voltage(DAC_CHANNEL_1, value++);
     vTaskDelay(30/portTICK_PERIOD_MS); 
     if (value >= 255) increament = 0;
    }
    else
    {
     dac_output_voltage(DAC_CHANNEL_1, value--);
     vTaskDelay(50/portTICK_PERIOD_MS); 
     if (value <= 190) increament = 1;
  
    }
    vTaskDelay(1); 
  }  
}
