This project is taken from `~/esp/esp-idf/examples/peripherals/i2c/i2c_tools`    

- Press CMD+P and type `>ESP-IDF: Add vscode configuration folder` and hit Enter     
- Press CMD+P and type `>ESP-IDF: Configure ESP-IDF extension` and hit Enter, New window apears and click `Use Existing Setup` and click Your desired IDF version.    
- type in terminal `get_idf` and then `idf.py flash monitor`     

Run following commands in the `i2c-tools>` prompt    
- `i2c-tools> help` for help     
- `i2c-tools> i2cdetect` will output the bus which is empty as we haven't updated the SDA, SCL Pin numbers in our code `cmd_i2ctools.c` which we can change in the prompt `i2c-tools> i2cconfig --sda=26 --scl=25`    
- Now run `i2c-tools> i2cdetect` to get the bus updated state, you will see the output `48` (device sitting on the bus at the address `48`)     
- `i2c-tools> i2cget -c 0x48 -r 0 -l 2` consist of slave address `0x48` got from `i2cdetect`, read register `-r` at address `0` and `-l` length to read is `2` bytes, for example the output we get is `0x1b 0x60`.      
     
**According to [datasheet](../docs/LM75A_Datasheet_01.pdf) page 7:**     
The Temperature register (Temp) holds the digital result of temperature measurement or monitor at the end of each analog-to-digital conversion. This register is read-only and contains two 8-bit data bytes consisting of one Most Significant Byte (MSByte) and one Least Significant Byte (LSByte). However, only 11 bits of those two bytes are used to store the Temp data in 2’s complement format with the resolution of 0.125 °C      
     
Table 9.   Temp register  
| MSByte | LSByte |
|:---------------------------------|:---------------------------------|     
|  7   6   5   4   3   2   1   0  | 7   6   5   4  3  2  1  0 |     
| D10  D9  D8  D7  D6  D5  D4  D3 | D2  D1  D0  x  x  x  x  x |      
| 0x1b | 0x60 |  
  
- We had sensor output as `0x1b 0x60`, in other words `0x1b60` and in binaries **00011011011**00000 and taking first 11th bits (MSB) and ignoring the rest LSB. That gives us `0xdb` or `219` in decimal.     
    
Going further in the datasheet we have two options as follows:   
1. If the Temp data MSByte bit D10 = 0, then the temperature is positive and Temp value (°C) = +(Temp data) × 0.125 °C.
2. If the Temp data MSByte bit D10 = 1, then the temperature is negative and Temp value (°C) = −(2’s complement of Temp data) × 0.125 °C.     
    
Having all the details we can calculate, when D10=0 as follows:    
+(219) × 0.125 °C = 27.375      
     
### I2C   
1. Only 2 lines (SDA, SCL) are required    
2. Multiple devices on the same bus    
3. An I<sup>2</sup>C circuit consists of one master and many slaves    
4. Each device has an address, one and more registers    
5. Some registers can be read from others can be written to   
6. Sometime you first need to write to a register as a command before doing a read     
7. Sometime you need to read/write more than one byte at a time     
8. The specific on how to communicate to the commponents is defined in the datasheet ([LM75A](../docs/LM75A_Datasheet_01.pdf)) of that component.
     
### ESP32 I<sup>2</sup>C Support     
1. 2 x I<sup>2</sup>C busses    
2. Either bus can act as a master or slave   
3. Configurable GPIO for SDA and SCL    

### Connection details    
| ESP32 | LM75A |
|:--------|:--------|
| 3.3v | VCC |
| GND | GND |
| 26 | SDA |
| 25 | SCL |   





### I2C Tools Example

(See the README.md file in the upper level 'examples' directory for more information about examples.)

### Overview

[I2C Tools](https://i2c.wiki.kernel.org/index.php/I2C_Tools) is a simple but very useful tool for developing I2C related applications, which is also famous in Linux platform. This example just implements some of basic features of [I2C Tools](https://i2c.wiki.kernel.org/index.php/I2C_Tools) based on [esp32 console component](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/console.html). As follows, this example supports five command-line tools:

1. `i2cconfig`: It will configure the I2C bus with specific GPIO number, port number and frequency.
2. `i2cdetect`: It will scan an I2C bus for devices and output a table with the list of detected devices on the bus.
3. `i2cget`: It will read registers visible through the I2C bus.
4. `i2cset`: It will set registers visible through the I2C bus.
5. `i2cdump`: It will examine registers visible through the I2C bus.

If you have some trouble in developing I2C related applications, or just want to test some functions of one I2C device, you can play with this example first.

## How to use example

### Hardware Required

To run this example, you should have any ESP32, ESP32-S and ESP32-C based development board. For test purpose, you should have a kind of device with I2C interface as well. Here we will take the CCS811 sensor as an example to show how to test the function of this sensor without writing any code (just use the command-line tools supported by this example). For more information about CCS811, you can consult the [online datasheet](http://ams.com/ccs811).

#### Pin Assignment:

**Note:** The following pin assignments are used by default, you can change them with `i2cconfig` command at any time.

|                     | SDA    | SCL    | GND  | Other | VCC  |
| ------------------- | ------ | ------ | ---- | ----- | ---- |
| ESP32 I2C Master    | GPIO18 | GPIO19 | GND  | GND   | 3.3V |
| ESP32-S2 I2C Master | GPIO18 | GPIO19 | GND  | GND   | 3.3V |
| ESP32-S3 I2C Master | GPIO1  | GPIO2  | GND  | GND   | 3.3V |
| ESP32-C3 I2C Master | GPIO5  | GPIO6  | GND  | GND   | 3.3V |
| ESP32-C2 I2C Master | GPIO5  | GPIO6  | GND  | GND   | 3.3V |
| ESP32-H2 I2C Master | GPIO1  | GPIO2  | GND  | GND   | 3.3V |
| Sensor              | SDA    | SCL    | GND  | WAK   | VCC  |

**Note:** It is recommended to add external pull-up resistors for SDA/SCL pins to make the communication more stable, though the driver will enable internal pull-up resistors.

### Configure the project

Open the project configuration menu (`idf.py menuconfig`). Then go into `Example Configuration` menu.

- You can choose whether or not to save command history into flash in `Store command history in flash` option.

### Build and Flash

Run `idf.py -p PORT flash monitor` to build and flash the project..

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

### Check all supported commands and their usages

```bash
 ==============================================================
 |       Steps to Use i2c-tools on ESP32                      |
 |                                                            |
 |  1. Try 'help', check all supported commands               |
 |  2. Try 'i2cconfig' to configure your I2C bus              |
 |  3. Try 'i2cdetect' to scan devices on the bus             |
 |  4. Try 'i2cget' to get the content of specific register   |
 |  5. Try 'i2cset' to set the value of specific register     |
 |  6. Try 'i2cdump' to dump all the register (Experiment)    |
 |                                                            |
 ==============================================================

i2c-tools> help
help
  Print the list of registered commands

i2cconfig  [--port=<0|1>] [--freq=<Hz>] --sda=<gpio> --scl=<gpio>
  Config I2C bus
  --port=<0|1>  Set the I2C bus port number
   --freq=<Hz>  Set the frequency(Hz) of I2C bus
  --sda=<gpio>  Set the gpio for I2C SDA
  --scl=<gpio>  Set the gpio for I2C SCL

i2cdetect
  Scan I2C bus for devices

i2cget  -c <chip_addr> [-r <register_addr>] [-l <length>]
  Read registers visible through the I2C bus
  -c, --chip=<chip_addr>  Specify the address of the chip on that bus
  -r, --register=<register_addr>  Specify the address on that chip to read from
  -l, --length=<length>  Specify the length to read from that data address

i2cset  -c <chip_addr> [-r <register_addr>] [<data>]...
  Set registers visible through the I2C bus
  -c, --chip=<chip_addr>  Specify the address of the chip on that bus
  -r, --register=<register_addr>  Specify the address on that chip to read from
        <data>  Specify the data to write to that data address

i2cdump  -c <chip_addr> [-s <size>]
  Examine registers visible through the I2C bus
  -c, --chip=<chip_addr>  Specify the address of the chip on that bus
  -s, --size=<size>  Specify the size of each read

free
  Get the current size of free heap memory

heap
  Get minimum size of free heap memory that was available during program execu
  tion

version
  Get version of chip and SDK

restart
  Software reset of the chip

deep_sleep  [-t <t>] [--io=<n>] [--io_level=<0|1>]
  Enter deep sleep mode. Two wakeup modes are supported: timer and GPIO. If no
  wakeup option is specified, will sleep indefinitely.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

light_sleep  [-t <t>] [--io=<n>]... [--io_level=<0|1>]...
  Enter light sleep mode. Two wakeup modes are supported: timer and GPIO. Mult
  iple GPIO pins can be specified using pairs of 'io' and 'io_level' arguments
  . Will also wake up on UART input.
  -t, --time=<t>  Wake up time, ms
      --io=<n>  If specified, wakeup using GPIO with given number
  --io_level=<0|1>  GPIO level to trigger wakeup

tasks
  Get information about running tasks
```

### Configure the I2C bus

```bash
esp32> i2cconfig --port=0 --sda=18 --scl=19 --freq=100000
```

* `--port` option to specify the port of I2C, here we choose port 0 for test.
* `--sda` and `--scl` options to specify the gpio number used by I2C bus, here we choose GPIO18 as the SDA and GPIO19 as the SCL.
* `--freq` option to specify the frequency of I2C bus, here we set to 100KHz.

### Check the I2C address (7 bits) on the I2C bus

```bash
esp32> i2cdetect
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- 5b -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
```

* Here we found the address of CCS811 is 0x5b.

### Get the value of status register

```bash
esp32> i2cget -c 0x5b -r 0x00 -l 1
0x10
```

* `-c` option to specify the address of I2C device (acquired from `i2cdetect` command).
* `-r` option to specify the register address you want to inspect.
* `-l` option to specify the length of the content.
* Here the returned value 0x10 means that the sensor is just in the boot mode and is ready to go into application mode. For more information about CCS811 you should consult the [official website](http://ams.com/ccs811).

### Change the working mode

```bash
esp32> i2cset -c 0x5b -r 0xF4
I (734717) cmd_i2ctools: Write OK
esp32> i2cset -c 0x5b -r 0x01 0x10
I (1072047) cmd_i2ctools: Write OK
esp32> i2cget -c 0x5b -r 0x00 -l 1
0x98
```

* Here we change the mode from boot to application and set a proper measure mode (by writing 0x10 to register 0x01)
* Now the status value of the sensor is 0x98, which means a valid data is ready to read

### Read the sensor data

```bash
esp32> i2cget -c 0x5b -r 0x02 -l 8
0x01 0xb0 0x00 0x04 0x98 0x00 0x19 0x8f
```

* The register 0x02 will output 8 bytes result, mainly including value of eCO~2~、TVOC and there raw value. So the value of eCO~2~ is 0x01b0 ppm and value of TVOC is 0x04 ppb.

## Troubleshooting

* I don’t find any available address when running `i2cdetect` command.
  * Make sure your wiring connection is right.
  * Some sensor will have a “wake up” pin, via which user can put the sensor into a sleep mode. So make sure your sensor in **not** in the sleep state.
  * Reset you I2C device, and then run `i2cdetect` again.
* I can’t get the right content when running `i2cdump` command.
  * Currently the `i2cdump` only support those who have the same content length of registers inside the I2C device. For example, if a device have three register addresses, and the content length at these address are 1 byte, 2 bytes and 4 bytes. In this case you should not expect this command to dump the register correctly.


(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)

