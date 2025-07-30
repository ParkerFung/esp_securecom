#include "main.h"
#include "comm/interface.h"
#include "nav_interface.h"
#include "nav.h"
#include "gps/gps.h"


extern bool navBlinkState;


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


  double scale = 10.0; //default to 10m per tic mark
  if (distance > 100 && distance <= 1000) scale = 50.0;
  else if(distance > 1000) scale = 100.0;

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


  double metersPerPixel = scale / 10.0; //convert distance to pixels

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
    display.println("WAITING FOR GPS...");
  }

  }
  display.display();
}
// void nav_display(double targetLat, double targetLng){

//     display.clearDisplay();
//     display.setCursor(0,0);

//     display.println("distance to target");

//   display.setCursor(0,20);

//   if(gps.location.isValid()){
//     float currentLat = gps.location.lat();
//     float currentLng = gps.location.lng();

//     double dLat = latDistance(currentLat,targetLat);
//     double dLng = lngDistance(currentLng, targetLng, (currentLat + targetLat) / 2.0);
//     double distance = getDistance(currentLat, currentLng, targetLat, targetLng);

//     display.setCursor(0,20);
//     display.print(distance, 3);
//     display.println("m");


//     display.setCursor(0,35);
//     display.print("D_LAT: ");
//     display.print(dLat);
//     display.println("m");

//     display.setCursor(0, 50);
//     display.print("D_LNG: ");
//     display.print(dLng);
//     display.println("m");
//   }
//   else{
//     display.setCursor(0,20);
//     display.println("WAITING FOR GPS...");
//   }

//   display.display();



// }

// void drawNav(){

//   display.drawLine(10,0, 10, 80, SSD1306_WHITE);

// }
