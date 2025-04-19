#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// Replace with your Wi-Fi credentials
const char* ssid = "OJTECH";
const char* password = "OJTECH4199";

WebSocketsServer webSocket(81);  // WebSocket server on port 81

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  
  // Send a batch of instrument readings every 5 seconds
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) {
    lastTime = millis();
    
    // Randomly decide how many instrument readings to send (between 1 and 3)
    int numInstruments = random(1, 4);
    
    // Allocate a JSON document (adjust capacity if needed)
    DynamicJsonDocument doc(1024);
    JsonArray instruments = doc.to<JsonArray>();
    
    for (int i = 0; i < numInstruments; i++) {
      JsonObject instrument = instruments.createNestedObject();
      instrument["id"] = random(1000, 10000);
      // Altitude in feet (simulate between 1,000 and 40,000 ft)
      instrument["altitude"] = random(1000, 40001);
      // Airspeed in knots (simulate between 100 and 600 knots)
      instrument["airspeed"] = random(100, 601);
      // Heading in degrees (0 to 359)
      instrument["heading"] = random(0, 360);
      // Pitch in degrees (simulate between -10 and 10 degrees)
      instrument["pitch"] = random(-10, 11);
      // Roll in degrees (simulate between -45 and 45 degrees)
      instrument["roll"] = random(-45, 46);
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    webSocket.broadcastTXT(jsonString);
    Serial.println("Sent: " + jsonString);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.println("Client connected");
      break;
    case WStype_DISCONNECTED:
      Serial.println("Client disconnected");
      break;
    default:
      break;
  }
}
