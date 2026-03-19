# Starry Sky Ceiling with Rain Sound Player Project

The home project for:

- keeping the CHINLY Bluetooth 16 W RGBW LED Fibre Optic Star Ceiling Lights Kit App connected and manageable with the
  Home Assistant.
- rain sound player using ESP32 and DFPlayer

# Idea #

The project started when my kid wanted a starry sky ceiling, like his friend had something similar implemented.
Engineer, like I am, started investigating different options and ideas how to make the ceiling as fancy as possible,
while at the same time thinking about a technical approach for the implementation.

Since the apartment had different "smart" thingies connected through the Home Assistant, the natural course was to
include everything manageable from the "smart" ceiling to the Home Assistant.

![Sky ceiling](../images/sky-1.jpg)

# Devices #

## Zigbee power switch ##

The live wire used for the ceiling has a Zigbee manageable switch to easily turn everything off, whether for the
inspection time, or during the vacation period. Zigbee has a strong and tight implementation within the Home Assistant,
nothing had to be developed.

## WLED strip ##

There's already a nice integration for the WLED on
the [Home Assistant](https://www.home-assistant.io/integrations/wled/). Not perfect, IMHO, with all these extra segments
and master light switch, but for the simple one-segment strip and tons of effects, it's excellent for the current usage
of the WLED strip.

## Starry sky ##

I bought
the [CHINLY Bluetooth 16 W RGBW LED Fibre Optic Star Ceiling Lights Kit App](https://www.amazon.de/dp/B08XJQM47T) from
Amazon.de. While the kit has a generic application for changing the light colors, and generic remote control, the kit
lacks any connectivity with the Home Assistant. The Internet knows about the kit, there are some integrations that try
to add the kit to the Home Assistant, but they are mostly capped by the string name the Kit exposes through BLE. Having
this kit exposing something completely different, there was no integration I could go of with.

The solution was inspecting the BLE stack between my phone and the kit. After writing everything to the Excel
file, finding patterns in the messages, I started writing message manipulation program with the C/C++ and YAML file for
calling the methods from the manipulation program using lambda calls.

The built binary is too heavy for standard ESP32, so I created the build for the ESP32-S3, but feel free to play with
other files

### Setup ###

#### Wi-Fi Networks ####

Create a local network file from the example one:

``` 
cp base/wifi_networks.example.yaml base/wifi_networks.yaml 
```

Edit the `base/wifi_networks.yaml` and add as many networks as you need. Git ignores the file, so your credentials stay
local.

The build has access point mode enabled on the node. Default credentials are ssid: `SSC`, password: `ssc12345`, but if
for
any reason you want them different, change them from the command line:

```
esphome -s ap_ssid 'New AP SSID' -s ap_password 'New AP Password' config/compile sounder.yaml
```

#### Device name ####

The same approach used for changing the Access Point values in the Wi-Fi subsection goes for the name of the ESP32 device
being displayed on the Home Assistant dashboard. If, for any reason you want to change the default name of the device to
something more meaningful to you, replace it with the following command:
```
esphome -s name 'starry-sky-ble' -s friendly_name 'Starry Sky BLE' config/compile sounder.yaml
```
