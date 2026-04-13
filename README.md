# 🚀 Unmanned Rocket Project

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Platform](https://img.shields.io/badge/platform-Arduino-teal.svg)
![Status](https://img.shields.io/badge/status-academic%20project-orange.svg)

An Arduino-based unmanned rocket payload system that collects atmospheric and kinematic data during flight. Developed as part of the PHY-220 course on unmanned vehicles and payload implementation.

---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Hardware Requirements](#hardware-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Data & Analysis](#data--analysis)
- [Contributing](#contributing)
- [License](#license)

---

## About

This project is part of a course on unmanned vehicles and payload systems, which covers the design and implementation of payloads for rockets, balloons, robotic rovers, and drones. The rocket payload gathers atmospheric and kinematic data during flight, which is then analyzed and presented as part of the course deliverables.

---

## Features

- Collects real-time atmospheric data (altitude, pressure, temperature) during rocket flight
- Logs kinematic data (acceleration, orientation) using onboard sensors
- Custom PCB design for compact and reliable in-flight electronics integration
- Arduino firmware written in C++ for sensor interfacing and data management
- Structured data output for post-flight analysis

---

## Tech Stack

| Category     | Tools / Technologies                        |
|--------------|---------------------------------------------|
| Firmware     | C++, Arduino IDE                            |
| Microcontroller | Arduino (AVR-based)                      |
| PCB Design   | Custom PCB (files in `/PCB`)                |
| Sensors      | IMU, barometric pressure sensor (onboard)   |
| Data Output  | Serial logging / onboard storage            |

---

## Project Structure

```
Rocket-Project/
├── Arduino/           # Arduino firmware source code (.ino / .cpp files)
│   └── ...            # Sensor drivers, data logging, flight logic
├── PCB/               # PCB design files (schematics, layout, Gerbers)
│   └── ...            # Custom board design for the payload
└── README.md
```

---

## Hardware Requirements

- Arduino board (e.g., Arduino Uno, Nano, or Mega)
- IMU sensor (e.g., MPU-6050) for acceleration and orientation
- Barometric pressure sensor (e.g., BMP280 or BMP180) for altitude and temperature
- Custom PCB (see `/PCB` folder for design files)
- LiPo or alkaline battery pack suitable for flight
- USB cable for programming and serial monitoring

---

## Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/DyDyVNR/Rocket-Project.git
   cd Rocket-Project
   ```

2. **Open the firmware in Arduino IDE**
   - Launch [Arduino IDE](https://www.arduino.cc/en/software)
   - Go to `File > Open` and select the `.ino` file inside the `Arduino/` folder

3. **Install required libraries**
   - Open `Sketch > Include Library > Manage Libraries`
   - Install any libraries referenced in the sketch (e.g., `Wire`, `Adafruit_BMP280`, `MPU6050`)

4. **Select your board and port**
   - Go to `Tools > Board` and select your Arduino model
   - Go to `Tools > Port` and select the correct COM port

5. **Upload the firmware**
   - Click the **Upload** button (→) or press `Ctrl+U`

---

## Usage

1. Assemble the payload electronics onto the custom PCB following the schematic in `/PCB`
2. Connect the Arduino to your computer and upload the firmware (see [Installation](#installation))
3. Open `Tools > Serial Monitor` (baud rate: `9600` or as set in the sketch) to verify sensor readings before flight
4. Mount the payload inside the rocket airframe
5. After the flight, retrieve the logged data from the Arduino's serial output or onboard storage for analysis

---

## Data & Analysis

The payload collects the following data during flight:

| Parameter       | Sensor Source              | Unit  |
|-----------------|----------------------------|-------|
| Altitude        | Barometric pressure sensor | m     |
| Atmospheric pressure | Barometric sensor     | hPa   |
| Temperature     | Barometric sensor          | °C    |
| Acceleration    | IMU (3-axis)               | m/s²  |
| Orientation     | IMU (gyroscope)            | °/s   |

Post-flight data is analyzed to evaluate the rocket's flight profile, apogee, and descent characteristics.

---

## Contributing

This is an academic project. If you'd like to suggest improvements:

1. Fork the repository
2. Create a new branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add your feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

---
