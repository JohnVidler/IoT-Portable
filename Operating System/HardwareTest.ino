#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <BBQ10Keyboard.h>
#include <SD.h>

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// System definitions
#define VBATPIN  A7
#define VMAX     4.2
#define VMIN     3.2

#define TFT_CS 9
#define TFT_DC 10
#define NEOPIXEL_PIN 11
#define SD_CS 5
#define VIBRATE_PIN A0

#define KB_UP    1
#define KB_DOWN  2
#define KB_LEFT  3
#define KB_RIGHT 4
#define KB_PUSH  5
#define KB_F1    6
#define KB_F2    11
#define KB_F3    7
#define KB_F4    12

BBQ10Keyboard keyboard;
Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
Adafruit_NeoPixel led(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
    Serial.begin( 115200 );

    tft.begin();
    tft.setRotation(1);
    tft.fillRect( 0, 0, 320, 240, BLACK );

    tft.setCursor( 0, 0 );
    tft.println( "Booting..." );
    Serial.println( "Booting..." );

    delay( 100 );
    tft.println( "   - LED & I/O" );
    led.begin();
    setLED( 255, 255, 255 );
    delay( 50 );
    setLED( 0, 0, 0 );

    /*pinMode( VIBRATE_PIN, OUTPUT );
    digitalWrite( VIBRATE_PIN, HIGH );
    delay( 100 );
    digitalWrite( VIBRATE_PIN, LOW );
    delay( 100 );
    digitalWrite( VIBRATE_PIN, HIGH );
    delay( 100 );
    digitalWrite( VIBRATE_PIN, LOW );*/


    delay( 100 );
    tft.print( "   - Battery Charge: " );
    tft.setTextColor( RED );
    tft.println( "FAIL" );
    tft.setTextColor( WHITE );

    delay( 100 );
    tft.println( "   - i2c interfaces" );
    Wire.begin();

    delay( 100 );
    tft.println( "   - Keyboard driver" );
    keyboard.begin();

    delay( 100 );
    tft.println( "   - SD Card" );
    SD.begin( SD_CS );
    //File root = SD.open("/");


    tft.println( "OK!" );
    delay( 1000 );
    tft.fillRect( 0, 0, 320, 240, BLACK );
    drawSystemBars();
    tft.setCursor( 0, 32 );

}

int cycle = 0;
int keyboardTimeout = 3000;

void loop() {
    if( keyboard.keyCount() > 0 ) {
        const BBQ10Keyboard::KeyEvent event = keyboard.keyEvent();

        keyboardTimeout = 3000;

        if( event.state == 3 ) { // This should be an enum, but I'm damned if I can figure out how to reference the lib's one for this -JV
            tft.print( event.key );
            tft.print( " " );
            tft.println( (int)event.key, HEX );
        }
    }

    if( keyboardTimeout > 0 ) {
        if( keyboardTimeout > 500 )
            keyboard.setBacklight( 1.0f );
        else
            keyboard.setBacklight( keyboardTimeout/500.0f );
        keyboardTimeout--;
    }
    

    delay( 1 );
}

void setLED( uint8_t r, uint8_t g, uint8_t b ) {
    led.setPixelColor(0, led.Color(r, g, b));
    led.show();
}

float getBatteryCharge() {
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return ((measuredvbat-VMIN)/VMAX) * 100.0;
}

inline uint16_t color( uint8_t r, uint8_t g, uint8_t b ) {
    return (((uint16_t)r & 0x1F) << 11) | (((uint16_t)g & 0x3F) << 5) | ((uint16_t)b & 0x1F);
}

inline void batteryIcon( uint16_t x, uint16_t y, uint16_t color ) {
    tft.drawRect( x+2, y+2, 9, 5, color );
    tft.drawLine( x+11, y+3, x+11, y+5, color );
}

void drawSystemBars() {
    // Top bar
    tft.fillRect( 0, 0, 320, 9, color( 255, 0, 0 ) );
    tft.setCursor( 1, 1 );

    tft.print( "100%" );
    batteryIcon( 25, 0, BLACK );
    
    // Bottom bar
    tft.fillRect( 0, 220, 320, 20, color( 0, 0, 255 ) );
}

void notificationBlink( uint8_t blinks ){
    /*keyboard.setBacklight( 1.0f );
    delay( 50 );
    keyboard.setBacklight( 0.0f );
    delay( 100 );
    while( blinks-- > 0 ) {
        keyboard.setBacklight( 1.0f );
        delay( 50 );
        keyboard.setBacklight( 0.0f );
        delay( 100 );
    }*/
}
