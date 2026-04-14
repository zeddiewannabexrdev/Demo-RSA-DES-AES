# Cryptography Lab Suite (AES, DES, RSA)

A professional desktop application designed for studying and demonstrating core cryptographic algorithms. This project provides a user-friendly graphical interface (GUI) to perform encryption and decryption using **AES-256**, **DES**, and **RSA-2048**.

---

## Features

- **Modern Desktop GUI**: Built with **Dear ImGui**, providing a lightweight and high-performance "hacker-style" interface.
- **Support for Major Algorithms**:
    - **AES-256 (CBC)**: High-security symmetric encryption.
    - **DES (CBC)**: Classic data encryption standard for educational use.
    - **RSA-2048 (OAEP)**: Public-key/asymmetric encryption with key pair generation.
- **Efficient & Portable**: CMake automatically manages all dependencies (GLFW, ImGui) via FetchContent.
- **Real-Time Logging**: Monitor the encryption/decryption process through an integrated status console.

---

## Project Structure

```text
├── src/
│   ├── core/           # Modularized C++ encryption logic
│   │   ├── CryptoService.h
│   │   └── CryptoService.cpp
│   ├── ui/             # Graphical User Interface code (ImGui)
│   │   ├── ImGuiApp.h
│   │   └── ImGuiApp.cpp
│   └── main_imgui.cpp   # Application entry point (GLFW + OpenGL3)
├── CMakeLists.txt      # Build configuration (Automatic dependency fetch)
└── README.md           # Documentation
```

---

## Getting Started

### Prerequisites

To build and run this project, you will need:
- **C++ Compiler** (MSVC, GCC, or Clang)
- **CMake** (v3.16 or higher)
- **OpenGL Drivers** (Standard on most systems)
- **Crypto++ Library** (installed and accessible in your environment)

### Installation (Crypto++)

#### Windows (with vcpkg):
```bash
vcpkg install cryptopp:x64-windows
```

#### Linux (Ubuntu/Debian):
```bash
sudo apt-get install libcryptopp-dev
```

---

## Build Instructions

### Using Bash/Terminal:
1. **Clone the repository** (if applicable).
2. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```
3. **Configure with CMake**:
   ```bash
   cmake ..
   ```
4. **Compile the application**:
   ```bash
   cmake --build . --config Release
   ```
5. **Run the app**:
   ```bash
   ./Release/CryptoLabSuite
   ```

### Using PowerShell:
1. **Create and enter build directory**:
   ```powershell
   New-Item -ItemType Directory -Force -Path build
   Set-Location build
   ```
2. **Configure with CMake**:
   ```powershell
   cmake ..
   ```
3. **Compile the application**:
   ```powershell
   cmake --build . --config Release
   ```
4. **Run the app**:
   ```powershell
   .\Release\CryptoLabSuite.exe
   ```

---

## How to Use

1. **Select Algorithm**: Choose the tab for AES, DES, or RSA.
2. **Select Files**:
    - Click **Browse** (or type) to select the input file you want to encrypt.
    - Provide names for the Secret Key and Output binary files.
3. **Execute**:
    - Click **Encrypt** to convert your file into ciphertext.
    - Provide the key file and click **Decrypt** to recover the original content.
4. **RSA Only**: Use the **Gen RSA Keys** button first to create your public/private key pair.

---

## Important Security Note
This application is intended for **academic and demonstration purposes**. For production-grade security, always follow industry-standard practices for key storage and entropy management.

---

## Credits
Developed using the **Crypto++** library and **Dear ImGui**.
