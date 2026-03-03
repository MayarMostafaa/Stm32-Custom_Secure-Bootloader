# Custom Secure Bootloader (STM32F446RE) 🔐

A custom secure bootloader project for **STM32F446RE** developed using **STM32CubeIDE**.
This project demonstrates the fundamentals of bootloader design on STM32, including firmware update flow, application handover, and secure validation concepts.

## 📌 Project Overview

This project implements a **custom bootloader** that runs before the main application and is responsible for:

* Initializing required peripherals
* Receiving / handling firmware update data 
* Verifying firmware integrity/authenticity (e.g., CRC/checksum/signature concept)
* Jumping safely to the user application

The goal of this project is to build a strong practical understanding of:

* STM32 boot flow
* Flash memory layout
* Vector table handling
* Application jump sequence
* Secure firmware update concepts

---

## 🛠️ Tools & Environment

* **MCU:** STM32F446RE
* **IDE:** STM32CubeIDE
* **Framework / HAL:** STM32Cube HAL (CubeMX-generated project structure)
* **Language:** C
* **Programmer/Debugger:** ST-LINK *(if used)*

---

## 📂 Project Structure

* `Core/` → Application source and headers
* `Drivers/` → STM32 HAL / CMSIS drivers
* `.ioc` → STM32CubeMX configuration file
* `.project`, `.cproject`, `.mxproject` → STM32CubeIDE project files
* `STM32F446RETX_FLASH.ld` → Flash linker script
* `STM32F446RETX_RAM.ld` → RAM linker script

> Build output folders such as `Debug/` and `Release/` are intentionally excluded from version control.

---

## 🔐 Secure Bootloader Concepts Covered

This project is focused on the **bootloader architecture and secure update concepts**, such as:

* Bootloader vs Application memory separation
* Firmware integrity verification (e.g., CRC/checksum)
* Controlled jump to application reset handler
* Flash programming workflow
* Protection against invalid application images (basic validation checks)

> **Note:** If cryptographic signature verification is not fully implemented yet, this project can still be considered a **secure bootloader prototype / educational implementation**.

---

## 🧠 Boot Flow 

1. MCU resets and starts execution from bootloader region
2. Bootloader initializes required peripherals
3. Bootloader checks for valid application image / update condition
4. If valid application exists → jump to user application
5. Otherwise → remain in bootloader mode (e.g., waiting for update)

---

## 🚀 How to Build

1. Open the project in **STM32CubeIDE**
2. Make sure the correct STM32 target is selected (**STM32F446RE**)
3. Build the project:

   * **Project → Build Project**
   * or use the hammer icon

---

## ⚡ How to Flash

1. Connect STM32 board via **ST-LINK**
2. Open project in STM32CubeIDE
3. Click **Run / Debug**
4. Program the target MCU

> Make sure the flash memory layout and linker settings match your bootloader/application design.

---

## 🧪 Notes / Future Improvements

Possible next improvements:

* Add cryptographic signature verification (RSA/ECDSA)
* Add rollback protection / versioning
* Add encrypted firmware updates
* Add UART/CAN-based firmware update protocol
* Add dual-image / fail-safe update mechanism
* Add detailed logging for update status and errors

---


## 👩‍💻 Author

**Mayar Shalaan**
Embedded Software Engineer | Interested in Embedded Systems, Linux, and Secure Firmware Development

---


