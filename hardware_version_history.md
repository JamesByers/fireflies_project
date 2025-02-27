# <img src="./assets/Fireflies_logo_2_0.png" height = 64px > The Fireflies Project - Hardware Revision History
This page provides information about each revision of the V1 (original) and V2 (Fireflies Hue) hardware implementations.

### V2 Hardware (Fireflies Hue)

**V2**, also known as Fireflies Hue adds the ability to read addition inputs.  An eight position rotary switch replaces the v1 toggle switch. Three additional potentiometers are added to control hue, number of concurrent fireflies, and percent mix of two colors.

<p align="left">
<img src="./assets/Fireflies_hue_2_2_0s.png" width=400px)
</p>

**Board v2.2.1**
* Adjusted location of screw holes to more closely align with the project box

**Board v2.2.0**
* Moved the polyfuse onto the board
* Changed the C2 capacitor from 25V to 16V

**Board v2.1.1**
* Improved the routing to the 8 position rotary switch.  All traces now on top copper

<p align="left">
<img src="./assets/Fireflies_hue_v2_1_1_3d.png" width=400px)
</p>
	
**Board v2.1.0**
* Upgraded rotary switch support to eight selections instead of seven.
* Removed jumper used to force default settings, freeing up a pin for the rotary switch.
* Added a top copper pour to reduce environmental impact of circuit board manufacture.
* Added support for 5k1 Ohm resistors on board for quicker build for forcing 3A USB-C current level.
* Positioned the small caps much closer to the XOR gate.
* Changed input and output pads to through hole for better mechanical strength

<table>
  <tr>
    <td>
    	<img src="./assets/Fireflies_hue_v2_1_0_top.png"  alt="1" width = 400px >
	</td>
    <td>
    	<img src="./assets/Fireflies_hue_v2_1_0_3d_top.png" alt="2" width = 400px >
    </td>
  </tr> 
</table>

	
**Board v2.0.0**
- Replaced toggle switch with rotary switch.
- Added support for three additional potentiometers.
- Added support for a RS485 daughter board to support long distances to the first LED.  Designed to handle distances of up to 100m.
- Added new custom Firefly Hue logo.

<p align="left">
<img src="./assets/Fireflies_Hue_v2_0_0_with_parts.png" width = 400)
</p>

### V1 Hardware - The original Fireflies LED Controller!
<table>
  <tr>
    <td>
    	<img src="./assets/Fireflies_std_v1_0_2.png"  alt="1" width = 400px >
	</td>
    <td>
    	<img src="./assets/Fireflies_v1_box_w_lid.jpg" alt="2" width = 400px >
    </td>
  </tr> 
</table>

* **Board v1.0.2**
  * Final V1 board design
  * Improved the layout for more effective ground plane and power distribution
* **Board v1.0.1**
  * Added a ground plane to the PCB.
  * Added .1uF capacitor for better power management for XOR gate.
* **Board v1.0.0**
  * Printed circuit board implementation fits in a readily [100X68X50mm project box](https://www.amazon.com/gp/product/B07RTYYHK7).
  * First PCB design (Printed Circuit Board)
  * Changed to a RP2024 Zero microcontroller from a Pi Pico
  * Replaced previous level shifter with an high speed XOR gate to shift the high speed signal from 3.3V to 5V.
  * Added a DIP switch to select between:
    * RGB Color order to assure the LED controller is compatible with all WS2812 RGB strings.
    * LED string lengths of 50, 100, 150, and 200 LED strings
    * Select alternate colors

### Pi Pico RP2040 original prototype

It worked! But barely.

I learned from the prototype that the 3.3V-5V level shifter I was using actually *reduced* the voltage of the 800 kHz communication to the LEDs.  Yikes!

![](./assets/Fireflies_pi_pico_prototype.jpg)



