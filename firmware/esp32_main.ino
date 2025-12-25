/*
  Vayu-X — ESP32 Firmware

  Description:
  Core firmware for the Vayu-X autonomous air quality robot.
  Handles sensor acquisition, AQI computation, decision logic,
  autonomous movement, and air filtration control.

  Platform: ESP32
  Status: Stable / Demo-tested

  Notes:
  - Deterministic, threshold-based control logic
  - Designed for real-time embedded operation
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// ==================== WIFI & SERVER SETTINGS ====================
const char* ssid = "Vayu-X";
const char* password = "123456789";
WebServer server(80);

// ==================== AQI Threshold ====================
#define AQI_STOP_THRESHOLD 101.0 

// ==================== OLED ====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==================== DHT22 ====================
#define DHTPIN 13
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Global DHT readings (to prevent frequent reads)
float globalTemp = 0.0;
float globalHumidity = 0.0;
unsigned long lastDHTRead = 0;
#define DHT_READ_INTERVAL 2000 // Read every 2 seconds

// ==================== PMS3003 ====================
HardwareSerial pms(2);
#define PMS_RX 16

uint16_t PM1 = 0, PM25 = 0, PM10 = 0;
uint8_t pms_buffer[32];
int pms_index = 0;

// ==================== MQ135 ====================
#define MQ_PIN 34

// ==================== Motor Pins ====================
#define AIN1 27
#define AIN2 26
#define PWMA 14

#define BIN1 25
#define BIN2 33
#define PWMB 32

#define STBY 12

// Motor timing control
unsigned long lastMotorUpdate = 0;
#define MOTOR_UPDATE_INTERVAL 100 // Update motors every 100ms

// ==================== Ultrasonic ====================
#define TRIG 4
#define ECHO 5
long distance = 0;

// ==================== OLED Update Timing ====================
unsigned long lastOledTime = 0;
#define OLED_UPDATE_INTERVAL 500 // Update display every 500ms

// ================= AQI CALCULATION BREAKPOINTS =================
struct BP { float c_low, c_high, i_low, i_high; };

BP pm25_bp[] = {
  {0.0, 12.0, 0, 50}, 
  {12.1, 35.4, 51, 100},
  {35.5, 55.4, 101, 150}, 
  {55.5, 150.4, 151, 200},
  {150.5, 250.4, 201, 300}, 
  {250.5, 350.4, 301, 400},
  {350.5, 500.4, 401, 500}
};

BP pm10_bp[] = {
  {0, 54, 0, 50}, 
  {55, 154, 51, 100},
  {155, 254, 101, 150}, 
  {255, 354, 151, 200},
  {355, 424, 201, 300}, 
  {425, 504, 301, 400},
  {505, 604, 401, 500}
};

// ================= AQI COMPUTATION =================
float computeAQI(float c, BP *bp, int size) {
  for (int i = 0; i < size; i++) {
    if (c >= bp[i].c_low && c <= bp[i].c_high) {
      return ((bp[i].i_high - bp[i].i_low) /
              (bp[i].c_high - bp[i].c_low)) *
             (c - bp[i].c_low) + bp[i].i_low;
    }
  }
  return -1;
}

// ================= HELPER FUNCTIONS =================
String getAQICategory(float aqi) {
  if (aqi <= 50) return "Good";
  if (aqi <= 100) return "Moderate";
  if (aqi <= 150) return "Unhealthy (Sen.)";
  if (aqi <= 200) return "Unhealthy";
  if (aqi <= 300) return "Very Unhealthy";
  return "Hazardous";
}

String getMQLevel(int value) {
  if (value < 1200) return "Low";
  if (value < 2200) return "Medium";
  return "High";
}

// ================= ULTRASONIC SENSOR =================
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Reduced timeout to prevent blocking
  long duration = pulseIn(ECHO, HIGH, 5000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

// ================= MOTOR CONTROL =================
void moveForward(int speed) {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, speed);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

void turnRight(int speed) {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, speed);
}

// ================= PMS3003 DATA ACQUISITION =================
void readPMSData() {
  while (pms.available()) {
    uint8_t incomingByte = pms.read();

    // Check for start bytes
    if (pms_index == 0 && incomingByte != 0x42) continue;
    if (pms_index == 1 && incomingByte != 0x4D) {
      pms_index = 0;
      continue;
    }

    pms_buffer[pms_index++] = incomingByte;

    // Safety check to prevent buffer overflow
    if (pms_index >= 32) {
      if (pms_index == 32) {
        // Valid frame received
        PM1  = pms_buffer[10] * 256 + pms_buffer[11];
        PM25 = pms_buffer[12] * 256 + pms_buffer[13];
        PM10 = pms_buffer[14] * 256 + pms_buffer[15];
      }
      pms_index = 0;
    }
  }
}

// ================= DHT22 READING (NON-BLOCKING) =================
void updateDHTReadings() {
  if (millis() - lastDHTRead >= DHT_READ_INTERVAL) {
    lastDHTRead = millis();
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Only update if readings are valid
    if (!isnan(h)) globalHumidity = h;
    if (!isnan(t)) globalTemp = t;
  }
}

// ================= WEB SERVER HANDLERS =================
void handleData() {
  int mq = analogRead(MQ_PIN);
  float aqi25 = computeAQI(PM25, pm25_bp, 7);
  float aqi10 = computeAQI(PM10, pm10_bp, 7);
  float AQI = max(aqi25, aqi10);

  // Build JSON response
  StaticJsonDocument<300> doc;
  doc["aqi"] = (int)AQI;
  doc["category"] = getAQICategory(AQI);
  doc["pm25"] = PM25;
  doc["pm10"] = PM10;
  doc["mq135"] = getMQLevel(mq);
  doc["temp"] = globalTemp;
  doc["humidity"] = globalHumidity;

  String jsonString;
  serializeJson(doc, jsonString);

  server.send(200, "application/json", jsonString);
}

void handleRoot() {
  server.send(200, "text/plain", "Vayu-X is Active. Connect App to /data");
}

// ===================== SETUP =====================
void setup() {
  Serial.begin(115200);
  Serial.println("Vayu-X Initializing...");

  // Initialize I2C and OLED
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
    while (1); // Stop if display fails
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Vayu-X Init...");
  display.display();

  // Initialize DHT22
  dht.begin();
  Serial.println("DHT22 initialized");

  // Initialize PMS3003
  // IMPORTANT: Use voltage divider on RX pin!
  // PMS TX -> 2kΩ -> ESP32 Pin 16
  //            |-> 1kΩ -> GND
  pms.begin(9600, SERIAL_8N1, PMS_RX, -1);
  Serial.println("PMS3003 initialized");

  // Initialize Motor Pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  Serial.println("Motors initialized");

  // Initialize Ultrasonic
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.println("Ultrasonic initialized");

  // Setup Wi-Fi Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Setup Web Server Routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  Serial.println("Web server started");

  // Display initialization complete
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Vayu-X Ready!");
  display.println("WiFi: Vayu-X");
  display.print("IP: ");
  display.println(IP);
  display.display();
  delay(3000);

  Serial.println("Setup complete!");
}

// ===================== MAIN LOOP =====================
void loop() {
  // 1. Handle web server (highest priority)
  server.handleClient();

  // 2. Read sensors (non-blocking)
  readPMSData();
  updateDHTReadings();

  // 3. Read MQ-135 (analog, fast)
  int mq = analogRead(MQ_PIN);

  // 4. Calculate AQI
  float aqi25 = computeAQI(PM25, pm25_bp, 7);
  float aqi10 = computeAQI(PM10, pm10_bp, 7);
  float AQI = max(aqi25, aqi10);

  // 5. Motor control (non-blocking timing)
  if (millis() - lastMotorUpdate >= MOTOR_UPDATE_INTERVAL) {
    lastMotorUpdate = millis();
    
    if (AQI >= AQI_STOP_THRESHOLD) {
      stopMotors();
    } else {
      distance = getDistance();
      
      if (distance > 0 && distance < 20) {
        stopMotors();
        delay(200);
        turnRight(180);
        delay(400);
      } else {
        moveForward(200);
      }
    }
  }

  // 6. Update OLED display (non-blocking timing)
  if (millis() - lastOledTime >= OLED_UPDATE_INTERVAL) {
    lastOledTime = millis();
    
    display.clearDisplay();
    display.setCursor(0, 0);

    if (AQI >= AQI_STOP_THRESHOLD) {
      display.println("!!! STOP MODE !!!");
      display.println("High Pollution!");
    } else {
      display.printf("Dist: %ld cm\n", distance);
    }
    
    display.printf("PM2.5: %d\n", PM25);
    display.printf("PM10 : %d\n", PM10);
    display.printf("AQI  : %.0f\n", AQI);
    display.printf("Temp : %.1fC\n", globalTemp);
    display.printf("Hum  : %.0f%%\n", globalHumidity);
    
    display.display();
  }

  // Small yield to prevent watchdog issues
  yield();
}
