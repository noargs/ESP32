While working in your project, it's not adviseable to keep everything in one file.     
     
Hence we should create multiple files and directories.    


### Files inside main directory      
      
Incase of multiple files (i.e. `calc.c` and `calc.h`) in the _main_ directory. You have to edit _main_ directory _CMakeLists.txt_ to let the compiler know that also compile, as follows    
```cmake
idf_component_register(
  SRCS 
      "main.c" 
      "calc.c"
                    
  INCLUDE_DIRS 
      "."
)
```     
       
### Files inside main/calc directory     

```cmake
idf_component_register(
  SRCS 
      "main.c" 
      "calc/calc.c"
                    
  INCLUDE_DIRS 
      "."
      "calc"
)
```       
