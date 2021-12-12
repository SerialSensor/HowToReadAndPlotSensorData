<img align="left" width="50" height="100" src="https://github.com/SerialSensor/ProjMedia/blob/master/appIcon.png" alt="WTF">

# How to read and plot sensor data
Example code for reading and plotting all currently available sensors
## Overview
- [Get the App](#get-the-app)
- [How to read sensor data with an Arduino](#how-to-read-sensor-data-with-an-arduino)
- [How to read or plot sensor data with your PC](#how-to-read-or-plot-sensor-data-with-your-pc)
  - [Receive data via USB](#receive-data-via-usb)
## Get the App
Serial Sensor is free, you can download it from the [play store](https://play.google.com/store/apps/details?id=com.karl.serialsensor).
## How to read sensor data with an Arduino
With an Arduino you can either read the sensor data via USB or Bluetooth (ony HC-Modules are currently supported). The provided example code should be self-explanatory.
## How to read or plot sensor data with your PC
You can send sensor data to your pc via the "Network" connection. There are two use-cases, which are explained below. Furthermore two scripts are provided to either plot or just receive and print the sensor data in real-time. 
<img height="500" alt="fast 1" src="https://raw.githubusercontent.com/SerialSensor/ProjMedia/master/Plot/plot.jpg"> 
### Receive data via W-Lan
Connect your Phone and PC to the same W-Lan. Get the IP-Address of your PC and enter it inside the app's connection menu (for the Network connection). Add the same IP-Address inside the python script. That's it, you are good to go. 
### Receive data via USB
You can receive data via USB connection only but using the "Network" connection mode. To do so, connect your Phone to your PC and enable USB-Thethering inside your Phone's settings. Your PC gets then a IP address assigned, which you can then use like for the W-Lan use-case.
