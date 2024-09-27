# The Fireflies LED  Controller Project

This project delivers both hardware and firmware for turning an addressable LED string into simulated fireflies

<video src="C:\git\fireflies_project\assets\IMG_0222.MOV"></video>

### Background
This project journey started in Fall of 2022 when I was experimenting with controlling led strings using microcontrollers to create outdoor Christmas lights.  A few months later I was reminiscing about seeing "lightning bugs" in summer while I was growing up.  The idea gelled to simulate the fireflies I remember so I could enjoy the effect any time of year.  

My first prototype used a Arduino Nano as the microcontroller.  It was able to support five simultaneous and supported three knobs to adjust RGB values so I could experiment with color choices.

I got a boost in the Fall of 2023 with a visit from a family member from the Midwest who has fireflies show up every year in her yard.  That allowed me to tune the color and duration to the fireflies I grew up with.  

Soon after that I decided to change a faster microcontroller RP2020 and found much more appropriate LED strings rather than the clunky plastic covered LED strips I had be working with.

That resulted in the first prototype using the Pi Pico RP2040 microcontroller that is still in use outdoors!  The journey has continues with an initial v1 and then the current V2 versions with increasing features.

<p align="left"><img src="./assets/Fireflies_v1_box_w_lid.jpg" width="400"></p>
### Hardware Revision History
#### V2 Hardware (Fireflies Hue)
<p align="left">
<img src="./assets/Fireflies_2_0_1_board_photo.png" width="400">
</p>
#### V1 Hardware
<p align="left"><img src="./assets/Fireflies_std_v1_0_2.png" width="400"></p>

Features

<p align="left"><img src="./assets/Fireflies_v1_box_w_lid.jpg" width="400"></p>

* Board v1.0.2 
  * Final V1 board design

  * Improved the layout for more effective ground plane and power distribution

* Board v1.0.1 
  * Added a ground plane to the PCB

  * Added 100 .1UF Capacitor

* Board v1.0.0
  * First PCB design (Printed Circuit Board)
  * Changed to a RP2024 Zero microcontroller from a Pi Pico
  * Replaced previous level shifter with an high speed XOR gate to shift the high speed signal from 3.3V to 5V.
  * Added a DIP switch to select between:

    * RGB Color order to assure the LED controller is compatible with all WS2812 RGB strings.
    * LED string lengths of 50, 100, 150, and 200 LED strings
    * Select alternate colors
  * New compact implementation fits in a common [100X68X50mm project box](https://www.amazon.com/gp/product/B07RTYYHK7).

#### Pi Pico RP2040 original prototype

It worked!

I learned from the prototype that the 3.3V-5V level shifter was actually *reducing the voltage* of the 800 kHz communication to the LEDs.  Yikes!

![](C:\Users\byerj023\Downloads\Fireflies_pi_pico_prototype.jpg)
