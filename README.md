# 🗺️ Offline OSM Map Viewer using ESP32

An embedded mapping project that displays an **offline map of Nagpur, Maharashtra, India**, on a 2.4-inch TFT touchscreen using an ESP32 development board. The map tiles are stored on a microSD card, allowing the map to be viewed without an active internet connection.

The project uses map data from **OpenStreetMap (OSM)**, processed using QGIS and exported as image tiles for offline use.

---

## ✨ Features

- 🗺️ Offline Map Viewing: Displays the map of Nagpur using locally stored map tiles on a microSD card. Once the tiles are prepared and stored, the map can be viewed without an active internet connection.

- 🔍 Multiple Zoom Levels: Supports map zoom levels from 10 to 15, allowing users to switch between a broader city view and more detailed street-level views.

- 👆 Touchscreen-Based Panning: Uses a resistive touchscreen to let users move around the map by touching and dragging the screen, making exploration more interactive.

- 💾 microSD Map Storage: Stores map tiles on a microSD card, allowing the project to handle a large collection of map images without requiring all map data to fit in the ESP32's internal memory.

- 🖥️ TFT Display Interface: Uses a 2.4-inch ST7789 colour TFT display with a resolution of 320 × 240 pixels to present the map in a compact, portable format.

- ⚡ ESP32-Based Embedded System: Uses the ESP32 as the main controller to manage the display, touchscreen input, SD card access, and map rendering.

---

## 🧰 Hardware Requirements

|Sr no.| Component | Description |
|---|---|---|
|1. | ESP32 DevKit V1 | Main microcontroller |
|2. | 2.4-inch ST7789 TFT | 320 × 240 colour display |
|3. | XPT2046 Touch Controller | Resistive touchscreen input |
|4. | microSD-to-SD adapter | lets you fit small sd card into bigger slot |
|5. | microSD card (8GB) | Contains the exported map images |
|6. | Female pin headers x2 | To place components |
|7. | Perfboard | Prototyping board |

*Note: The touchscreen controller and microSD interface may be integrated into your display module or connected separately.*

---

## 📸 Hardware photos

| Zoom level 10 | Zoom level 11 |
|:---: | :---: |
| ![10](docs/images/zoom_10.jpg) | ![11](docs/images/zoom_11.jpg) |
| Zoom level 12 | Zoom level 13 |
| ![12](docs/images/zoom_12.jpg) | ![13](docs/images/zoom_13.jpg) |
| Zoom level 14 | Zoom level 15 |
| ![14](docs/images/zoom_14.jpg) | ![15](docs/images/zoom_15.jpg) |

---

## ⚙️ How It Works

1. OpenStreetMap geographic data is processed and styled in QGIS.
2. The map is exported as 256 × 256 JPEG tiles at zoom levels 10–15.
3. The exported tiles are copied to the microSD card.
4. The ESP32 reads the required tiles and decodes them for display.
5. Touch input allows the user to pan around the map and change zoom levels.

---
## 🚀 Getting Started

Follow the instructions below to prepare the offline map, set up the hardware, and run the project.

### 💻 1. Software Requirements and Setup

**Step 1: Install QGIS**
- Download and install [QGIS](https://qgis.org/) on your computer.
- QGIS is used to prepare the Nagpur map, customise its appearance, and export it as image tiles for offline viewing.

**Step 2: Prepare the Map**
- Obtain the required geographic data from [OpenStreetMap](https://www.openstreetmap.org/).
- Open QGIS, load the geographic data, and adjust the map styling according to your requirements.
- Export the map as 256 × 256 pixel JPEG tiles at the required zoom levels.
- Copy the generated tiles to a microSD card, keeping the folder structure and filenames expected by the firmware.

*The repository may already contain sample map tiles. You can use these instead of generating your own map.*

**Step 3: Install the Development Software**
- Install the [Arduino IDE](https://www.arduino.cc/en/software) and configure ESP32 board support using the Boards Manager.
- Install the libraries required by the project, including Adafruit GFX, Adafruit ST7789, XPT2046_Touchscreen, and TJpg_Decoder.
- Open the project's `.ino` file in Arduino IDE, select the appropriate ESP32 board and COM port, and compile the firmware.

### 🔧 2. Hardware Assembly

**Step 1: Prepare the Perfboard**
- Arrange the components on the perfboard before soldering. Check the component positions, wiring connections, and available space to avoid overcrowding.
- Position the female pin headers so the ESP32 development board can be inserted and removed easily for programming or replacement.

**Step 2: Check Component Orientation**
- Carefully check the pin labels and orientation of the ESP32 headers, TFT display, touchscreen connections, and microSD interface before soldering.
- Ensure that the power, ground, and SPI signal connections match the pin connection table in this README.
- Double-check the connections to avoid reversed power pins or short circuits.

**Step 3: Solder the Connections**
- Solder the female headers and other required connections securely to the perfboard.
- Use a suitable soldering iron and apply heat only as needed to create clean, reliable joints.
- Inspect each solder joint for loose connections, solder bridges, or accidental shorts before powering the circuit.

**Step 4: Test the Hardware**
- Insert the ESP32 into the female headers and connect the TFT, touchscreen, and microSD card according to the wiring diagram.
- Power the circuit through USB and verify that the display, touch input, and SD card operate correctly before using the complete map viewer.

> **Safety note:** Disconnect the power supply while soldering or modifying the circuit. Avoid touching the hot soldering iron or freshly soldered joints.

### 📥 3. Get the Project Files

You can obtain the project files using either of the following methods.

**Option A: Clone the Repository**

If Git is installed on your computer, open a terminal and run:

```bash
git clone https://github.com/SohamKale83/Offline-OSM-Map-ESP32.git
```

Open the downloaded project folder to access the available source code and resources.

**Option B: Download the Files Manually**

- Open the [Offline-OSM-Map-ESP32 repository](https://github.com/YOUR_USERNAME/Offline-OSM-Map-ESP32).
- Click **Code → Download ZIP**.
- Extract the ZIP file to your computer.
- Alternatively, open the required files and folders on GitHub and download or copy them individually.

### ▶️ 4. Upload and Run

1. Open the Arduino `.ino` file in Arduino IDE.
2. Verify that the firmware's SD card paths match the folder structure of the map tiles.
3. Select the appropriate ESP32 board and COM port.
4. Compile and upload the firmware.
5. Insert the microSD card containing the map tiles and restart the device.
6. Use the touchscreen to explore the offline Nagpur map, pan around, and change zoom levels.

**Note:** The repository's source code, libraries, wiring, and map-tile structure must be compatible with your hardware. If you use your own map tiles, ensure their filenames and directory structure match the firmware.

---

## 🔮 Future Improvements

- **🔎 Place Search:** Implement a search feature to locate streets, landmarks, hospitals, schools, and other important places directly on the offline map.

- **🛣️ Route Calculation:** Add road-based route planning between two selected locations, including road distance calculations.

- **📍 Points of Interest:** Display interactive markers for important locations such as hospitals, colleges, gardens, and public facilities.

- **⚡ Improved Map Caching:** Optimise map rendering by caching decoded map tiles in available memory to reduce repeated JPEG decoding and improve panning responsiveness.

- **🧭 GPS Integration:** Integrate a GPS module to display the user's current location on the offline map.

- **🖨️ 3D-Printed Enclosure:** Design and manufacture a compact 3D-printed case to securely house the ESP32, TFT display, touchscreen, and supporting components, making the device more comfortable to hold and use.

- **🚀 ESP32-S3 Upgrade:** Upgrade to an ESP32-S3 development board with 8 MB PSRAM to provide additional memory for map-tile caching and rendering, enabling smoother panning, faster map updates, and better overall performance.

---

## 🙌 Credits

- [OpenStreetMap](https://www.openstreetmap.org/) — geographic data.
- [QGIS](https://qgis.org/) — geographic data processing and map rendering.

Map data © OpenStreetMap contributors. OpenStreetMap data is available under the [Open Database License (ODbL)](https://www.openstreetmap.org/copyright).

---

**Project Status:** Work in progress 🚧

Built as an exploration of embedded systems, geographic information systems (GIS), and offline mapping using ESP32.

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Soham Kale**
GitHub: [@SohamKale83](https://github.com/SohamKale83)