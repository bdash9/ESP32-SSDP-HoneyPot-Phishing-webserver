# ESP32-SSDP-HoneyPot-Phishing-webserver

Based on the project by HakCat-Tech/Nugget-SSDP-Phisher https://github.com/HakCat-Tech/Nugget-SSDP-Phisher/blob/main/Nugget-SSDP/webserver.h

This code runs a web server on the ESP32 that is discoverable by SSDP (uPNP (plug and play)).
When someone browses to it they get a web page that has a log in. When they log in the username/password are displayed on the screen.
I changed some code so it will work with the Adafruit ESP32-S2 Feather TFT .

You will need to chaneg the SSID and password to your wifi SSID and password in the Nugget-SSDP.ino file.
