SPIFFS background and [project setup](../36_spiffs1_with_app/)    
     
SPIFFS is virtual filesytem _vfs_ and we dont have the true notion of a directory. Hence in `fopen("/spiffs/sub/data.txt", "r")`, `sub/data.txt` is the actual name of the file. Therefore we cannot go into the directory and get the file out from the directory.   
    
`#include <dirent.h>` dir entery gives us some information about directories.   
`#include <sys/unistd.h>` is the Unix standard, it has some definitions of particular types that we may utilise       
`#include <sys/stat.h>` gives us statistics of a particular file.