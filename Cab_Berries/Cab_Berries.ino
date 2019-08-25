/**
   BasicHTTPSClient.ino

    Created on: 20.08.2018

*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <FastLED.h>
#include <ESP8266HTTPClient.h>

int goldBerries = 1;     
int blueBerries = 1;
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#include <WiFiClientSecureBearSSL.h>
#define NUM_LEDS 100
#define UPDATES_PER_SECOND 100
int ledPin = 5; // GPIO13
int value = 0;
int berrydepositdelay = 15;
ESP8266WiFiMulti WiFiMulti;
 CRGB leds[NUM_LEDS];

 CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {

  LEDS.addLeds<WS2812,5,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(255);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  pinMode(ledPin, OUTPUT);
    
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  //turn off all berries
  for (int i = 0; i < NUM_LEDS; i++){ 
    leds[i] = CRGB ::Black;
  }
  FastLED.show();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("ToscheStation", "fourwordsalluppercase");
  WiFiMulti.addAP("ToscheStation5G", "fourwordsalluppercase");
  WiFiMulti.addAP("Frijoles", "holyfriJoles");
  WiFiMulti.addAP("baltimorenode", "spencerthedog");
  WiFiMulti.addAP("Butts", "buttsButts");
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    //DynamicJsonBuffer jsonBuffer(200);
    //DynamicJsonBuffer jsonBuffer2(1000);
    DynamicJsonDocument doc(2048);

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint("ea583e59a73de64613d78b36de1f933b01f2b795");

    HTTPClient https;

    Serial.print("new_code[HTTPS] begin...\n");
    if (https.begin(*client, "https://bmorekq.com/api/gamepayload")) {
   // if (https.begin(*client, "https://bmorekq.com/api/matches/2004/gamepayload")) {
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      https.setTimeout(2000);
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        
        Serial.println(HTTP_CODE_OK);
        
        // file found at server
       
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          Serial.println(" ok "); 
                  
          //  DynamicJsonDocument jsonBuffer(1024);
          //    deserializeJson(jsonBuffer, https.getString());
          // const char* sensor = jsonBuffer[""];
          //  Serial.println(sensor);
          
          //we're going to declare it as a static array rather than a dynamic one.
          
          
          //StaticJsonBuffer<5000> jsonBuffer;
         
          //JsonObject& root = doc.parseObject(https.getString());
          deserializeJson(doc, https.getString());
          
          //doc["in_progress"].prettyPrintTo(Serial);
      
          Serial.println("line 97 http ok");
          bool inprogress = doc["in_progress"];
          //inprogress = true;
            if (inprogress == false) {
              Serial.println("Sorry, no game is happening :( ");
              //Serial.println("Wait before next round...");
              //delay(50);

             for(int j = 1; j < 7500; j++){
                ChangePalettePeriodically();
                static uint8_t startIndex = 0;
                startIndex = startIndex + 1; 
    
                FillLEDsFromPaletteColors( startIndex);
                
                FastLED.show();
                FastLED.delay(1000 / UPDATES_PER_SECOND);
              }
              
              turnoffleds();
            } 
            else { 
              Serial.println("YAY! game in progress!!!!! "); 
              
              JsonArray berries = doc["berries"];
              Serial.println(berries.size());
              
              
              
              int berrytest = 0;

             
              int newBlueBerries = 0;
              int newGoldBerries = 0;
              for (int i = 0; i < berries.size(); i++){
                //doc["berries"][i]["playerId"].prettyPrintTo(Serial);
                
              
              
                int berrytest = doc["berries"][i]["playerId"];
                Serial.println("Player number:");
                Serial.println(berrytest);
                Serial.println("");
                if (berrytest % 2 == 1){
                  newGoldBerries++;
                } else {
                  newBlueBerries++;
                }
              }
              if (newBlueBerries>blueBerries && blueBerries !=0){
                for (blueBerries; blueBerries<=newBlueBerries; blueBerries++){
                    for (int j = 0; j < 10; j++) { 
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[52+4*blueBerries-4] = CRGB(50,0,50);
                      leds[52+4*blueBerries-3] = CRGB(100,0,100);
                      leds[52+4*blueBerries-2] = CRGB(255,0,255);
                      leds[52+4*blueBerries-1] = CRGB(0,0,0);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[52+4*blueBerries-4] = CRGB(0,0,0);
                      leds[52+4*blueBerries-3] = CRGB(50,0,50);
                      leds[52+4*blueBerries-2] = CRGB(100,0,100);
                      leds[52+4*blueBerries-1] = CRGB(255,0,255);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[52+4*blueBerries-4] = CRGB(255,0,255);
                      leds[52+4*blueBerries-3] = CRGB(0,0,0);
                      leds[52+4*blueBerries-2] = CRGB(50,0,50);
                      leds[52+4*blueBerries-1] = CRGB(100,0,100);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[52+4*blueBerries-4] = CRGB(100,0,100);
                      leds[52+4*blueBerries-3] = CRGB(255,0,255);
                      leds[52+4*blueBerries-2] = CRGB(0,0,0);
                      leds[52+4*blueBerries-1] = CRGB(50,0,50);
                      FastLED.show();
                      delay(berrydepositdelay);
                    }
                    leds[52+4*blueBerries-4] = CRGB(255,0,255);
                    leds[52+4*blueBerries-3] = CRGB(255,0,255);
                    leds[52+4*blueBerries-2] = CRGB(255,0,255);
                    leds[52+4*blueBerries-1] = CRGB(255,0,255);
                    FastLED.show();
                  }
                  blueBerries = newBlueBerries;
                }
                if (newGoldBerries>goldBerries){
                  for (goldBerries; goldBerries<=newGoldBerries; goldBerries++){
                    for (int j = 0; j < 10; j++){ 
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[4*goldBerries-4] = CRGB(50,0,50);
                      leds[4*goldBerries-3] = CRGB(100,0,100);
                      leds[4*goldBerries-2] = CRGB(255,0,255);
                      leds[4*goldBerries-1] = CRGB(0,0,0);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[4*goldBerries-4] = CRGB(0,0,0);
                      leds[4*goldBerries-3] = CRGB(50,0,50);
                      leds[4*goldBerries-2] = CRGB(100,0,100);
                      leds[4*goldBerries-1] = CRGB(255,0,255);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[4*goldBerries-4] = CRGB(255,0,255);
                      leds[4*goldBerries-3] = CRGB(0,0,0);
                      leds[4*goldBerries-2] = CRGB(50,0,50);
                      leds[4*goldBerries-1] = CRGB(100,0,100);
                      FastLED.show();
                      delay(berrydepositdelay);
                      leds[4*goldBerries-4] = CRGB(100,0,100);
                      leds[4*goldBerries-3] = CRGB(255,0,255);
                      leds[4*goldBerries-2] = CRGB(0,0,0);
                      leds[4*goldBerries-1] = CRGB(50,0,50);
                      FastLED.show();
                      delay(berrydepositdelay);            
                    }
                    leds[4*goldBerries-4] = CRGB(255,0,255);
                    leds[4*goldBerries-3] = CRGB(255,0,255);
                    leds[4*goldBerries-2] = CRGB(255,0,255);
                    leds[4*goldBerries-1] = CRGB(255,0,255);
                    FastLED.show();          
                  }
                  goldBerries = newGoldBerries;
                }
                Serial.println("Blue Berries");
                Serial.println(blueBerries);
                Serial.println("");
                Serial.println("Gold Berries");
                Serial.println(goldBerries);
                Serial.println("");
                
                if (blueBerries == 12) {
                  Serial.println("Blue won");
                  blueBerries = 0 ;
                  goldBerries = 0;
                  SetupBlueWinPalette();
                  makelightpattern();
                  turnoffleds();
                }
                if (goldBerries == 12) {
                  Serial.println("Gold won");
                  blueBerries = 0;
                  goldBerries = 0;
                  SetupGoldWinPalette();
                  makelightpattern();
                  turnoffleds();
                }
              }
                    
                  
                

            }
        }

      
    }
    doc.clear();
    //jsonBuffer.clear();
  }
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } } 

void makelightpattern(){
for(int j = 1; j < 1500; j++){
                //ChangePalettePeriodically();
                static uint8_t startIndex = 0;
                startIndex = startIndex + 1;     
                FillLEDsFromPaletteColors( startIndex);
                FastLED.show();
                FastLED.delay(1000 / UPDATES_PER_SECOND);
              }
}

              
void SetupPurpleAndGreenPalette()
{
  CRGB blue = CHSV( HUE_BLUE, 255, 255);
  CRGB orange  = CHSV( HUE_ORANGE, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16(
                                  blue,  blue,  black,  black,
                                  orange, orange, black,  black,
                                  blue,  blue,  black,  black,
                                  orange, orange, black,  black );
}
void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::Orange;
  currentPalette[4] = CRGB::Orange;
  currentPalette[8] = CRGB::Orange;
  currentPalette[12] = CRGB::Orange;
    
}

void SetupBlueWinPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::Blue;
  currentPalette[4] = CRGB::Blue;
  currentPalette[8] = CRGB::Blue;
  currentPalette[12] = CRGB::Blue;
  
}
void turnoffleds(){
//turn off all berries
              for (int i = 0; i < NUM_LEDS; i++){ 
                leds[i] = CRGB ::Black;
              }
              FastLED.show();
}

void SetupGoldWinPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::Orange;
  currentPalette[4] = CRGB::Orange;
  currentPalette[8] = CRGB::Orange;
  currentPalette[12] = CRGB::Orange;
  
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Orange,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,
  
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Blue,
  CRGB::Blue,
  
  CRGB::Orange,
  CRGB::Orange,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black,
};



void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 180;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 20)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
    if( secondHand == 30)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
    //if( secondHand == 40)  { SetupBlueWinPalette();       currentBlending = NOBLEND; }
    //if( secondHand == 50)  { SetupBlueWinPalette();       currentBlending = LINEARBLEND; }
    //if( secondHand == 60)  { SetupGoldWinPalette();       currentBlending = NOBLEND; }
    //if( secondHand == 70)  { SetupGoldWinPalette();       currentBlending = LINEARBLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 50)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 60)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 70)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
  }
}
