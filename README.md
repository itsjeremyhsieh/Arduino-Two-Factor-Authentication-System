# Arduino-Two-Factor-Authentication-System
This project is an implementation of a two-factor authentication system using an Arduino board, an RFIC card scanner, and a numeric keypad. The system works by requiring the user to first scan their RFIC card and then input a password via the numeric keypad. Only after both steps have been successfully completed will the system grant access.

## Requirements
To replicate this project, you will need the following components:

* Arduino board (such as an Arduino Uno)
* RFIC card scanner module (such as the MFRC522)
* LCD display
* Numeric keypad
* Breadboard
* Jumper wires
* LEDs and buzzer (optional, for visual feedback)
## Installation
Download the code from the GitHub repository.
Connect the components as follows:

![rfid_psw_bb](https://user-images.githubusercontent.com/94349736/233986981-418d6b81-a1fb-4f48-baa7-63fbb6361a37.png)

Upload the code to your Arduino board using the Arduino IDE.

## Usage
Power on the system and wait for the "Please scan card" message to appear on the display

Scan your RFIC card by holding it close to the card scanner module. If the card is recognized, the LED connected to pin A3 will light up and the system will prompt you to enter your password.

Enter your password using the numeric keypad. If the password is correct, the green LED connected to pin A3 will light up and the system will grant access. If the password is incorrect, the red LED connected to pin A4 will turn on and the system will prompt you to try again.

After access is granted, the system will display a welcome message with the username displayed. 
After 2 seconds, the system is reset and prompt you to scan your card again for the next user.

## Acknowledgments
This project was inspired by the concept of two-factor authentication and the need for enhanced security in personal and professional settings.
