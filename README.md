# Quantum Key Distribution: BB84 Protocol Simulation

BB84 Protocol Simulation by Muhammad Haris Mubashar

## Table of Contents

- [Introduction](#introduction)
- [Conventions](#conventions)
- [Protocol](#protocol)
- [Getting Started](#getting-started)
- [Usage](#usage)

## Introduction

The BB84 protocol is a key distribution protocol which uses photons and quantum phenomon to distribute keys (from Alice to Bob in our example).
In practice, the protocol utilized uncertainty and quantum superposition in order to work as described in the simulation.
The simulation is intended to show how the protocol would work while using probabilities we would expect to see in practice.


## Conventions

We will use the following bit encoding convention in the simulation:
- Using the following bit encoding:
    - Rectilinear Basis (r):
        - 0 is encoded to H (Horizontal), ↔ (0°)
        - 1 is encoded to V (Vertical), ↕ (90°)
    - Diagonal Basis (d):
        - 0 is encoded to R (45° Right), ⤢ (45°)
        - 1 is encoded to L (-45° Left), ⤡ (-45°)

The inputs for encryption and decryption basis will be strings consisting of character "r" and "d", where "r" represents a rectilinear basis, and "d" represents a diagonal basis.

Once the encryption algorithm uses the encryption basis to encrypt the binary message, the message will consist of characters "H", "V", "R", and "L" (as shown above) where:
- "H" represents Horizontal Rectilinear (↔, 0°) polarization
- "V" represents Vertical Rectilinear (↕ 90°) polarization
- "R" represents "Right" Diagonal (⤢ 45°) polarization
- "L" represents "Left" Diagonal (⤡ -45°) polarization

## Protocol

Here is the approach of the BB84 protocol which the simulation will demonstrate:

Protocol:
1. Alice chooses a bit string
2. Alice randomly chooses an encoding basis (Rectilinear Basis or Diagonal Basis) for each bit
3. Using the encoding basis, Alice generates encoded quantum states for the bits and sends them to Bob
4. Bob randomly chooses a decoding basis (Rectilinear Basis or Diagonal Basis) for each bit
5. Using the decoding basis, Bob decodes the quantum states that Alice has sent. As a result, Bob now has a bit string as well (not necessarily the same as Alice’s bit string)
6. Bob publicly states the decoding basis he used.
7. Alice checks the encoding basis Bob used for each bit with the decoding basis she used for each bit. If the basis used by both her and Bob are the same for a bit, Alice informs Bob that the bit is suitable for use. The bits for which different basis were used are discarded. The bits that are suitable for use are kept as shared information (key)

At this point, Bob would reveals some of the bits to check for the presence of Eve, and Alice confirms the bits to check whether eavesdropping occurred.

## Getting Started

To get a local copy up and running, follow these steps:\
Clone the repo using the command:
- git clone https://github.com/harismubashar/bb84_simulation.git

or
- Save the files (bb84_sim.c, Makefile) locally

Traverse to the cloned directory (or the directory with the files) and run the Makefile using:
- make

This will create the executable bb84_sim in the directory

## Usage

    To run a quick simulation without any specified parameters, you may simply run (this will choose a random message of length 10-100):
        ./bb84_sim
    To run a simulation of the protocol with specified message, but random encryption and decryption basis, follow the format:
        ./bb84_sim <binary message>
    To run a simulation with chosen message, encryption and decryption basis, follow the format:
        ./bb84_sim <binary message> -e <encryption basis> -d <decription basis>
    Note that the encryption basis and decryption basis must be of the same length as the message
    You may also opt to choose only an encryption basis or decryption basis and let the other one be randomized like so:
        ./bb84_sim <binary message> -e <encryption basis>
        ./bb84_sim <binary message> -d <decription basis>

