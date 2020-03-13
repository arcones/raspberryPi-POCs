# matrix-keyboard

Program to read a matrix keyboard connected to the RaspberryPi.

## Requirements 
- RaspberryPi with [wiringPi](http://wiringpi.com/) lib installed
- [Matrix keyboard](img/matrix-keyboard.jpg)
- [Jumper wire (8 female-male)](img/jumper-wire-female-male.jpg)

## Proposed wiring diagram
To connect the matrix keyboard to the raspberryPi, 8 connections must be done, 4 for the columns and 4 for the rows.
Following table is a proposal of how to connect the jumper wires from the matrix keyboard to the raspberryPi.
I have used a raspberryPi 4B but you can use execute wiringPi's function `gpio readall` in your raspberryPi terminal to check your GPIO board and its pins to make your own version.

| BCM | Name  | Mode | Connection | Color |
|:---:|:-----:|:----:|:----------:|:-----:|
| 0   |SDA.0  |IN    | COL_1      | Yellow|
| 1   |SCL.0  |IN    | COL_2      | Orange|
| 2   |SDA.1  |IN    | COL_3      | Red   |
| 3   |SCL.1  |IN    | COL_4      | Brown |
| 5   |GPIO.21|IN    | ROW_1      | Grey  |
| 6   |GPIO.22|IN    | ROW_2      | Purple|
| 12  |GPIO.26|IN    | ROW_3      | Blue  |
| 13  |GPIO.23|IN    | ROW_4      | Green |

## Implementation

