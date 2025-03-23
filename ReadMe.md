# **This project is still in development!**

# Cockail Mixer Machine
A machine that can mix drinks

<img src="Images/CAD overview.png" alt="CAD drawing of the machine">

## Firmware
The firmware needs to be flashed once to enable OTA BIN file upload. Either with Arduino IDE and compile it, or using a BIN uploader of choice.

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
- [FreeCAD] (https://github.com/FreeCAD/FreeCAD) is used for 3D modeling.
- [A2Plus_Workbench] (https://github.com/kbwbe/A2plus) is used for creating the assembly.

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
The LED on the ESP (LED_BUILDIN) reflects these errors:
- **ON** WIFI starts, goes OFF when WIFI setup is successfully completed.
- **blink 100ms** WIFI setup failed, APmode has started.
- **blink 500ms** It is connecting to the given WIFI.

The LED strip shows different statuses depending on the operational status.
The LED strip will show the following effects when said action is being performed:
- Show lighting effect when just booted (ColorBoot)
- Show lighting effect when Homing (ColorHoming)
- Show lighting effect when Homing Failed (ColorHomeFail)
- Shows rainbow effect when finished/idle
- Shows lighting effect on set dispenser when in use (ColorMoveBase + ColorMoveActive)
- LEDs will be at 25% brightness after being idle for DisableSteppersAfterIdleS

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
There are multiple soft settings, these are settings that can be changed after the sketch has been uploaded, but are still saved so they can be restored after a reboot.
The most up-to-date values can be found in the top of the [WiFiManagerBefore.h](Arduino/WiFiManagerBefore.h) source file, and can only be set in [mixer.local/ip](http://mixer.local/ip).
These settings are auto saved 30000ms after the last change of SoftSettings, or directly after APmode and in the [mixer.local/ip](http://mixer.local/ip) page.
Note that the character " and TAB (EEPROM_Seperator) cannot be used, these will be replaced with ' and SPACE respectively. Leave black to skip updating these, use spaces ' ' to clear the values
- **MotorMAXSpeed** The max speed the motors can go
- **MotorMAXAccel** The max acceleration the motors can do
- **BedSize_X** **BedSize_Y** **BedSize_Z** The max size in steps the X motor can go
- **Manual_X** **Manual_Y** The X position to go to when done, and for the manual steps
- **ShotDispenserML** The amount of ML in a shot dispenser 
- **HomeMAXSpeed** The max speed of the second soft home
- **HomedistanceBounce** The amount to bounce back when the switch is triggered
- **DisableSteppersAfterDone** When set to true, will free and disable the steppers, although this saves heat and power, will require to home again.
- **MaxGlassSize** The max amount of mL that can fit in a glass
- **Reserved** Reserved spots for future things
- **D#** 20 Dispensers with their setup data, saved in the format 'Type,LocationX,LocationY,LocationZ,TimeMSML,TimeMSoff,IngredientID'

### OTA (Over The Air update)
This page can be accessed on [mixer.local/ota](http://mixer.local/ota) (or 'IP/ota') and enables you to update firmware over WiFi.
On this page is a 'choose file' button where the new version can be selected. Make sure the right, compatible, most updated file is selected ("Mixer.bin"). This can be downloaded from [releases](https://github.com/jellewie/Arduino-CocktailMachine/releases). 
After which the 'Upload' button needs to be pressed for the update process to begin, the unit will automatically update and reboot afterward.
Note that [SoftSettings](#soft-settings) are preserved.

### Full reset
If a full reset is desired it can be achieved by going to 'mixer.local/reset'. But note that accessing the page will directly wipe all [SoftSettings](#soft-settings) from existence and there will be no way to restore them back. If the wipe was successful it will be reported back and will execute a restart.

# Specifications 
## Input voltage
The power supply is 12V. 
the current draw and power consumption are as time of writing still unknown.

# Appendix
* Firmware
[This is included in this repository](Arduino)
* PCB & schematic
[This is included in this repository](Schematic-PCB)
Beta/ updated version on [EasyEDA](https://easyeda.com/selbiekoekie/cocktail-machine)
* BOM
[This is included in this repository](BOM.md)
