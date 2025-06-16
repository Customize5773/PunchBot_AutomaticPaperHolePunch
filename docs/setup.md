# PunchBot Setup Guide

## 1. Mechanical Assembly  
### A) Frame Construction  
1. Attach **300mm aluminum extrusions** to base plate  
2. Mount **linear rails** parallel to extrusions  
3. Install **NEMA 17 motor** at feeding end  

### B) Paper Feed System  
1. Attach **rubber rollers** to stepper shaft  
2. Install **idler roller** on opposite side  
3. Mount **IR sensor** at paper entry point  

### C) Punch Mechanism  
1. Secure **servo motor** to punch arm  
2. Connect punch arm to **commercial punch head**  
3. Calibration:  
   - Ensure full punch travel = 15mm  
   - Adjust servo horn at 90° rest position  

## 2. Electronics Wiring  
| Component       | Arduino Pin |  
|-----------------|-------------|  
| Stepper STEP    | 3           |  
| Stepper DIR     | 4           |  
| Stepper ENABLE  | 5           |  
| Servo           | 6           |  
| IR Sensor       | 7           |  
| Button (UP)     | 8           |  
| Button (DOWN)   | 9           |  
| Button (SELECT) | 10          |  
| OLED SDA        | A4          |  
| OLED SCL        | A5          |  

**Power Connections:**  
- Stepper Driver: 12V/2A DC  
- Arduino: USB or 7-12V barrel jack  

## 3. First-Time Calibration  
```ino
// Add this to setup() for calibration
void calibrateSystem() {
  punchServo.write(0); 
  delay(1000);
  punchServo.write(180);
  delay(1000);
  punchServo.write(90); // Center position
  
  feedPaper(100); // Test feed 100mm
}
```

## 4. Operation Test  
1. Insert paper stack in tray  
2. Select preset with UP/DOWN buttons  
3. Press SELECT to start batch  
4. Verify:  
   - Paper feeds smoothly  
   - Holes are cleanly punched  
   - OLED shows correct count  

> **Troubleshooting Tip:** If paper jams occur, reduce `FEED_SPEED` in code (line 43)  
