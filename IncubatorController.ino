#include <DHT.h>
#include <U8g2lib.h>

#define DHTTYPE DHT22

char temperature [5];
char humidity [5];
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

float setPointLowTemp = 27.0;
float setPointLowHumi = 40.0;

float setPointHighTemp = 32.0;
float setPointHighHumi = 60.0;

int counter = 0;

DHT dht(7, DHTTYPE);

//u8g2 display instance
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8);

void setup() {
  pinMode(2, OUTPUT);

  dht.begin();
  Serial.begin(9600);

  pinMode(9, INPUT);
  pinMode(12, INPUT);

  u8g2.begin();
  u8g2.enableUTF8Print();

  u8g2.setColorIndex(1);
}

void loop() {

  u8g2.firstPage();
  do {
    //    draw();
    //    if(String(humidity).toFloat()>setPointHighHumi){
    //      digitalWrite(2, LOW);
    //      }else{
    //        digitalWrite(2, HIGH);
    //      }

   draw();

  } while ( u8g2.nextPage() );
}

void draw() {

  readTemperature();
  readHumidity();


  Serial.println(temperature);
  Serial.println(humidity);

    mainDisplay();
}

void menu() {
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setFont(u8g2_font_amstrad_cpc_extended_8f);
  u8g2.setFontPosTop();
  u8g2.drawStr(42, 6, "MENU");
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(4, 28, "> Set Temperature");
  u8g2.drawStr(4, 45, "> Set Humidity");

  u8g2.drawFrame(0, 0, 128, 24);
  u8g2.drawFrame(0, 23, 128, 21);
  u8g2.drawFrame(0, 43, 128, 21);

}

void mainDisplay() {
  u8g2.drawFrame(0, 0, 64, 64);
  u8g2.drawFrame(64, 0, 64, 64);
  if (counter < 1000) {
    showTemperatureHumidity();
  } else if (counter < 1500) {
    showSetPointTemperatureHumidity(true);
  } else if (counter < 2000) {
    showSetPointTemperatureHumidity(false);
  } else {
    counter = 0;
  }
  counter++;
}

void showTemperatureHumidity() {
  u8g2.setFont(u8g2_font_helvB10_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(2, 4, "PV");

  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setFontPosTop();
  u8g2.drawUTF8(22, 4, DEGREE_SYMBOL);
  u8g2.drawUTF8(28, 4, "C");
  u8g2.drawStr(90, 4, "%");

  u8g2.setFont(u8g2_font_helvB24_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(1, 28, temperature);
  u8g2.drawStr( 65, 28, humidity);
}

void showSetPointTemperatureHumidity(boolean isHigh) {

  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setFontPosTop();
  u8g2.drawUTF8(22, 4, DEGREE_SYMBOL);
  u8g2.drawUTF8(28, 4, "C");
  u8g2.drawStr(90, 4, "%");


  u8g2.setFont(u8g2_font_helvB10_tf);
  u8g2.setFontPosTop();
  u8g2.drawStr(2, 4, "SV");

  String temp;
  String humi;

  if (isHigh) {
    u8g2.drawStr(65, 4, "H");
    temp = String(setPointHighTemp);
    humi = String(setPointHighHumi);

  } else {
    u8g2.drawStr(65, 4, "L");
    temp = String(setPointLowTemp);
    humi = String(setPointLowHumi);
  }

  temp[temp.length() - 1] = '\0';
  humi[humi.length() - 1] = '\0';

  u8g2.setFont(u8g2_font_helvB24_tf);
  u8g2.setFontPosTop();

  u8g2.drawStr(1, 28, temp.c_str());
  u8g2.drawStr( 65, 28, humi.c_str());
}

void readTemperature()
{
  float t = dht.readTemperature();
  dtostrf(t, 3, 1, temperature);
}

void readHumidity()
{
  float h = dht.readHumidity();
  dtostrf(h, 3, 1, humidity);
}
