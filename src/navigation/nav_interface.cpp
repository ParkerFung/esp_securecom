#include "main.h"
#include "comm/interface.h"
#include "nav_interface.h"
#include "nav.h"
#include "gps/gps.h"


extern bool navBlinkState;
extern double navScale = 10.0;

void nav_display(double targetLat, double targetLng){

  display.clearDisplay();

  //top of UI
  if(gps.location.isValid()){
  float currentLat = userLat;
  float currentLng = userLng;

  double distance = getDistance(currentLat, currentLng, targetLat, targetLng);

  display.setCursor(0,0);
  display.print("DIST: ");
  display.print(distance);
  display.println(" m");

  double dLat = latDistance(currentLat,targetLat);
  double dLng = lngDistance(currentLng, targetLng, (currentLat + targetLat) / 2.0);

  const int originX = 64;
  const int originY = 40;

  display.drawLine(0, originY, 127, originY, SSD1306_WHITE); //x axis
  display.drawLine(originX, 17, originX, 63, SSD1306_WHITE); //y axis
  
  for (int x = originX % 10; x < 128; x += 10) {
    if (x >= 0 && x < 128) {
      display.drawPixel(x, originY - 1, SSD1306_WHITE);
      display.drawPixel(x, originY + 1, SSD1306_WHITE);
    }
  }
  for (int y = originY % 10; y < 64; y += 10) {
    if (y >= 17 && y < 64) {
      display.drawPixel(originX - 1, y, SSD1306_WHITE);
      display.drawPixel(originX + 1, y, SSD1306_WHITE);
    }
  }

  double metersPerPixel = navScale / 10.0; //convert distance to pixels

  int xOffset = (int)(dLng / metersPerPixel);
  int yOffset = (int)(dLat / metersPerPixel);

  int targetX = 64 + xOffset;
  int targetY = 32 - yOffset; //y is inverted

  if (distance > 1000.0) {
    if (targetX < 0) targetX = 0;
    if (targetX > 127) targetX = 127;
    if (targetY < 17) targetY = 17;
    if (targetY > 63) targetY = 63;
  }
  if (navBlinkState) {
    if (targetX >= 0 && targetX < 128 && targetY >= 17 && targetY < 64) {
      display.fillCircle(targetX, targetY, 2, SSD1306_WHITE);
    }
  }
  else{
    display.setCursor(0,20);
  }
  }

  //-------------------------------------------
  int scaleBarX = 5;
  int scaleBarY = 60;
  int maxScaleBarWidth = 30; 

  double metersPerPixel = navScale / 10.0;
  double scaleBarDistance = metersPerPixel * maxScaleBarWidth;

  int scaleOptions[] = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000};
  int chosenDistance = scaleOptions[0];

  for (int i = 0; i < sizeof(scaleOptions) / sizeof(scaleOptions[0]); i++) {
    if (scaleOptions[i] > scaleBarDistance) {
      chosenDistance = scaleOptions[i];
      break;
    }
  }

  int barWidthPixels = chosenDistance / metersPerPixel;
  if (barWidthPixels > maxScaleBarWidth) {
    barWidthPixels = maxScaleBarWidth;
  }

  display.drawLine(scaleBarX, scaleBarY, scaleBarX + barWidthPixels, scaleBarY, SSD1306_WHITE);

  display.drawPixel(scaleBarX, scaleBarY - 1, SSD1306_WHITE);
  display.drawPixel(scaleBarX + barWidthPixels, scaleBarY - 1, SSD1306_WHITE);

  int labelX = scaleBarX + barWidthPixels + 2;
  int labelY = scaleBarY - 4;

  display.setCursor(labelX, labelY);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (chosenDistance >= 1000) {
    display.print(chosenDistance / 1000);
    display.println(" km");
  } else {
    display.print(chosenDistance);
    display.println(" m");
  }

  //-----------------------------------------------
  display.display();
}