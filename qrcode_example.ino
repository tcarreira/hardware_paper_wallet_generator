#include "QRCode/src/qrcode.h"
#include "QRCode/src/qrcode.c"

const int QRcode_Version = 6;   //  set the version (range 1->40)
const int QRcode_ECC = ECC_LOW;       //  set the Error Correction level (range 0-3) or symbolic (ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH)

QRCode qrcode;                  // Create the QR code
uint8_t contrast = 150;

#define Lcd_X  128
#define Lcd_Y  64

void qr_code_setup() {
  delay(500);
  Serial.begin(9600);
  display.begin();

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN_LOW, OUTPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN_LOW, OUTPUT);

  digitalWrite(LEFT_BUTTON_PIN_LOW, LOW);
  digitalWrite(RIGHT_BUTTON_PIN_LOW, LOW);

  while (!Serial) {
  }

  display.clear();
  display.set_contrast(contrast);
  display.draw_string(0, 0 * OLED_FONT_HEIGHT, "Starting...");
  display.display();

  // Start time
  uint32_t dt = millis();

  //--------------------------------------------
  // Allocate memory to store the QR code.
  // memory size depends on version number
  uint8_t qrcodeData[qrcode_getBufferSize(QRcode_Version)];

  //--------------------------------------------
  //configure the text string to code
  // qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "{\"a\":\"DE6os4N86ef9bba6kVGurqxmhpBHKctoxY\"}"); //dARK address
  //qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "DE6os4N86ef9bba6kVGurqxmhpBHKctoxY");   //dARK address
  // qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "ark:AePNZAAtWhLsGFLXtztGLAPnKm98VVC8tJ?amount=20.3");    //ARK address

  //  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "hello pj");
  //qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "1BGJvqAuZvr23EixA65PEe5PMLAjVTeyMn");     //bitcoin address
  
  // qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "bc1q2ykvpr64564xx4fwarfzwql0fpskzfzkmcpt90");     //bitcoin address
  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "zpub6u21rjWdUtExkK1AyQYNpUJMjCu6ZxNtjf4wwRUUSMFzeoS9uQKRrpFUneijiMmV7wP4ePFxnF9EK7ZDXrJh3QT3dUAASvEGXwffZpYzoxE");     //bitcoin address

  //--------------------------------------------
  // Print Code Generation Time
  dt = millis() - dt;
  Serial.print("QR Code Generation Time: ");
  Serial.print(dt);
  Serial.print("\n");

  // //--------------------------------------------
  // //    This prints the QR code to the serial monitor as solid blocks. Each module
  // //    is two characters wide, since the monospace font used in the serial monitor
  // //    is approximately twice as tall as wide.
  // // Top quiet zone
  // Serial.print("\n\n\n\n");
  // for (uint8_t y = 0; y < qrcode.size; y++) {
  //   // Left quiet zone
  //   Serial.print("        ");
  //   // Each horizontal module
  //   for (uint8_t x = 0; x < qrcode.size; x++) {
  //     // Print each module (UTF-8 \u2588 is a solid block)
  //     Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588" : "  ");
  //   }
  //   Serial.print("\n");
  // }
  // // Bottom quiet zone
  // Serial.print("\n\n\n\n");


  // //--------------------------------------------
  // //display generation time to OLED display
  // display.clear();
  // display_printf(0, 1 * OLED_FONT_HEIGHT, "Generation Time(ms): %d ms", dt);
  // display_printf(0, 2 * OLED_FONT_HEIGHT, "     %d ms", dt);
  // display.display();
  // delay(1000);

  //--------------------------------------------
  //Turn on all pixels
  display.clear();
  for (uint8_t y = 0; y < Lcd_Y; y++) {
    for (uint8_t x = 0; x < Lcd_X; x++) {
      display.draw_pixel(x, y, OLED::WHITE);
    }
  }
  display.display();


  //--------------------------------------------
  //setup the top left corner of the QRcode
  uint8_t x0 = 50;
  uint8_t y0 =  3;   //16 is the start of the blue portion OLED in the yellow/blue split 64x128 OLED

  //--------------------------------------------
  //display QRcode
  // display.clear();
  // display.inverse();
  display.display();
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {

      OLED::tColor pixel_color = OLED::BLACK;
      if (qrcode_getModule(&qrcode, x, y) == 0) {     //change to == 1 to make QR code with black background
        pixel_color = OLED::WHITE;
      }

      if (QRcode_Version > 3) {
        //uncomment to display code in normal size
        display.draw_pixel(x0 + x, y0 + y, pixel_color);
      } else {
        //uncomment to double the QRcode (max QRcode_Version==3)
        display.draw_pixel(x0 + 2 * x,     y0 + 2 * y, pixel_color);
        display.draw_pixel(x0 + 2 * x + 1, y0 + 2 * y, pixel_color);
        display.draw_pixel(x0 + 2 * x,     y0 + 2 * y + 1, pixel_color);
        display.draw_pixel(x0 + 2 * x + 1, y0 + 2 * y + 1, pixel_color);
      }

    }
  }
  display.display();
}

void qr_code_loop() {
  unsigned int pressed = button_pressed();
  if ( pressed & BUTTON_LEFT_PRESS ) {
    contrast -= 20;
    if (contrast < 1) {
      contrast = 1;
    }
    delay(500);
  } else if (pressed & BUTTON_RIGHT_PRESS) {
    contrast += 20;
    if (contrast > 254) {
      contrast = 254;
    }
    delay(500);
  }
  display.set_contrast(contrast);
  display.display();
  delay(100);
}

