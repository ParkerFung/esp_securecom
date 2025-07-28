# esp_securecom — A Secure ESP32 LoRa Messaging Device

**esp_securecom** is a handheld communication device built around the ESP32 that sends and receives preset messages with GPS coordinates over LoRa. It’s designed for off-grid or low-bandwidth environments and works without any cellular or Wi-Fi connection.

This project began as a learning experience to integrate multiple embedded systems modules, LoRa, GPS, OLED, and buttons, into a working and memory-efficient application. Along the way, I explored how to parse GPS data, send structured messages over radio, and prepare for secure encrypted communication (in progress).

---

## What It Is

- A portable off-grid messaging device
- Based on the ESP32-WROOM-1 microcontroller
- Sends and receives preset messages with real-time GPS coordinates
- Displays messages and coordinates on an OLED screen
- Uses LoRa (SX1262) for long-range communication
- Designed to run independently without external networks
- Encryption support coming soon

---

## Hardware Required

| Component        | Description                            |
|------------------|----------------------------------------|
| ESP32-WROOM-1    | Main microcontroller                   |
| SX1262 LoRa Module | Long-range radio communication       |
| GT-U7 GPS Module | Provides real-time latitude/longitude  |
| SSD1306 OLED (128x64) | I2C display for UI                 |
| 3x Tactile Buttons | For navigating UI and sending messages |
| Jumper Wires     | For prototyping                        |
| Breadboard (optional) | For hardware layout during testing |

---

## Wiring and Pin Assignments

### LoRa SX1262 Module

| LoRa Pin | ESP32 GPIO | Description                   |
|----------|------------|-------------------------------|
| CS       | GPIO 10    | Chip Select (NSS)             |
| CLK      | GPIO 12    | SPI Clock (SCK)               |
| MOSI     | GPIO 11    | SPI MOSI                      |
| MISO     | GPIO 13    | SPI MISO                      |
| RESET    | GPIO 9     | Reset                         |
| BUSY     | GPIO 8     | Busy                          |
| DIO1     | GPIO 7     | Interrupt                     |
| DIO2     | GPIO 6     | Optional IRQ / TCXO control   |
| TXEN     | GPIO 5     | PA Enable                     |
| RXEN     | GPIO 2     | LNA Enable                    |

---

### SSD1306 OLED Display

| OLED Pin | ESP32 GPIO | Description                          |
|----------|------------|--------------------------------------|
| VCC      | 3.3V       | Power supply                         |
| GND      | GND        | Ground                               |
| SDA      | GPIO 21    | I2C Data                             |
| SCL      | GPIO 4     | I2C Clock                            |

---

### GPS Module GT-7U

| GPS Pin | ESP32 GPIO | Description                          |
|---------|------------|--------------------------------------|
| VCC     | 3.3V or 5V | Power                                |
| GND     | GND        | Ground                               |
| TXD     | GPIO 16    | GPS TX                               |
| RXD     | GPIO 3     | GPS RX                               |

---

### Button GPIO Connections

| Button    | GPIO Pin |
|-----------|----------|
| Up        | 38       |
| Down      | 39       |
| Select    | 14       |

---

## Features

- **Preset Message Sending**  
  Choose from affirmative, negative, or distress messages.

- **GPS Position Sharing**  
  Each sent message includes real-time coordinates and a timestamp.

- **Message Receiving**  
  Incoming messages are stored with timestamps and can be reviewed.

- **OLED UI Navigation**  
  Menu system allows switching between screens: Send, Received, and Coordinates.

- **Live GPS Display**  
  View current coordinates in real time.

---

## Why I Built It

I wanted to learn how different embedded systems components work together in real-world scenarios, especially when memory and processing are limited. This project helped me:

- Parse and display live GPS data
- Communicate over LoRa with message structure
- Design a clean UI on a 128x64 OLED
- Prepare for encrypted communication
- Manage memory carefully on constrained hardware

---

## In Progress

- **Encryption**: Lightweight encryption (e.g., XOR or AES) to secure messages
- **Improved Reliability**: More robust transmission and reception logic
- **Memory Optimization**: Reduce use of dynamic strings and heap fragmentation

---

## How to Build

1. Install [PlatformIO](https://platformio.org/) in VS Code
2. Clone this repo or open the project folder
3. Connect hardware to the correct GPIOs (see pin tables above)
4. Install required libraries:
   - [RadioLib](https://github.com/jgromes/RadioLib)
   - [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
   - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
5. Upload using PlatformIO's **Upload** button

---

## What Works

- Message sending with GPS coordinates and timestamp
- Message receiving with message log
- Real-time GPS coordinate viewing
- Button-based menu navigation
- OLED display rendering and UI

---

## File Structure

| File                 | Purpose                            |
|----------------------|------------------------------------|
| `main.cpp`           | Main loop and core app logic       |
| `radio.cpp/.h`       | LoRa send/receive functions        |
| `gps.cpp/.h`         | GPS module and data formatting     |
| `interface.cpp/.h`   | OLED drawing and menu rendering    |
| `input.cpp/.h`       | Physical button handling           |
| `platformio.ini`     | PlatformIO build configuration     |

---

## What I Learned

- How to wire and control multiple modules (LoRa, GPS, OLED) from one MCU
- How to parse NMEA GPS data and extract coordinates/time
- How to structure a menu-driven UI on a small screen
- Where memory inefficiencies arise when using `String` on ESP32
- Planning ahead for secure message encryption on embedded systems

---

## Future Improvements

- Add delivery acknowledgments
- Improve LoRa signal quality with better antenna tuning
- Add scrollable or paged message viewing

---

## Troubleshooting Tips

- **GPS says "no fix" or "no GPS"**: Check outdoor reception and wiring
- **OLED not displaying anything**: Verify I2C pins and address
- **LoRa not sending/receiving**: Ensure both devices use same frequency and bandwidth
- **ESP32 slow performance**: Avoid heavy string manipulation or unnecessary redraws

---

## Libraries Used

- [`RadioLib`](https://github.com/jgromes/RadioLib) — LoRa communication
- [`TinyGPSPlus`](https://github.com/mikalhart/TinyGPSPlus) — GPS parsing
- [`Adafruit SSD1306`](https://github.com/adafruit/Adafruit_SSD1306) — OLED display

---

## Final Notes

This project is still under development, but the current implementation already supports basic messaging and live GPS. It’s a valuable learning platform for secure embedded communication and modular system design.

Created by  
**Parker Fung**
