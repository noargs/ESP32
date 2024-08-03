`wifi_scan_config_t` mention in the struct you want to get. If you want to see hidden network make `.show_hidden` true, If you want to see any channel with particular number then `.channel = 13` it will only show channel 13 enteries.      
     
> [!IMPORTANT]      
> When you newly import `Kconfig.projbuild` from another project, after you already build your current project and options from `Kconfig.projbuild` doesnt show up in `idf.py menuconfig` then it's better to run `idf.py fullclean` before going to `idf.py menuconfig`     