# ESP32 Smart Door Lock using Web Server

## Overview
This project implements a smart door lock system using ESP32 and a solenoid lock. The lock can be controlled through a web interface over WiFi.

The ESP32 connects to a WiFi network and runs a web server. A user can open the ESP32 IP address in a browser and control the lock by pressing lock and unlock buttons.

## Working
- ESP32 connects to a WiFi network using given SSID and password
- A web server is started on port 80
- The main page displays the current lock status (LOCKED or UNLOCKED)
- When the user clicks a button:
  - A request is sent to the ESP32
  - ESP32 updates the GPIO output
  - The relay is activated to control the solenoid lock

## Components Used
- ESP32
- Relay module
- Solenoid lock
- External power supply
- Connecting wires

## Connections
- GPIO 25 connected to relay input
- Relay controls the solenoid lock using external power
- Common ground between ESP32 and external supply

## Software
- Arduino IDE
- WiFi library
- WebServer library

## Usage
1. Upload the code to ESP32
2. Open Serial Monitor
3. Wait for WiFi connection
4. Note the IP address
5. Open the IP address in a browser
6. Use the buttons to lock or unlock

## Description
The system provides a simple web-based interface for controlling a solenoid lock. The ESP32 handles HTTP requests and controls the lock state through a relay module.
