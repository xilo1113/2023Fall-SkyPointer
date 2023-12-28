# 2023Fall-AutoLaser
## 1. Set up
### 1.1 wifi set up
Both PC and stm32 need to connect to the same wifi.
### 1.2 PC side
#### 1.2.0 Clone the skypointer_pc folder
#### 1.2.1 Install libraries
```bash
pip install -r requirements.txt
```
#### 1.2.2 wifi set up
modify the ip address in `main.py` to the pc ip address
```python
    HOST = "YOUR_IP_ADDRESS"
    PORT = YOUR_PORT
```
modify the port address in `source/my_wifi.h` to the port socket is listening to
```c++
        static constexpr size_t REMOTE_PORT = YOUR_PORT;
```
#### 1.2.2 Run
Run main.py, and the program will start to listen to the port and wait for the data from the microcontroller.
GUI will pop up and you can type the star you want to observe in the text box and click ENTER.
### 1.2 stm32 side
#### 1.2.1 Clone the skypointer_stm32 folder
#### 1.2.2 wifi set up
modify the wifi ssid and password in `mbed_app.json`
```c++
            "nsapi.default-wifi-ssid": "\"YOUR_WIFI_ID\"",
            "nsapi.default-wifi-password": "\"YOUR_WIFI_PASSWORD\"",
```
modify the ip address in `mbed_app.json` to the pc ip address and port
```json
    "config": {
        "hostname": {
            "value": "\"PC_IP_ADDRESS\""
        },
    }
```
#### 1.2.3 Run
compile the program using MbedOS 6 and download it to the microcontroller.
### 1.3 Hardware set up
#### 1.3.1 Connect the STM32 to stepped motor driver
| Servo motor driver |STM32 |
| ----- | ------------------ |
| EN    | D1                 |
| MS1   | D2                 |
| MS2   | D3                 |
| MS3   | D4                 |
| STEP  | D10                |
| DIR   | D0                 |
| VDD   | 5V                 |
| GND(near VDD) | GND         |
#### 1.3.2 Connect the stepped motor driver to stepped motor
| driver | Stepped motor |
| ----- | ------ |
| 2B    | Red    |
| 2A    | Green  |
| 1A    | Blue   |
| 1B    | Black  |
| STEP  | D10    |
| DIR   | D0     |
#### 1.3.3 Connect the servo to the STM32
| STM32 | Servo |
| ----- | ----- |
| D6  | Signal   |
| 5V  | VCC      |
| GND | GND      |

## 2. Usage
### 2.1 PC side
Key in the star you want to observe in the text box and click ENTER.
If the star is not in the database, pointer will point to polar star.
### 2.2 stm32 side
#### 2.2.1 set up
Once the wifi socket is connected, orange LED will light up.
Once the the calibration is done, LED 3 will light up. 
Please don't move the microcontroller during the calibration.
#### 2.2.2 recalibration
If you want to recalibrate the microcontroller, press the user button on the microcontroller.  
Please do the calibration on flat serface. Our device aren't stable enough to do the calibration on a tilted floor.

#### 2.2.3 observe
Once the calibration is done, you can key in the star you want to observe in the text box and click ENTER.
Sky pointer will point to the star you want to observe.
