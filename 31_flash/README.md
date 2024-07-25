`idf.py size` will give you the various section in memory, of which particular interest are _Flash code_ and _Flash rodata_    
     
Flash actually contains our code, we can also use it for our filesystem or to hold data, either variables or structures, even after the chip is being reset/reboot.     

You will find `build/partition_table/partition-table.bin`, we can use a tool (`esp-idf/components/partition_table/gen_esp32part.py`) to convert that file into a csv      

As running `get_idf` in the beginning will also include `gen_esp32part.py` into our path. Hence you dont need to run it with absoulte path (`$ python ~/esp/esp-idf/components/partition_table/gen_esp32part.py build/partition_table/partition-table.bin partition_sample.csv`)    

Instead you can run `gen_esp32part.py build/partition_table/partition-table.bin partition_sample.csv`            
     
### ESP-IDF Partition Table    
     
| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   | 0x9000  | 24K  |  |
| phy_init   | data  | phy   | 0xf000  | 4K  |  |
| factory   | app  | factory   | 0x10000  | 1M  |  |

- **Name** could be _any value_, help us to identify the tag, so we know what it is about     
- **Type** has to be one of two value either _app_ (can have subtype _factory_ or _ota 0-15_) or _data_ (can have subtype as _ota_, _phy_, _nvs_, and _spiffs_)
     
### app     
**factory** is current application that we have, information going into the chip, i.e. `app_main` function and what it contains etc. that will go to factory partition
**ota 0-15** over the air, allows us to write and application and stores in the memory in flash and we get them via WiFi and we can have multiple partitions (i.e. ota0, ota1 to ota15)      
     
### data    
**ota** this tells which ota from app i boot, shall i boot ota0, ota1 etc     
**phy** information that is used for the WiFi driver.    
**nvs** _non volatile storage_ this is where we can actually save information and have it persist even after the chip reset. Very often things like the wifi configuration that we use, also use **nvs**    
**nvs_keys** we can also have nvs_keys (encryption keys for nvs)       
     
### Offset    
Column where the memory starts, nice things is you don't have to have offset if you have size     
     
> [!NOTE]   
> app, factory and ota must have 64K offset (0x10000 or decimal 65,536 bytes / 1024 = 64K), in other words, if you have multiple OTA positions, each one of them must have the offset of multiples of 0x10000     
    
### Flags    
The last one is flags, which enable us to encrypt various different partitions, that we have on the flash, we do that by setting this particular field to `encrypted`, can only do that if we have **enabled flash encryption**   
      

Let's change the partition size of **factory** from 1M to 2M. let's see how we can configure the IDF so that it utilises this new partition table   
     
### Changing partition table
In new terminal run `idf.py menuconfig` and go down to the `Patition Table --->` and  `Partition Table (Single factory app, no OTA) --->`, default is _Single factory app, no OTA_ 
Other options are _Factory app, two OTA definitions_ and _Custom parition table CSV_   
Lets choose _Custom parition table CSV_ and there you will see _(partitions.csv) Custom partition CSV file_. Hit enter and give your `partition_sample.csv` file which you created with `gen_esp32part.py` too from `partition-table.bin`     

Run `idf.py build` and you will encounter an error _Partitions tables occupies 2.1MB of flash (2162688 bytes) which does not fit in configured flash size 2MB. Change the flash size in menuconfig under the 'Serial Flasher Config' menu._    
Go back to `idf.py menuconfig` and go to _Serial flasher config_ and _Flash size_ to 4MB  and now rebuilt `idf.py build`         
      
### Changing partition table once again
In new terminal run `idf.py menuconfig` and go down to the `Patition Table --->` and  `Partition Table (Single factory app, no OTA) --->`, and choose _Factory app, two OTA definitions_   
Now run `gen_esp32part.py build/partition_table/partition-table.bin partition_sample.csv` to generate partition table once again as shown below     

| Name | Type | SubType | Offset | Size | Flags |
|:--------|:--------|:--------|:--------|:--------|:--------|
| nvs   | data  | nvs   | 0x9000  | 16K  |  |
| otadata   | data  | ota   | 0xd000  | 8K  |  |
| phy_init   | data  | phy   | 0xf000  | 4K  |  |    
| factory   | app  | factory   | 0x10000  | 1M  |  |   
| ota_0   | data  | ota_0   | 0x110000  | 1M  |  |     
| ota_1   | data  | ota_1   | 0x210000  | 1M  |  |      
    
In summary, we can split up the flash into various partitions     