# PunchBot: Advanced Automatic Paper Hole Punch  
 
**Automate your document prep with smart batch processing & customizable hole patterns**  

## Key Features
- **Batch Processing**: Punch 1-50 sheets automatically  
- **Memory Presets**: Save custom hole patterns (2/3/4-hole)  
- **OLED Dashboard**: Real-time progress tracking  
- **Stepper Feed System**: Precise paper alignment  
- **EEPROM Storage**: Remember settings between uses  

## Hardware Requirements
[Full Bill of Materials](hardware/bom.txt)  
| Component | Quantity |  
|-----------|----------|  
| Arduino Uno | 1 |  
| NEMA 17 Stepper | 1 |  
| Micro Servo | 1 |  
| OLED Display | 1 |  
| IR Sensor | 1 |  
| Push Buttons | 3 |  
| 3D Printed Parts | 5+ |  

## Setup Guide 
[Full Assembly Instructions](docs/setup.md)  
1. **Mechanical Assembly**  
   - Mount stepper & feed rollers  
   - Install punch mechanism  
   - Position IR paper sensor  

2. **Electronics Wiring**  
   ```plaintext
   Stepper -> Pins 3,4,5
   Servo   -> Pin 6
   IR      -> Pin 7
   Buttons -> Pins 8,9,10
   OLED    -> I2C (A4,A5)
   ```

3. **First-Time Calibration**  
   ```ino
   void setup() {
     calibrateSystem(); // See setup.md
     // ...
   }
   ```

## Usage Instructions
[Preset Configuration Guide](docs/presets.md)  
| Button | Function |  
|--------|----------|  
| UP ▲ | Cycle presets |  
| DOWN ▼ | Adjust parameters |  
| SELECT | Start batch |  

**Typical Workflow:**  
1. Load paper stack in tray  
2. Select hole pattern (2/3/4-hole)  
3. Press SELECT to begin batch  
4. Watch real-time progress on OLED  

## Install Code 
1. Download repository:  
   ```bash
   git clone https://github.com/yourusername/PunchBot.git
   ```
2. Open `code/punchbot.ino` in Arduino IDE  
3. Install required libraries:  
   - Adafruit_SSD1306  
   - Adafruit_GFX  
   - Servo  
4. Upload to Arduino  

## Customize Your Presets 
Modify `presets[]` array in code:  
```ino
// Format: {holes, spacing_mm, max_sheets}
Preset presets[3] = {
  {4, 30.0, 15},   // Custom 4-hole 
  {2, 100.0, 30},  // Wide 2-hole
  {5, 25.0, 5}     // 5-hole special
};
```

## Troubleshooting  
| Issue | Solution |  
|-------|----------|  
| Paper jams | Reduce `FEED_SPEED` value |  
| Weak punches | Increase servo torque |  
| OLED not working | Check I2C connections |  
| Presets not saving | Replace EEPROM battery |  

## License
MIT License - Free for personal and commercial use
