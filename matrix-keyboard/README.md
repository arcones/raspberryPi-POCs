# matrix-keyboard

Program to read a matrix keyboard connected to the RaspberryPi.

## Requirements 
- RaspberryPi with [wiringPi](http://wiringpi.com/) lib installed
- [Matrix keyboard](img/matrix-keyboard.jpg)
- [Jumper wire (8 female-male)](img/jumper-wire-female-male.jpg)

## Proposed wiring diagram
To connect the matrix keyboard to the raspberryPi, 8 connections must be done, 4 for the columns and 4 for the rows.

Following table is a proposal of how to connect the jumper wires from the matrix keyboard to the raspberryPi.

As in code I have used BCM pin codes, implementation should work in any rasberryPi model.

You can execute wiringPi's function `gpio readall` in your raspberryPi terminal to check where your GPIO board has the BCM pins used.

| BCM | Mode | Connection |
|:---:|:----:|:----------:|
| 0   |OUT   | COL_1      |
| 1   |OUT   | COL_2      |
| 2   |OUT   | COL_3      |
| 3   |OUT   | COL_4      |
| 5   |IN    | ROW_1      |
| 6   |IN    | ROW_2      |
| 12  |IN    | ROW_3      |
| 13  |IN    | ROW_4      |

## Implementation
The implementation relies in [Mealy machines](https://en.wikipedia.org/wiki/Mealy_machine) which are finite state machines.
Two Mealy machines has been implemented and are concurrently working:

![alt text](img/mealy-diagram.png "Mealy Diagram")

 - The first one is switching ON and OFF columns periodically each 25ms, that is, starting with only the first column with high voltage on it, after 25ms it switches OFF the first column and ON the second, and so on; when the 25ms are done on the fourth column which is the last, the process starts again with the first. This short period of change allow to read every column during a normal keystroke.
 - The second one detects the key pressed and prints the key value in screen.

To avoid debounces, the key detection is switched off 250ms just after every keystroke.