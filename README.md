# Controlled Ransomware Encryption Simulator
## Multi-Layer Behavior-Based Detection

A C++ application that simulates ransomware encryption behavior in a 
controlled lab environment while demonstrating how a behavioral detection 
engine identifies and halts the attack in real time.

## Features
- Real AES-256-CBC encryption via OpenSSL
- Shannon entropy analysis for encryption detection
- Three independent behavioral detection rules:
  - BurstRule: sliding time window burst detection
  - EntropyRule: entropy delta threshold detection
  - ExtensionChangeRule: mass file renaming detection
- Honeypot trap file detection
- KeyVault + Decryptor for full file recovery
- Persistent simulation log file

## Technologies
- C++ (C++17)
- OpenSSL 3.x
- MSVC / Visual Studio

## Purpose
Educational cybersecurity project demonstrating behavioral 
ransomware detection concepts.
German Jordanian University — CS492 Senior Project — 2026

## Warning
This simulator is for educational purposes only. 
Only run inside the designated lab folder.
