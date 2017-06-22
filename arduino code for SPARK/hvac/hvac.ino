#include "DHT.h"
#include <Adafruit_NeoPixel.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;

#define DHTPIN 5     // what pin we're connected to
#define PINLED 4
#define BRIGHTNESS 50


// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16


// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PINLED, NEO_GRB + NEO_KHZ800);

void setup() {
  
  Serial.begin(9600);   
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    
    
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library

  //init
  WiFiMulti.addAP("Spark Base", "spark123");
   dht.begin();
   pixels.show(); 

   
   
}

void loop() {

// wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://192.168.0.103:3000/hvac"); //HTTP

USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
http.addHeader("Content-Type", "application/x-www-form-urlencoded");

// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();


 // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");


    
    if(t < 28){
      
       lightColor(pixels.Color(255,0,0), 10); // Red
       Serial.println("on heater!");
    }else{
       lightColor(pixels.Color(0, 0, 255), 10); // Blue
       Serial.println("on Aircon!");
    }   
  }

int httpCode = http.POST("temp="+String(t)+"&humid="+String(h));
http.writeToStream(&Serial);

       

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(10000);





  
  

 

}

// Fill the dots one after the other with a color
void lightColor(uint32_t c, uint8_t wait) {

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, c); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(wait); // Delay for a period of time (in milliseconds).

  }
 
 
}
