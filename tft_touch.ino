
    /*
     Demonstrates class Adafruit_GFX_Button
     For Adafruit 2.8" TFT LCD - resistive.
    */

    #include <SPI.h>
    #include <Wire.h>
    #include "Adafruit_GFX.h"
    #include "Adafruit_ILI9341.h"
    #include "Adafruit_STMPE610.h"

    // Default values for Adafruit shield v2.
    #define STMPE_CS 6
   #define TFT_CS   9
   #define TFT_DC   10
   #define SD_CS    5

    Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
    Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
    Adafruit_GFX_Button measBtn, calBtn, attBtn, valBtn;

    // Assign human-readable names to some common 16-bit color values:
    #define   BLACK   0x0000
    #define   BLUE    0x001F
    #define   RED     0xF800
    #define   GREEN   0x07E0
    #define CYAN    0x07FF
    #define MAGENTA 0xF81F
    #define YELLOW  0xFFE0
    #define WHITE   0xFFFF

    #define TS_MINX 3800
    #define TS_MAXX 100
    #define TS_MINY 100
    #define TS_MAXY 3750

    #define BUTTON_W 80
    #define BUTTON_H 40

    #define BUTTON_WH 40
    #define BUTTON_HH 20

     // Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
     // Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
     byte rotation = 3; //(0->3)
     TS_Point p;
     int x, y;

     void btnHandler()
     {
      // inverted
      measBtn.drawButton(true);
      tft.setCursor(30, 60);
      tft.setTextSize(2);
      tft.print("ok btn hit.");
      delay(600);
      // normal
      measBtn.drawButton(false);
      // erase text
      tft.fillRect( 30, 60, tft.width()-10, 20, BLACK );
     }
     
     void setup()
     {
      tft.begin();
      ts.begin();
      if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
        while (1);
      }
      tft.setRotation(rotation);
      tft.fillScreen(BLACK);
      
      measBtn.initButton( &tft, BUTTON_WH, BUTTON_HH, BUTTON_W, BUTTON_H, RED, WHITE, BLACK,"MEAS", 2 );
      measBtn.drawButton(false);

      calBtn.initButton( &tft, BUTTON_WH + BUTTON_W, BUTTON_HH, BUTTON_W, BUTTON_H, RED, WHITE, BLACK,"CAL", 2 );
      calBtn.drawButton(false);

      attBtn.initButton( &tft, BUTTON_WH + BUTTON_W * 2, BUTTON_HH, BUTTON_W, BUTTON_H, RED, WHITE, BLACK,"ATT", 2 );
      attBtn.drawButton(false);

      valBtn.initButton( &tft, BUTTON_WH + BUTTON_W * 3, BUTTON_HH, BUTTON_W, BUTTON_H, RED, WHITE, BLACK,"VAL", 2 );
      valBtn.drawButton(false);
     }

    void loop()
    {
     if (!ts.bufferEmpty()) { 
      p = ts.getPoint();
     switch (rotation){
      case 0:
      x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
      break;
     case 1:
     // p.x, p.y reversed //
       x = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
       y = map(p.x, TS_MAXX, TS_MINX, 0, tft.height());
       break;
     case 2:
      x = map(p.x, TS_MAXX, TS_MINX, 0, tft.width());
      y = map(p.y, TS_MAXY, TS_MINY, 0, tft.height());
      break;
     case 3:
     //  p.x, p.y reversed //
      x = map(p.y, TS_MAXY, TS_MINY, 0, tft.width());
      y = map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
      break;
     }

     while (ts.touched()) {
      Serial.print("\tPressure = "); Serial.println(p.z);
      
      if (measBtn.contains(x,y)) {
          measBtn.press(true);
        } else {
          measBtn.press(false); 
        } 
       if (measBtn.isPressed()) {
          btnHandler();
        }
      }
     }
    }

