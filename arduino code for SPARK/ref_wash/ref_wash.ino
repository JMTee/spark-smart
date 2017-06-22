#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;



#define ROTARY_ANGLE_SENSOR A0
#define LED 5//the Crowtial- LED is connected to D5 of Arduino
#define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the Crowduino
#define Crowtail_VCC 5//VCC of the grove interface is normally 5v
#define FULL_ANGLE 300//full value of the rotary angle is 300 degrees


int ledpin=4;
 int button = 0;
void setup()
{
   Serial.begin(9600); 

   USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    
  pinMode(ledpin,OUTPUT);
  pinMode(button,INPUT); 
   pinMode(ROTARY_ANGLE_SENSOR, INPUT);
  pinMode(LED,OUTPUT);

   //init
  WiFiMulti.addAP("Spark Base", "spark123");
}
void loop()
{

  if(digitalRead(button)){
  digitalWrite(ledpin,HIGH);
 }
 else{
 digitalWrite(ledpin,LOW);
 }



  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://192.168.0.103:3000/ref-dry"); //HTTP

USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
http.addHeader("Content-Type", "application/x-www-form-urlencoded");




int degrees=0;
  degrees = getDegree();
  Serial.println("The angle between the mark and the starting position:");
  Serial.println(degrees);  
  
int httpCode = http.POST("degree="+String(degrees)+"&dryoff="+digitalRead(button));
http.writeToStream(&Serial);

  int brightness;
  /*The degrees is 0~300, should be converted to be 0~255 to control the*/
  /*brightness of LED                           */
  brightness = map(degrees, 0, FULL_ANGLE, 0, 255); 
  analogWrite(LED,brightness); /*The range of brightness is 0~255 */  
  
 




        
       

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

    delay(1000);





 
}


/************************************************************************/
/*Function: Get the angle between the mark and the starting position  */  
/*Return: -int,the range of degrees is 0~300 */
int getDegree()
{
  int sensor_value = analogRead(ROTARY_ANGLE_SENSOR);
  float voltage;
  voltage = (float)sensor_value*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/Crowtail_VCC;
  return degrees;
}


