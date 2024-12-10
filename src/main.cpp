#include <Arduino.h>
#include <WiFi.h>
#include <inttypes.h>
#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

String createJsonPayload(int signal)
{
  // Create a JSON document
  DynamicJsonDocument doc(200);

  // Add the pulse signal and timestamp to the document
  doc["signal"] = signal;
  doc["timestamp"] = millis(); // Add a timestamp (optional)

  // Create a string to hold the JSON payload
  String jsonString;

  // Serialize the JSON document to a string
  serializeJson(doc, jsonString);

  // Return the JSON string
  return jsonString;
}

//  Variables
int PulseSensorPurplePin = 32; // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal;
int Threshold = 550; // Determine which Signal to "count as a beat", and which to ingore.

HTTPClient http;
const String serverUrl = "http://192.168.231.210:5000/pulse-data";

char ssid[32]; // your network SSID (name)
char pass[32]; // your network password (use for WPA, or use
int pulses[20] = {};
unsigned long previousMillis = 0;
const long interval = 2000;

void nvs_access()
{
  // Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  // Open
  Serial.printf("\n");
  Serial.printf("Opening Non-Volatile Storage (NVS) handle... ");
  nvs_handle_t my_handle;
  err = nvs_open("storage", NVS_READWRITE, &my_handle);
  if (err != ESP_OK)
  {
    Serial.printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
  }
  else
  {
    Serial.printf("Done\n");
    Serial.printf("Retrieving SSID/PASSWD\n");
    size_t ssid_len;
    size_t pass_len;
    err = nvs_get_str(my_handle, "ssid", ssid, &ssid_len);
    err |= nvs_get_str(my_handle, "pass", pass, &pass_len);
    switch (err)
    {
    case ESP_OK:

      Serial.printf("Done\n");
      // Serial.printf("SSID = %s\n", ssid);
      // Serial.printf("PASSWD = %s\n", pass);
      break;
    case ESP_ERR_NVS_NOT_FOUND:
      Serial.printf("The value is not initialized yet!\n");
      break;
    default:
      Serial.printf("Error (%s) reading!\n", esp_err_to_name(err));
    }
  }
  // Close
  nvs_close(my_handle);
}

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // CONNECT WITH WIFI
  // Retrieve SSID/PASSWD from flash before anything else
  nvs_access();
  // We start by connecting to a WiFi network
  delay(1000);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC address: ");
  Serial.println(WiFi.macAddress());

  Serial.println(serverUrl);
}

int i = 0;
void loop()
{
  // unsigned long currentMillis = millis();

  // Check if it's time to send the POST request
  // if (currentMillis - previousMillis >= interval) {
  //   // Save the last time a POST request was sent
  //   previousMillis = currentMillis;

  //   // Send the pulse data
  //   if (i == 20) {
  //     // Print out pulse data for debugging
  //     for (int j = 0; j < 20; j++) {
  //       Serial.print(pulses[j]);
  //       Serial.print(" ");
  //     }
  Signal = analogRead(PulseSensorPurplePin);
  String payload = createJsonPayload(Signal);

  // HTTP CLIENT
  http.begin(serverUrl); // Specify the URL
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(payload); // Send the POST request
  Serial.println(httpCode);

  // Reset array and index after sending the POST request
  //   for (int j = 0; j < 20; j++) {
  //     pulses[j] = 0;
  //   }
  //   i = 0;
  // }
  // }

  // // Collect pulse data every 100ms (for example)
  // if (i < 20) {
  //   int signal = analogRead(PulseSensorPurplePin);
  //   pulses[i] = signal;
  //   i++;
  // }
  delay(500);
}