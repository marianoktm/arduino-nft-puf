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

## How to run [WIP]
As previously said, this project consists in three different components.

### 0. Some compatibilities notes [WIP]
- The PUF component is expected to run without problems on Arduino Uno R3 boards. We tested it with Arduino Uno R4 boards without success. 
- The various scripts needed to compile the bootloader were tested on Ubuntu, Manjaro with success. We encountered issues on MacBooks with Apple Silicon processors. We did not test it with Windows. We strongly recommend to use Ubuntu Linux during the PUF steps.
- The whole project is a **PROOF OF CONCEPT**! We do not hold any liability for any use you make of it. We **DO NOT** recommend to deploy it on a production network or on the mainnet. 
- The Smart Contract was successfully deployed on Hardhat on Manjaro and MacBooks with Apple Silicon. It was also positively tested on Remix.

### 1. Generating an SRAM-PUF [WIP]
In order to generate the PUF the intrinsic randomness of the SRAM at startup will be used. There's the need to overwrite the original bootloader because it initializes the SRAM, and thus the startup SRAM data can't be read.

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

### 3. Interacting with the NFT [WIP]


## Authors
This project was developed by:
- **Simone D'Angelo**
- **Mariano Aponte**
- **Sergio Aprea**

## Acknowledgments
We extend our gratitude to Dr. Franco Cirillo, for providing us the hardware and software needed to implement the PUF.

