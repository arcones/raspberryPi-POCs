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
- Two finite state machines (*fsm*) are concurrently used and managed via interruptions:

···The first one is switching ON and OFF columns secuentially each 25 ms

···The second one detects if any key is pressed and if so, prints the key value in screen

- To avoid debounces, the key detection is switched off 325ms just after one key is pressed

## Additional resources
- [Info about finite state machines in wikipedia](https://en.wikipedia.org/wiki/Mealy_machine)
