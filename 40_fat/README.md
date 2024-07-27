### Wear leveling    
    
Reading and writing (like SPIFFS) from any solid state device on same sector over and over again eventually wear it out and consquently loses its data or integrity and you can get corrupted data or it kist stop functioning completely.     
Wear leveling takes your entire partition and start writing to it in different sector, and avoiding same sector for erasing and writing again and again. It basically extends the life of your flash.    
    
**wl_handle** Wear leveling    
     
### Changes in FAT       
     
Following changes in `CMakeLists.txt`, `partitions.csv` and `idf.py menuconfig`    
     
```cmake
fatfs_create_spiflash_image(storage ../flash-loaded FLASH_IN_PROJECT)    
```     
    
| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   |   | 0x6000  |  |
| phy_init   | data  | phy   |   | 0x1000  |  |
| factory   | app  | factory   |   | 1M  |  |    
~~| storage   | data  | spiffs   |   | 1M  |  |~~
| storage   | data  | fat   |   | 1M  |  |     
     
And finally in `menuconfig` _Long filename buffer in heap or on stack_ can be either which doesn't matter.   
`Component config > FAT Filesystem support > Long filename support > Long filename buffer in heap`    