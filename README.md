# Arduino-NFT-PUF
A joint project for the IoT Security and Data Security courses of the Master's Degree in Computer Science at the University of Salerno.

## Overview
Arduino-NFT-PUF is an innovative approach to device fingerprinting and ownership verification by leveraging SRAM Physical Unclonable Functions (PUFs) and blockchain technology. This project enables unique identification of Arduino devices through their intrinsic hardware characteristics and secures ownership via Non-Fungible Tokens (NFTs).

## Key Features
This work consists in three main components:

### Arduino SRAM PUF
We implemented a PUF for Arduino devices through the inherent randomness in SRAM startup values. 

A custom bootloader and some data manipulation are needed in order to obtain a stable PUF response. 

We have to thank Dr. Franco Cirillo to provide us the hardware and software needed to accomplish so.

### NFT Smart Contract
We developed a simple NFT Smart Contract (compliant to ERC-721 standard) in order to link an Ethereum account to a PUF. 

The contract is writted in Solidity and tested on an Ethereum testnet through Hardhat.

### DApp
We developed a simple DApp in order to interact with the Smart Contract. 

It is possible to log in into the app through metamask, and mint NFTs through it. The app works through Web3.js.

## How to run

### Various dependencies and programs
For the Arduino SRAM PUF:
- Arduino IDE
- avrdude
- avr-gcc
- scons
- minicom

In addition, the following hardware components are needed:
- Two Arduino Uno R3
- 6 cables to connect the boards
- An USB cable to connect the board to a PC

For the NFT Smart Contract:
- Hardhat (as a testnet, but any testnet should do the trick)
- Solidity

For the DApp:
- Javascript
- Web3.js
- [WIP]

### 0. Some compatibilities notes [WIP]
- The PUF component is expected to run without problems on Arduino Uno R3 boards. We tested it with Arduino Uno R4 boards without success. 
- The various scripts needed to compile the bootloader were tested on Ubuntu, Manjaro with success. We encountered issues on MacBooks with Apple Silicon processors. We did not test it with Windows. We strongly recommend to use Ubuntu Linux during the PUF steps.
- The whole project is a **PROOF OF CONCEPT**! We do not hold any liability for any use you make of it. We **DO NOT** recommend to deploy it on a production network or on the mainnet. 
- The Smart Contract was successfully deployed on Hardhat on Manjaro and MacBooks with Apple Silicon. It was also positively tested on Remix.

### 1. Generating an SRAM-PUF [WIP]
A **Physical(ly) Unclonable Function (PUF)** leverages the intrinsic randomness of the hardware to derivate an unique identifier.
In order to generate the PUF the intrinsic randomness of the SRAM at startup will be used. 
There's the need to overwrite the original bootloader because it initializes the SRAM, and thus the startup SRAM data can't be read.

#### Using an Arduino Board as an ISP
In order to load a custom bootloader on a board, we need to use another board as an ISP. We will then have a "programmer" board and a "target" board. The programmer will load the custom bootloader on the target board.

To allow a board as a Programmer, follow these steps:
1. Choose one of the two boards as a programmer and connect it to the PC;
2. Open Arduino IDE;
3. Go to **Files -> Examples -> 11. Arduino ISP** and select the `ArduinoISP` sketch;
4. Load the sketch on the programmer board;
5. Next, go to **Tools -> Programmer** and select **Arduino as ISP**. 

Now the two boards can be connected using the following scheme:
| **Programmer Pins** | **Target Pins** |
|---------------------|-----------------|
| PIN 10              | RESET           |
| PIN 11              | PIN 11          |
| PIN 12              | PIN 12          |
| PIN 13              | PIN 13          |
| 5V                  | 5V              |
| GND                 | GND             |

#### Compiling and loading the Custom Bootloader
Download the content of `Arduino PUF` and follow these steps:
1. Connect the Programmer board to the Target board using the above table;
2. Connect the Programmer board to the PC;
3. Go to `scripts` and compile the bootloader using `./compile_bootloader.sh`. The compiled bootloader will be available in `build`.
4. Go to `scripts` and run `./flash_bootloader.sh <path to bootloader.hex>`

#### Obtaining two unstable PUFs
We will use minicom to obtain two instable PUFs from the Target board:
1. Disconnect the Target board from the Programmer board;
2. Disconnect the Programmer board from the PC;
3. Connect only the Target board to the PC;
4. Check the serial port on which the board is connected using `ls /dev/tty:.*`;
5. Write down the port name (for instance `/dev/ttyUSB0` or `/dev/ttyACM0`);
6. Open two terminals and execute `minicom` on both using the correct port, for instance `minicom -D /dev/tty.usb_portname -b 115200 -C file1.txt` and `minicom -D /dev/tty.usb_portname -b 115200 -C file2.txt`. As an important note, the Target board must be disconnected and reconnected between minicom executions;
7. Extract the instable PUFs from the two files and copy them into `enrollement/dump/new_arduino_1_0.puf` and `enrollement/dump/new_arduino_1_1.puf`.

#### Obtaining a stable PUF
Now we will extract a stable PUF:
- Execute `./enroller > helperdata.txt`.

In this file there will be an `helper_data` field. Copy the string and convert it as follows:
1. Add `0x` before each byte;
2. Separe each byte with a comma. For instance, `466f727a61204e61706f6c69` becomes `0x46, 0x6f, 0x72, 0x7a, 0x61, 0x20, 0x4e, 0x61, 0x70, 0x6f, 0x6c, 0x69`.

Now go to `bootloader` and locate the `puf.c` file. Insert the converted helper data at the following row:
```c 
   const uint8_t helper_data[304] PROGMEM = {/*insert here the converted helper data*/};
```

Compile and load the bootloader again.

Now we can extract a stable PUF:
1. Connect the Target board to the PC;
2. Check the serial port on which the board is connected using `ls /dev/tty:.*`;
3. Write down the port name (for instance `/dev/ttyUSB0` or `/dev/ttyACM0`);
4. Open a terminal and run `minicom -D /dev/tty.usb_nomeporta -b 115200 -C puf_response.txt`. To print the stable PUF digit `x` into the minicom terminal.
5. Now the Stable PUF is saved into `puf_response.txt` and can be used as desired.

### 2. Deploying the NFT on Hardhat testnet [WIP]
In order to deploy the NFT to a testnet, a new hardhat project should be initialized.

1. Create a new directory and initialize a npm project with `npm init`;
2. Install hardhat with `npm install --save-dev hardhat`;
3. Install the needed dependency with `npm install @openzeppelin/contracts`;
4. Create a hardhat project with `npx hardhat init`. Choose the Javascript project;
5. Delete the sample contract and the sample deployment scripts;
6. Download the Smart Contract and move it into `contracts`;
7. Download the deployment script and move it into `ignition/modules/`;
8. Compile the contract using `npx hardhat compile`;
9. From another terminal window, run the hardhat testnet using `npx hardhat node`;
10. From the root of the project, run `npx hardhat ignition deploy ignition/modules/ArduinoNFT.js --network localhost`.
11. Now the contract is deployed and it is possible to interact with it.

Note that the deployment script `ArduinoNFT.js` sets the account "0" as the owner of the contract. If another owner is needed or desired, it can be selected by modifying the row `const owner = m.getAccount(0);`.

### 3. Interacting with the NFT [WIP]


## Authors
This project was developed by:
- **Simone D'Angelo**
- **Mariano Aponte**
- **Sergio Aprea**

## Acknowledgments
We extend our gratitude to Dr. Franco Cirillo, for providing us the hardware and software needed to implement the PUF.

