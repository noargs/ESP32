Previous project we have FAT Wear leveling (Read + Write). Now we will make FAT write protected (ReadOnly)    
    
Changes in `cmake`     
     
```cmake
fatfs_create_rawflash_image(storage ../flash-loaded FLASH_IN_PROJECT)
```    
     
As before, make changes to `menuconfig`   
- 4M flash (Serial flasher config > Flash size > 4MB)  
- Custom partition table `partitions.csv`
- Long filename support (Component config > FAT Filesystem support > Long filename support > Long filename buffer in heap)    

> [!NOTE]    
> If your partition size is big enough you can have multiple columns in partition table, one design for read only and other one you can use for read/write         
