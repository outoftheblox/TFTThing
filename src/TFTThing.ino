#include <Thing.h>
#include <BlinkPattern.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

using namespace ootb;

Thing thing;
BlinkPattern led(BUILTIN_LED);

#define TFT_RST -1 //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_CS D4  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0
#define TFT_DC D3  //for TFT I2C Connector Shield V1.0.0 and TFT 1.4 Shield V1.0.0

// #define TFT_RST -1   //for TFT I2C Connector Shield (V1.1.0 or later)
// #define TFT_CS D0    //for TFT I2C Connector Shield (V1.1.0 or later)
// #define TFT_DC D8    //for TFT I2C Connector Shield (V1.1.0 or later)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

BlinkPattern::Pattern<2> initialize{{1,1},50};
BlinkPattern::Pattern<2> on{{39,1},25};

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("ClientID:" + thing.clientId());

    tft.initR(INITR_144GREENTAB);
    tft.setTextWrap(false); // Allow text to run off right edge
    tft.fillScreen(ST7735_BLACK);
    tft.setRotation(3);

    led.setPattern(initialize);

    showText(thing.clientId());

    thing.onStateChange([](const String& msg)
    {
        showText(msg);
        Serial.println(msg);
    });

    thing.begin();
    thing.setClientBufferSize(1024);

    String topic = "things/" + thing.clientId() + "/oled/text";
    thing.addActuator(topic, [](Value& value)
    {
        showText((String)value);
        Serial.println((String)value);
    });

    led.setPattern(on);
}

void loop()
{
    thing.handle();
}

void showText(const String& value)
{
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.println(value);
}
