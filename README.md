# arduino-smart-blinds

## Frontend web application to control an arduino

This project aims at created a frontent web page that communicates with an arduino via a remote serial terminal (based on https://github.com/bricoleau/WiFiTerm). The arduino controls the blinds in the house.

The goal is to have an web interface that is synced with the current state of the blinds (open/close for every blind). The arduino can also be controlled by regular swithes in the house.

## Dependencies

-   https://github.com/bricoleau/WiFiTerm
-   https://github.com/Links2004/arduinoWebSockets
-   https://github.com/esp8266/arduino-esp8266fs-plugin (tool to upload files to the arduino)

Line to add in the arduino IDE preference: https://dl.espressif.com/dl/package_esp32_index.json,https://arduino.esp8266.com/stable/package_esp8266com_index.json
