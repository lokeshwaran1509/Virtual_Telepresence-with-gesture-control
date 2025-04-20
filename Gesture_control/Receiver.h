// Receiver Code (Robot Car) - ESP32 with Servo Motors
//------------------------------------------------------
#include <WiFi.h>
#include <esp_now.h>
#include <Servo.h>

// Servo motor pins
const int servo1Pin = 2;  // Front Left
const int servo2Pin = 4;  // Front Right
const int servo3Pin = 12; // Rear Left
const int servo4Pin = 13; // Rear Right

Servo servo1, servo2, servo3, servo4;

// ESP-NOW peer address (Transmitter ESP32 MAC address)
uint8_t transmitterAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Replace with your transmitter's MAC address. Use FF:FF:FF:FF:FF:FF for broadcast, but replace with actual address.

// Structure to hold received MPU6050 data
typedef struct struct_message {
  float ax, ay, az;
  float gx, gy, gz;
} struct_message;
struct_message mpuData;

// Function to initialize ESP-NOW
void initESP_NOW() {
  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set the transmitter as a peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, transmitterAddress, 6);
  peerInfo.channel = 0;  // Set the channel
  peerInfo.encrypt = false;
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(onDataReceived); // Register the receiver callback
}

// Callback function for ESP-NOW receiving
void onDataReceived(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&mpuData, incomingData, sizeof(mpuData));
  Serial.print("Data received from: ");
  Serial.print(mac_addr[0], HEX);
  Serial.print(":");
  Serial.print(mac_addr[1], HEX);
  Serial.print(":");
  Serial.print(mac_addr[2], HEX);
  Serial.print(":");
  Serial.print(mac_addr[3], HEX);
  Serial.print(":");
  Serial.print(mac_addr[4], HEX);
  Serial.print(":");
  Serial.print(mac_addr[5], HEX);
  Serial.println();

  // Print MPU6050 data
  Serial.print("ax: ");
  Serial.print(mpuData.ax);
  Serial.print(", ay: ");
  Serial.print(mpuData.ay);
  Serial.print(", az: ");
  Serial.print(mpuData.az);
  Serial.print(", gx: ");
  Serial.print(mpuData.gx);
  Serial.print(", gy: ");
  Serial.print(mpuData.gy);
  Serial.print(", gz: ");
  Serial.println(mpuData.gz);

  // Control servo motors based on MPU6050 data.  Adjust these values!
  int servo1Angle = map(mpuData.ax, -8, 8, 0, 180); // Example mapping.  Adjust -8, 8, 0, 180
  int servo2Angle = map(mpuData.ay, -8, 8, 0, 180);
  int servo3Angle = map(mpuData.gx, -250, 250, 0, 180);
  int servo4Angle = map(mpuData.gy, -250, 250, 0, 180);

  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  servo4.write(servo4Angle);
}

void setup() {
  Serial.begin(115200);
  // Initialize Servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  // Initialize ESP-NOW
  initESP_NOW();
}

void loop() {
  // put your main code here, to run repeatedly:
}
