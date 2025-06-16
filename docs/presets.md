# Custom Preset Configuration  
**Modify hole patterns without reprogramming**  

## Default Presets  
| Preset | Holes | Spacing (mm) | Sheets/Batch |  
|--------|-------|-------------|--------------|  
| 1      | 2     | 80.0        | 1-50         |  
| 2      | 3     | 80.0        | 1-50         |  
| 3      | 4     | 40.0        | 1-50         |  

## Creating New Presets  
1. **Access configuration mode:**  
   - Power on while holding SELECT button  
   - OLED shows "CONFIG MODE"  

2. **Navigate presets:**  
   - UP/DOWN: Cycle through presets 1-3  
   - SELECT: Edit highlighted preset  

3. **Adjust parameters:**  
   ```plaintext
   Hole count: [3]    <UP/DOWN to change>
   Spacing: [80.0]    <UP: +0.5mm / DOWN: -0.5mm>
   Batch size: [10]   <UP: +1 / DOWN: -1>
   SAVE? (Hold SELECT 3s)
   ```

## Advanced Customization  
Modify `presets[]` array in code (lines 24-28):  
```ino
// Format: {holes, spacing_mm, max_sheets}
Preset presets[3] = {
  {4, 30.0, 15},   // Custom 4-hole 
  {2, 100.0, 30},  // Wide-spaced 2-hole
  {5, 25.0, 5}     // 5-hole for special binding
};
```

## Saving to EEPROM  
1. Changes auto-save when:  
   - SELECT held 3 seconds in config mode  
   - OLED flashes "SAVED"  
2. Factory reset:  
   - Power on while holding UP+DOWN  
   - OLED shows "DEFAULTS RESTORED"  

## Measurement Tips  
1. Use calipers to measure hole spacing  
2. Test patterns on scrap paper  
3. Optimal spacing ranges:  
   - 2-hole: 70-120mm  
   - 3-hole: 75-85mm (standard A4)  
   - 4-hole: 30-50mm  
