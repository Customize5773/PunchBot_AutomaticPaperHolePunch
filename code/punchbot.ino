#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// Pin Definitions
#define STEPPER_STEP_PIN 3
#define STEPPER_DIR_PIN 4
#define STEPPER_EN_PIN 5
#define SERVO_PIN 6
#define IR_SENSOR_PIN 7
#define BUTTON_UP_PIN 8
#define BUTTON_DOWN_PIN 9
#define BUTTON_SELECT_PIN 10

// Display Settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Punching Mechanism
Servo punchServo;

// Preset Configuration
struct Preset {
  int holeCount;
  float holeSpacing;
  int sheetsToPunch;
};

Preset presets[3] = {
  {2, 80.0, 1},  // 2-hole preset (80mm spacing)
  {3, 80.0, 1},  // 3-hole preset
  {4, 40.0, 1}   // 4-hole preset (40mm spacing)
};

// Global Variables
int currentPreset = 1;  // Default to 3-hole preset
int sheetsPunched = 0;
int sheetsInBatch = 0;
bool processingBatch = false;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200;

// Stepper Constants
const int STEPS_PER_MM = 20;      // Adjust based on your mechanism
const int PAPER_LENGTH = 210;     // A4 paper length in mm
const int FEED_SPEED = 500;       // Microseconds between steps

void setup() {
  // Initialize pins
  pinMode(STEPPER_STEP_PIN, OUTPUT);
  pinMode(STEPPER_DIR_PIN, OUTPUT);
  pinMode(STEPPER_EN_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  
  digitalWrite(STEPPER_EN_PIN, LOW);  // Enable stepper
  
  // Initialize servo
  punchServo.attach(SERVO_PIN);
  punchServo.write(90);  // Rest position
  
  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while(true);  // Halt if display fails
  }
  
  // Initialize EEPROM
  EEPROM.get(0, currentPreset);
  if(currentPreset < 0 || currentPreset > 2) currentPreset = 1;  // Validate
  
  // Show startup screen
  displayStartupScreen();
  delay(2000);
  
  // Load counter from EEPROM
  sheetsPunched = EEPROM.read(10) << 8 | EEPROM.read(11);
  updateDisplay();
}

void loop() {
  // Button handling with debounce
  if((millis() - lastDebounceTime) > debounceDelay) {
    if(digitalRead(BUTTON_UP_PIN) == LOW) {
      changePreset(1);
      lastDebounceTime = millis();
    }
    else if(digitalRead(BUTTON_DOWN_PIN) == LOW) {
      changePreset(-1);
      lastDebounceTime = millis();
    }
    else if(digitalRead(BUTTON_SELECT_PIN) == LOW) {
      startBatch();
      lastDebounceTime = millis();
    }
  }
  
  // Batch processing
  if(processingBatch) {
    processNextSheet();
    if(sheetsInBatch <= 0) {
      processingBatch = false;
      updateDisplay();
    }
  }
  
  delay(50);
}

// Change preset and save to EEPROM
void changePreset(int direction) {
  currentPreset = constrain(currentPreset + direction, 0, 2);
  EEPROM.put(0, currentPreset);
  updateDisplay();
}

// Start batch processing
void startBatch() {
  sheetsInBatch = presets[currentPreset].sheetsToPunch;
  processingBatch = true;
  updateDisplay();
}

// Process one sheet in the batch
void processNextSheet() {
  // Feed paper into position
  feedPaper(FIRST_HOLE_POSITION);
  
  // Punch holes
  for(int i = 0; i < presets[currentPreset].holeCount; i++) {
    punchHole();
    if(i < presets[currentPreset].holeCount - 1) {
      feedPaper(presets[currentPreset].holeSpacing);
    }
  }
  
  // Eject paper
  feedPaper(PAPER_LENGTH - (FIRST_HOLE_POSITION + 
           (presets[currentPreset].holeCount - 1) * 
           presets[currentPreset].holeSpacing));
  
  // Update counters
  sheetsInBatch--;
  sheetsPunched++;
  
  // Save counter to EEPROM
  EEPROM.write(10, sheetsPunched >> 8);
  EEPROM.write(11, sheetsPunched & 0xFF);
  
  updateDisplay();
}

// Punch a single hole
void punchHole() {
  punchServo.write(0);     // Punch down
  delay(500);
  punchServo.write(90);    // Return to rest
  delay(300);
}

// Feed paper a specific distance
void feedPaper(float distanceMM) {
  digitalWrite(STEPPER_DIR_PIN, HIGH);  // Forward direction
  int steps = distanceMM * STEPS_PER_MM;
  
  for(int i = 0; i < steps; i++) {
    digitalWrite(STEPPER_STEP_PIN, HIGH);
    delayMicroseconds(FEED_SPEED);
    digitalWrite(STEPPER_STEP_PIN, LOW);
    delayMicroseconds(FEED_SPEED);
    
    // Check for paper jam
    if(digitalRead(IR_SENSOR_PIN) == HIGH) {
      displayPaperJamWarning();
      while(true);  // Halt on error
    }
  }
}

// Display functions
void updateDisplay() {
  display.clearDisplay();
  
  // Header
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("PunchBot v1.0");
  
  // Current preset
  display.setCursor(0, 15);
  display.print("Preset: ");
  display.print(presets[currentPreset].holeCount);
  display.print("-hole");
  
  // Batch info
  display.setCursor(0, 30);
  if(processingBatch) {
    display.print("Batch: ");
    display.print(sheetsInBatch);
    display.print("/");
    display.print(presets[currentPreset].sheetsToPunch);
  } else {
    display.print("Ready - Press GO");
  }
  
  // Sheets punched
  display.setCursor(0, 45);
  display.print("Total: ");
  display.print(sheetsPunched);
  display.print(" sheets");
  
  // Footer instructions
  display.setCursor(0, 57);
  display.print("UP/DOWN/SEL");
  
  display.display();
}

void displayStartupScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 15);
  display.print("PUNCHBOT");
  display.setTextSize(1);
  display.setCursor(30, 40);
  display.print("Advanced Edition");
  display.display();
}

void displayPaperJamWarning() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("  PAPER JAM ERROR!");
  display.setCursor(0, 35);
  display.print(" Check mechanism and");
  display.setCursor(0, 45);
  display.print("     reset system");
  display.display();
}
