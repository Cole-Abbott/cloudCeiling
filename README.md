
# Cloud Ceiling

## Description
Cloud Ceiling is a smart home project that allows you to control the lighting in your home based on the weather conditions outside. It uses data from a weather API to determine the cloud coverage and adjusts the brightness of your ceiling lights accordingly.

## Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/Cole-Abbott/cloudCeiling.git
    ```

2. Install PlatformIO VSCode extension
    - [PlatformIO](https://platformio.org/)

## Usage
1. Update "myServer.h" with your WiFi credentials
    ```c++
    #define SSID "yourSSID"
    #define password = "yourPassword";
    ```
2. Update "myNeopixel.h" with your Neopixel settings, and "myNeopixels.h" to match the led strips you are using

3. Update "index.html" to reflect the neopixel strips you are using

4. Use PlatformIO to upload the code to your ESP32 microcontroller.
5. Use Platform IO to upload the data folder to the SPIFFS of the ESP32.

6. Open the serial monitor and wait for the ESP32 to connect to the WiFi network.

7. Once connected, the ESP32 will host a web server at its IP address, which can be found in the serial monitor.


## Contributing
Contributions are welcome! If you have any ideas or improvements, please submit a pull request. 

## License
This project is licensed under the [MIT License](LICENSE).

