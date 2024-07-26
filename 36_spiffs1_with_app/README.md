### SPIFFS (SPI Flash File System)   
     
- Flash an entire directory into flash   
- Using common c file functions to create/read/upadte/delete files.   
- Size is configured in the partition table        
- Does not currently support directory structure but the filename can act like a path      
     
## SPIFFS Process    

- Use a tool to convert a directory to a bin file. This can optionally be done at build time.
- Use a tool to load the file into flash. This can optionally be done at flash time.    
     
     
Go to `esp/esp-idf/components/partition_table` and copy the file `partitions_singleapp.csv` into our project and rename it to `paritions.csv` which contains the following table.   

| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   |   | 0x6000  |  |
| phy_init   | data  | phy   |   | 0x1000  |  |
| factory   | app  | factory   |   | 1M  |  |   

Add a new column `storage` where our `spiffs_dir` will go into the flash on the chip.   

| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   |   | 0x6000  |  |
| phy_init   | data  | phy   |   | 0x1000  |  |
| factory   | app  | factory   |   | 1M  |  |    
| storage   | data  | spiffs   |   | 1M  |  | 

Now configure this with `idf.py menuconfig` by going to `Partition table` > `Partition Table (...)` > `Custom parition table CSV` > `Custom parition CSV file` and give name of your parition table csv in our case its `partitions.csv` and hit Enter.    
Now go back and select `Serial flasher config --->` and increase `Flash size (2MB) --->` to 4MB and save `S` and exit   

Now we push the `spiffs_dir` directly into the flash when we flash the chip.    
```cmake
idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")
register_component()
spiffs_create_partition_image(storage ../spiffs_dir FLASH_IN_PROJECT)
```   
     
The last parameter `FLASH_IN_PROJECT` to `spiffs_create_partitionimage()` says we want to flash our chip when we write the `idf.py flash` command    

Next we will read the files in `main.c` that we have just flashed into the flash.   
```c
#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"

#define TAG      "SPIFFS"

void app_main(void)
{
  esp_vfs_spiffs_conf_t config = {
    .base_path              = "/spiffs",
    .partition_label        = NULL,
    .max_files              = 5,
    .format_if_mount_failed = true,
  };

  esp_vfs_spiffs_register(&config);

  FILE *file = fopen("/spiffs/sub/data.txt", "r");
  if (file == NULL)
  {
    ESP_LOGE(TAG, "could not open file");
  }
  else
  {
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
      printf(line);
    }
    fclose(file);
  }

  esp_vfs_spiffs_unregister(NULL);
}
```    

**.base_path** root directory of how we actually see our spiffs files (i.e. `spiffs_dir` will be accessed as `/spiffs`)      
**.partition_label** if NULL (no name given) it will going to use first partition it will find in our partition table i.e. `storage`      
**.max_files**  how many files we can open up simultaneously      
**.format_if_mount_failed** `true` means if anything goes wrong (any file corrupted) it will format the particular storage partition     

**fopen()** last paramter can be `r`, `w`, `rw`.    

**esp_vfs_spiffs_unregister(NULL)** giving NULL as parameter means cleanup the first partition you will see (i.e. `storage`).     

Run `idf.py flash monitor` to get the output in the terminal                        
