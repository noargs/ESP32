Hit CMD+P or (Ctrl+P for windows) and write `>ESP-IDF:Show Component Registry` and hit enter       
     
Type `led` in the _Search Components_ to find the suitable library      

As we are looking for library for _Addressable LED Strip_. Hence we will go for `espressif/led_strip vx.xx`. Therefore tap into it and hit the _Install_ button.    

New file `idf_component.yml` will appear which will keep track of the dependencies.  

You can grab the sample code from _ESP Component Resigtry_ page and use that in the `main.c`     

Under **API References** in the _ESP Component Register_ page you will find the headers you need to include (i.e. `#include "led_strip.h"`).                  

