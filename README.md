# Cryptography Algorithms Demo (AES, DES, RSA)

This project is a collection of demonstration tools for basic cryptographic algorithms, intended for studying and researching **Information System Security**. The project is implemented in C++ using the powerful **Crypto++** library.

## Key Features
- **AES-256 Encryption (CBC mode)**: Advanced Encryption Standard with a 256-bit key.
- **DES-64 Encryption (CBC mode)**: Data Encryption Standard with a 64-bit key (for conceptual demonstration).
- **RSA-2048 Encryption (OAEP)**: The most popular public-key cryptography algorithm.

---

## Prerequisites & Installation

### 1. C++ Compiler
You need a C++ compiler such as `g++` (MinGW for Windows or GCC for Linux) or `MSVC`.

### 2. Crypto++ Library Installation
This project depends on the **Crypto++** library.

#### On Windows (MinGW/MSYS2):
```bash
pacman -S mingw-w64-x86_64-cryptopp
```

#### On Windows (vcpkg):
```bash
vcpkg install cryptopp:x64-windows
```

#### On Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install libcryptopp-dev libcryptopp-doc libcryptopp-utils
```

---

## Compilation and Execution Guide

Each algorithm is implemented in a separate, independent `.cpp` file. Follow the instructions below to run each demo:

### 1. AES (Advanced Encryption Standard)
Compile:
```bash
g++ AES_demo.cpp -o aes_tool -lcryptopp
```
Run:
```bash
./aes_tool
```

### 2. DES (Data Encryption Standard)
Compile:
```bash
g++ DES_demo.cpp -o des_tool -lcryptopp
```
Run:
```bash
./des_tool
```

### 3. RSA (Rivest–Shamir–Adleman)
Compile:
```bash
g++ RSA_demo.cpp -o rsa_tool -lcryptopp
```
Run:
```bash
./rsa_tool
```

---

## How it Works

When you run any of the files above, the program performs the following steps:

### Encryption Process (Option 1):
1.  **Key Generation**: The program generates a random Key and an Initialization Vector (IV/Salt).
2.  **File Reading**: Reads content from `input.txt` (if it doesn't exist, the program will automatically create a sample file).
3.  **Encryption Execution**: Uses the selected algorithm to encrypt the data.
4.  **File Output**: 
    - Saves the encrypted file as binary (`.bin`).
    - Saves the key into separate files (`.bin` for symmetric encryption, `.der` for RSA) for later decryption.

### Decryption Process (Option 2):
1.  **Key Loading**: Reloads the saved keys from the key files.
2.  **Ciphertext Reading**: Reads the binary data from the encrypted file.
3.  **Decryption Execution**: Converts the ciphertext back to the original plaintext.
4.  **Result Output**: Saves the decrypted result into a text file (`_decrypted.txt`) for verification.

---

## Important Notes
- The key files (`.bin`, `.der`) are essential for decryption. If these files are lost, the data cannot be recovered.
- This project is for educational/demo purposes only and should not be used in high-security production environments without professional refinement.

---

## File Structure
- `AES_demo.cpp`: AES symmetric encryption.
- `DES_demo.cpp`: DES symmetric encryption.
- `RSA_demo.cpp`: RSA asymmetric encryption.
- `input.txt`: Source file containing data to be encrypted.
- `README.md`: Project documentation.
