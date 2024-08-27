#include <WiFiNINA.h> // library for WiFi functionality
#include <Wire.h>     // library for I2C communication
#include <BH1750.h>   // library for BH1750 light sensor 

// WiFi Credentials
char ssid [] = "Vansh" ;
char pass [] = "12345678";

WiFiClient client; // Instance of the WifiClient for wifi communication
BH1750 lightMeter; // Instance of the BH1750 light sensor library const 

// Variables to configure webhook requests
String queryString = "?value1=57&value2=25"; 
char HOST_NAME[] = "maker.ifttt.com"; 
String PATH_NAME = "https://maker.ifttt.com/trigger/lightIntensity_variation/with/key/pC2JWZTvsXqipqikbM3mJ4rTkPMJV2YgnQG9lthtMlF"; // IFTTT Webhook URL


void setup() 
{
  Serial.begin(9600); 

  //Initialization of WiFi Communication, I2C Communication, and BH1750 light sensor
  WiFi.begin(ssid, pass);
  Wire.begin();
  lightMeter.begin();

  // Attempt to connect to WiFi
  connectWiFi();
}


void loop() 
{
  // Read Light intensity in Lux(Lx) unit. Lux means luminous flux per unit area
  float lux = lightMeter.readLightLevel(); 
  Serial.print("Lux: ");        
  Serial.println(lux);
  
  // Check light intensity outside a threshold value. It can vary depending on the usage like sunlight, LED Bulbs, etc
  if (lux >= 360) {
    triggerWebhook("Sunlight falling on Terrarium");
  } 
  else if (lux < 80) {
    triggerWebhook("Terrarium is shaded");
  }
  delay(5000); // Wait for 5 seconds to take next reading
}


// Function to connect to WiFi
void connectWiFi() 
{
  Serial.println("WiFi connection in progress...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(ssid, pass);
    delay(5000); // Wait 5 seconds
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}


// Procedure to trigger the IFTTT webhook with the specified event name
void triggerWebhook(String eventName) 
{
  // Check if the WiFi is still connected, if not then try to connect again
  if (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi();
  }
  
  if (client.connect("maker.ifttt.com", 80)) 
  {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
    delay(500);
    client.stop();
    Serial.println(eventName+ "- Webhook triggered successfully");
  } 
  else {
    Serial.println("Failed to connect to webhook server");
  }
}