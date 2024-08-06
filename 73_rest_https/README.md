There are two options to use TLS    

### For testing purpose only              
`idf.py menuconfig` > Component config > ESP-TLS > Allow potentially insecure options    
`idf.py menuconfig` > Component config > ESP-TLS > Skip server certificate verification by default   
     
### Get certificate     
Type `https://weatherapi-com.p.rapidapi.com/forecast.json?q=Carrick-on-suir&days=3` in the browser you'll get an error as API Keys aren't provided however you will get the chance to download the certificate. (Dont forget to turn off the above _For testing purpose only_ if it was previously turned on).   
Download the certificate (Base64) other then the wildcard one (Not `*.p.rapidapi.com` but `Amazon RSA 2048 M02`)     
      
> [!NOTE]    
> You can download **Single certificate** or **Chain certificate**. They have different working however either can be used for https      

Create folder in the project root `cert` and add your certificate there. Add the following entry into 'CMakeLists.txt'       
```cmake
idf_component_register(
  SRCS "main.c"
  INCLUDE_DIRS "."
  EMBED_TXTFILES "../cert/amazon_chain_cert.pem")
```       
     
And then you can grab the certificate in your application with `asm` attribute as follows:    
```c
extern const uint8_t cert[] asm("_binary_amazon_chain_cert_pem_start");
```     
> [!IMPORTANT]     
> Name of the certificate file has to be prefixed by `_binary_` and suffix by `_start` and any dot or special characters in the filename has to be replaced by underscores    
        
Finally, you can use the certificate `cert` array, by providing it as a value to the `.cert_pem` member of the ``esp_http_client_config_t` struct, along with any REST call       
```c
esp_http_client_config_t esp_http_client_config = {
  .url = "https://weatherapi-com.p.rapidapi.com/forecast.json?q=Carrick-on-suir&days=3",
  ...
  .cert_pem = (char*)cert,
```   


