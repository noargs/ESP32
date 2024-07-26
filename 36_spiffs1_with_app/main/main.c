#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"

#define TAG      "SPIFFS"

void app_main(void)
{
  esp_vfs_spiffs_conf_t config = {
    .base_path              = "/spiffs",   // root directory of how we actually see our spiffs files
    .partition_label        = NULL,        // if NULL (no name given) it will going to use first partition it will find in our partition table i.e. `storage`
    .max_files              = 5,           // how many files can open up simultaneously
    .format_if_mount_failed = true,        // anything goes wrong (any file corrupted) it will format the particular storage partition  
  };

  esp_vfs_spiffs_register(&config);

  FILE *file = fopen("/spiffs/sub/data.txt", "r");    // r, w, or rw
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

  esp_vfs_spiffs_unregister(NULL);          // NULL will cleanup the first partition
}
