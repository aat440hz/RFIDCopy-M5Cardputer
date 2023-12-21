# RFID Cloner for M5Cardputer

## Overview

This RFID Cloner project is designed for the M5Cardputer, a portable ESP32-based development kit with a built-in display and speaker. The project allows you to read and clone MIFARE Classic RFID cards using an MFRC522 RFID module and the M5Cardputer.

## Features

- Read and display information from MIFARE Classic RFID cards.
- Clone MIFARE Classic RFID cards by writing the UID to another card.

## Hardware Requirements

To set up this project, you will need the following hardware:

- M5Cardputer
- MFRC522 RFID module
- MIFARE Classic RFID cards

## Installation

1. Clone this repository to your local environment.
2. Upload the provided code to your M5Cardputer using the Arduino IDE or PlatformIO.
3. Connect the MFRC522 RFID module to your M5Cardputer.
4. Power on the M5Cardputer.

## Usage

- When you power on the M5Cardputer, it will be in "Read Mode." Press the 'A' button to switch between "Read Mode" and "Write Mode."
- In "Read Mode," the device will read and display information from MIFARE Classic RFID cards.
- In "Write Mode," you can write the UID of a card to another card by presenting it to the MFRC522 module.

## Troubleshooting

- If you encounter issues with display text, the code includes adjustments to minimize text overlap by reducing text size and adjusting the error message position.
- Ensure that your hardware connections are correct.
- Make sure you have MIFARE Classic RFID cards available for reading and cloning.

## Contributions

Contributions to this project are welcome. Feel free to fork this repository and submit pull requests for improvements or new features.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- This project is based on the MFRC522 library.
- Developed for educational and experimental purposes.
---
https://github.com/Sarah-C/M5Stack_Core_MFRC522_RFID_Cloner
