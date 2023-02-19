# ESP32-BluetoothRemote-React

This project uses an ESP32, an IR Sender and Expo to create an application to control the Edifier 1280DB speakers via a smartphone.

This works via a POST-Request to the ESP32 which then sends the signal via a IR Sender to the speakers. So to be able to send the request to the ESP you need to be in the same Network.

For this to work the sender has to be placed in front of the controlling speaker (right speaker).

## ESP32

The PIN used for data transfer to the IR Sender is GPIO13.

The ESP is configured to use the static IP-Address `192.168.0.199`, if this doesn't match your WiFi configuration you have to change the IP in the `main.cpp` file as well as in the react project in the `App.js` file in the `handleClick` function.