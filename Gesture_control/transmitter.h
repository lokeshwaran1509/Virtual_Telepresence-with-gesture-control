// Transmitter Code (Glove) - ESP32 with MPU6050
//------------------------------------------------------
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h> // Use Adafruit Library

// WiFi credentials
const char *ssid = "your_SSID";       // Replace with your WiFi SSID
const char *password = "your_PASSWORD"; // Replace with your WiFi password

// MPU6050 object
Adafruit_MPU6050 mpu;

// ESP-NOW peer address (Receiver ESP32 MAC address)
uint8_t receiverAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Replace with your receiver's MAC address.  Use FF:FF:FF:FF:FF:FF for broadcast, but replace with actual address for production

// Structure to hold MPU6050 data
typedef struct struct_message {
  float ax, ay, az;
  float gx, gy, gz;
} struct_message;
struct_message mpuData;

// Function to initialize MPU6050
void initMPU6050() {
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050");
    while (1)
      ;
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Set accelerometer range
  mpu.setGyroRange(MPU6050_RANGE_250_DPS);     // Set gyro range
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);  // Set filter bandwidth
  Serial.println("MPU6050 initialized");
}

// Function to get MPU6050 data
void getMPU6050Data() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  mpuData.ax = a.acceleration.x;
  mpuData.ay = a.acceleration.y;
  mpuData.az = a.acceleration.z;
  mpuData.gx = g.gyro.x;
  mpuData.gy = g.gyro.y;
  mpuData.gz = g.gyro.z;
}

// Callback function for ESP-NOW sending
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Packet sent to: ");
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
  Serial.print(", Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Function to initialize ESP-NOW
void initESP_NOW() {
  WiFi.mode(WIFI_STA); // Set ESP32 as a Station mode
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set the receiver as a peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  // Set the channel.  Make sure sender and receiver are on the same channel
  peerInfo.encrypt = false; // Set to false for now, you can enable encryption later

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_send_cb(onDataSent); // Register the send callback
}

void setup() {
  Serial.begin(115200);
  // Initialize MPU6050
  Wire.begin();
  initMPU6050();
  // Initialize ESP-NOW
  initESP_NOW();
}

void loop() {
  // Get MPU6050 data
  getMPU6050Data();

  // Send data via ESP-NOW
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&mpuData, sizeof(mpuData));
  if (result != ESP_OK) {
    Serial.println("Error sending data"); //  Removed redundant printing of the error code.
  }
  delay(10); // Small delay
}
