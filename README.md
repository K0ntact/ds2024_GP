# Peer-to-peer Chat System Based on Message Passing Interface (MPI)

## Description

In this project, our objective is creating a P2P chat system using MPI.

## Table of Contents

- [Design](#design)
- [Installation](#installation)
<!-- - [](#contributing)
- [License](#license) -->

## Design
The design of our system is descirbes with details in the README.pdf.

## Installation and Usage
We will use OpenMPI compiler, so if you did not install it, get it by the command:
```bash
sudo pacman -S openmpi
```
for who use Arch Linux \\
Or
```bash
sudo apt install openmpi
```
for Ubuntu

Next, clone the source code.

First, compile the files using the following command:
```bash
mpic++ peer1.cpp -o peer1
mpic++ peer2.cpp -o peer2
```

Then, run the program in the following order:
```bash
mpiexec --report-uri - -n 1 ./peer1
```
The output will be something like this:
```
12345670.0;tcp4://192.168.1.8:44379
```
Run the second peer in a separate terminal with the following command:
```bash
mpiexec --dvm uri:"12345670.0;tcp4://192.168.1.8:44379" -n 1 ./peer2
```

To chat, pipe your message to the named pipe of the peer on your machine (myfifo1 for peer1, myfifo2 for peer2).
For example, to send a message from peer1 to peer2:
```bash
echo "Hello, peer2!" > myfifo1
```
Similarly, to send a message from peer2 to peer1:
```bash
echo "Hello, peer1!" > myfifo2
```
