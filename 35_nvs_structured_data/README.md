NVS can be an option if you want to have a _Structured data_ and be able to get it using key value pair.

So far we only saw how to use Default NVS partition and Custom NVS partition and How to write integer into it. Now we will try to write/read _Structure data_ into NVS partition table.      
    
We have other API's beside reading/writing the integer `nvs_get_i32()` (i.e. `_64`, `_i8`, `_str`, `_blob`)    
     
We will add 😺😺😺😺😺 (structured data) `nvs_get_blob()` as we are keeping track of purr eating their food in a IoT application     

> [!NOTE]
> `key` has to be unique and maximum **15** as per IDF documentation in `nvs_get_blob()`.  
> We should also set the original `catSize` (last parameter to `nvs_get_blob()`). The idf checks this and error if its value is smaller then the object size in NVS. It should read `size_t catSize = sizeOf(Cat)`    

Use `idf.py erase_flash flash monitor` to full erase and flash/monitor the chipurr. It will show `NVS: Value not set yet` error as it ran first and couldn't find anything to read and also wrote 5 cats in the NVS. When you reset the chip you will read the cats back.            