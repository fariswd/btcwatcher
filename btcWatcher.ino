#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define USE_SERIAL Serial
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
ESP8266WiFiMulti WiFiMulti;

//payload adalah isi dari API yang diambil
String payload;
int before = 0;

void setup() {

    //serial begin
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    //count 5 untuk init
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    //connect wifi AP
    WiFiMulti.addAP("<SSID>", "<PASSWORD>");

    //lcd begin
    lcd.begin();
    lcd.backlight();
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        USE_SERIAL.print("[HTTP] begin...\n");
        
        //begin http
        http.begin("https://vip.bitcoin.co.id/api/btc_idr/ticker", "02:1E:E8:F4:28:FF:C4:58:33:15:22:8B:FB:D7:C4:AC:D7:B0:51:10"); //HTTPS
        USE_SERIAL.print("[HTTP] GET...\n");
        
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                //USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    //json buffer & setup json parser on root
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);

    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }

    //calculate last price of BTCIDR
    int last = root["ticker"]["last"];

    //memisahkan per 3 digit dengan koma
    int last1 = last/1000000; //print jutaan
    int last2 = last%1000000;
    int last3 = last2/1000; //print ratus ribuan
    int last4 = last2%1000; //print ratusan

    String plast3, plast4;
    if(last3 == 0){
      plast3 = "000";
    } else {
      plast3 = String(last3);
    }
    if(last4 == 0){
      plast4 = "000";
    } else {
      plast4 = String(last4);
    }
    
    //print serial
    USE_SERIAL.println(last);
    USE_SERIAL.println(String(last1) + "," + String(plast3) + "," + String(plast4));

    //print lcd 1st line
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BTC: "+ String(last1) + "," + String(plast3) + "," + String(plast4));

    //print lcd 2nd line
    if(before<last){
      lcd.setCursor(0, 1);
      lcd.print("^^^^^ UP!! ^^^^^");
      before = last;
      USE_SERIAL.println(before);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("vvvv DOWN!! vvvv");
      before = last;
      USE_SERIAL.println(before);
      
    }

    //delays
    delay(5000);
    //delay(300000); //5 menit
}

