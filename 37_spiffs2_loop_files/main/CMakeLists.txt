idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS ".")

# register_component()

spiffs_create_partition_image(storage ../spiffs_dir FLASH_IN_PROJECT)
