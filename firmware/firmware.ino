#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"

// Hardware Pin Definitions
const int RELAY_PIN = 23;        // Triggers garage door motor button
const int CLOSED_SENSOR_PIN = 18; // Magnetic switch at the bottom
const int OPEN_SENSOR_PIN = 19;   // Magnetic switch at the top

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Keep relay off initially
  
  pinMode(CLOSED_SENSOR_PIN, INPUT_PULLUP);
  pinMode(OPEN_SENSOR_PIN, INPUT_PULLUP);

  // Connect to Network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi network!");
}

void loop() {
  // Read current physical states
  bool isClosed = (digitalRead(CLOSED_SENSOR_PIN) == LOW);
  bool isOpen = (digitalRead(OPEN_SENSOR_PIN) == LOW);
  
  String doorState = "moving";
  if (isClosed) doorState = "closed";
  if (isOpen) doorState = "open";

  // Check network status and report telemetry back to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(SERVER_HOST) + ":" + String(SERVER_PORT) + "/api/garage/telemetry";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    String payload = "{\"status\":\"" + doorState + "\"}";
    int httpResponseCode = http.POST(payload);
    
    http.end();
  }
  
  delay(3000); // Send status update updates every 3 seconds
}

// In a full implementation, you would add a listening web client or MQTT subscription here 
// to trigger digitalWrite(RELAY_PIN, HIGH); delay(500); digitalWrite(RELAY_PIN, LOW);
