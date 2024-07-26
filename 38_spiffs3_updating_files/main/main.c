#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"

#include <stdlib.h>
#include <dirent.h>       // info about directories
#include <sys/unistd.h>   // contain types we may use
#include <sys/stat.h>     // statistics of particular file

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

  DIR *dir = opendir("/spiffs");
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
    char fullpath[300];
    sprintf(fullpath, "/spiffs/%s", entry->d_name);
    struct stat entryStat;
    if (stat(fullpath, &entryStat) == -1)
    {
      ESP_LOGE(TAG, "error getting stats for %s", fullpath);
    }
    else
    {
      ESP_LOGI(TAG, "full path = %s, file size = %ld", fullpath, entryStat.st_size);
    }
  }

  size_t total = 0, used = 0;
  esp_spiffs_info(NULL, &total, &used);
  ESP_LOGI(TAG, "total = %d, used = %d", total, used);

  FILE *file = fopen("/spiffs/index.html", "r");    // r, w, or rw
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

