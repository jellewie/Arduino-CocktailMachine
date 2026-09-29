# **Cocktail Mixer Machine**  
_A machine that can mix drinks_

![Cocktail Mixer Machine CAD](Images/CAD%20overview.png)

Part of this project is sponsored by [PCBWay](https://www.pcbway.com/). Read more about that [here](Sponsorship.md).

## **Firmware**
> **Note**: This project is still in development!

### Compile
- [Arduino sketch](Arduino) The whole sketch is the 'Arduino' folder.
- [ESP32](https://dl.espressif.com/dl/package_esp32_index.json) must be added as an additional board manager in Arduino IDE.
- [FastLED](https://github.com/FastLED/FastLED) can be downloaded through the built-in library manager in Arduino IDE.
- [AccelStepper](https://github.com/waspinator/AccelStepper) can be downloaded through the built-in library manager in Arduino IDE.
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) can be downloaded through the built-in library manager in Arduino IDE.
- [Deno](https://deno.land/) to re-compile the webpage.
- [Arduino-WIFIManager](https://github.com/jellewie/Arduino-WiFiManager) (already included).
- [Arduino-Button](https://github.com/jellewie/Arduino-Button) (already included).
- [Arduino-Stable-analog-read](https://github.com/jellewie/Arduino-Stable-analog-read) (already included).
- [FreeCAD](https://github.com/FreeCAD/FreeCAD) is used for 3D modeling.
- [A2Plus_Workbench](https://github.com/kbwbe/A2plus) is used for creating the assembly.

To custom compile with new drinks and such, update the drinks in [Website/src/drinksConfig.js](Website/src/drinksConfig.js). I use an automated Google Drive sheet for this.

To update the Arduino to use this, run "deno task build" in the command prompt. This will update the client.h, after which you can start Arduino IDE and use it.

### BIN
Uploading a BIN file with a cable to the ESP can be done by any [ESP32 BIN file uploader](https://www.espressif.com/en/support/download/other-tools). this step is required for the first flash time.

## Webpage
<img align="right" src="Images/Webpage main.png" alt="The landing webpage of the machine" width=30%>
The machine makes its own fancy landing page. This page has a lot going for it, First of all, are the items sorted on last used (this is done locally per user device). But the page has also a lot of functions, for example, it has a search bar to search for the name, but it also has a dice function to select a random mix.

After you have chosen your mix, you can directly start mixing, or you can Customize the drink to your liking

<img src="Images/Webpage Customize.png" alt="Customize drink" width=30%>
It also has a settings page

<img src="Images/Webpage settings.png" alt="Settings.png" width=30%>
This is also where the dispensers are configured

<img src="Images/Webpage dispensers.png" alt="Settings.png" width=30%>

## LED status


## WIFI page
The 2 most important pages are the [Setup](#setup) page, where the user can set up the mixer. And the [main landing page](#control) where the fancy UI is.
Setup is only really needed once during the setup of WiFi in Apmode, because in runtime the mixer settings can be adjusted in the main UI.

### Setup
The setup page will be accessible and shown in APmode, but can also be accessed by going to [mixer.local/ip](http://mixer.local/ip). 
The password is replaced with starts, and cannot be received without changing the firmware. 
Leave fields blank (or for the password leaving only stars) to not update those values upon sending this forum, set on space ' ' to clear them.
By default the SSID and password are limited to 16 characters by the firmware, and the total bytes that can be stored in memory is limited to 4096. Going over these values results in unexpected behavior. 

### Getting it’s IP
User devices that support mDNS, like Windows and Apple, can use [mixer.local/info](http://mixer.local/info) to obtain its IP.

Alternatively in a lot of stages (like after boot) the IP will be shown on the display.

### Control
The control page is the default landing page the user will land on. Here you select, customize, and mix the selected drink. 

- **customize** Will give a pop-up window to customize and mix your drink.
- **MIX** Will start mixing it right away.
- **Settings button** The gear on the right lower corner will open the settings page.

#### Settings button
- Some [Soft settings](#Soft-settings) are exposed here
- **OTA** will direct to the [OTA](#ota-over-the-air-update) update page, where the firmware can be updated over the WiFi.
- **Reset** will fully restart the ESP.
- **Info** will open the info page with some information like the version compile date.
- **Saved settings** just redirects you to [mixer.local/ip](http://mixer.local/ip) to show you the values saved in the EEPROM.

### Soft settings

### OTA (Over The Air update)
This page can be accessed on [mixer.local/ota](http://mixer.local/ota) (or 'IP/ota') and enables you to update firmware over WiFi.
On this page is a 'choose file' button where the new version can be selected. Make sure the right, compatible, most updated file is selected ("Mixer.bin"). This can be downloaded from [releases](https://github.com/jellewie/Arduino-CocktailMachine/releases). 
After which the 'Upload' button needs to be pressed for the update process to begin, the unit will automatically update and reboot afterward.
Note that [SoftSettings](#soft-settings) are preserved.

### Full reset
If a full reset is desired it can be achieved by going to 'mixer.local/reset'. But note that accessing the page will directly wipe all [SoftSettings](#soft-settings) from existence and there will be no way to restore them back. If the wipe was successful it will be reported back and will execute a restart.

      
# Appendix
* Firmware
[This is included in this repository](Arduino)
* PCB & schematic
[This is included in this repository](Schematic-PCB)
Beta/ updated version on [EasyEDA](https://easyeda.com/selbiekoekie/cocktail-machine)
* BOM
[This is included in this repository](BOM.md)
