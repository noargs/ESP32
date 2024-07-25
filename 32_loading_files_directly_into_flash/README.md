Text files `index.html` and `sample.txt` are null terminated as when we got the address of `indexHtml[]` it put the null at the end of it. Hence we can print it out as regular string with printf statement.     
     
We uses assembly instruction `asm("_binary_index_html_start")` where `index_html` refers to `index.html` and prefixed with `_binary_` and end with `_start`    
     
On the other hand files with Make directive `COMPONENT_EMBED_FILES` doesn't null terminated which wouldn't make sense in a jpeg or an image. In this case, if we want to use it we have to find the start and end of it
```c
extern const unsigned char imgStart[] asm("_binary_wroverb_jpg_start");
extern const unsigned char imgEnd[] asm("_binary_wroverb_jpg_end");
```     
    
Even though `imgStart[]` and `imgEnd[]` look like char arrays. There are actually pointers in memory. Hence we can take the end address and substract it with start address, that will give us the **full file size**. 
```c
const unsigned int imgSize = imgEnd - imgStart;
```  


