#include <Arduino.h>
#include <oled.h>
#include <string.h>

#include "bip39_words.h"
#include "bip39_test_vectors.h"

#define PIN_I2C_SDA 5
#define PIN_I2C_SCL 4
#define PIN_I2C_RESET NO_RESET_PIN
#define I2C_ADDRESS 0x3C

#define LEFT_BUTTON_PIN 25
#define LEFT_BUTTON_PIN_LOW 27
#define RIGHT_BUTTON_PIN 17
#define RIGHT_BUTTON_PIN_LOW 18

// There are 2 different versions of the board
// OLED display=OLED(2,14,4);
// OLED display=OLED(4,5,16);
// OLED display=OLED(5,4,16); // working!!!! TTGO T1
// OLED display=OLED(5,4); // working!!!! TTGO T1
OLED display = OLED(PIN_I2C_SDA, PIN_I2C_SCL, PIN_I2C_RESET, OLED::W_128, OLED::H_64, OLED::CTRL_SSD1306, I2C_ADDRESS);

static const uint8_t bitmap[] = {
    1, 2, 4, 8, 16, 32, 64, 128, 128, 128, 192, 192, 128, 128, 128, 64, 32, 16, 8, 4, 2, 1,        // first page (8 vertical bits, 22 columns)
    255, 255, 255, 255, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 255, 255, 255, 255 // second page (8 vertical bits, 22 columns)
};

void setup()
{
  delay(500);
  Serial.begin(9600);
  display.begin();

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN_LOW, OUTPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN_LOW, OUTPUT);

  digitalWrite(LEFT_BUTTON_PIN_LOW, LOW);
  digitalWrite(RIGHT_BUTTON_PIN_LOW, LOW);

  while (!Serial) {}
}

void loop()
{
  // Draw pixels in the outer edges
  display.draw_pixel(0, 0);
  display.draw_pixel(127, 0);
  display.draw_pixel(127, 63);
  display.draw_pixel(0, 63);
  display.display();

  delay(500);

  // Draw text with normal size
  display.draw_string(20,2,"Sending text");
  display.draw_string(20,15,"over serial");
  display.display();
  delay(500);

  for (int i=0 ; i<BIP39_VECTOR_COUNT ; i++) {
    Serial.printf("### Test vector #%d\n", i);
    Serial.printf("randomBytes (byte): ");
    for (int j=0; j<vectorEntropyBytesLength[i]; j++){
      Serial.printf("%02x", vectorEntropy[i][j]);
    }
    Serial.printf("\nrandomBytes (str ): %s\n", vectorEntropyStr[i]);
    Serial.printf("seed: %s\n\n", vectorSeed[i]);
  }
  display.clear();

  for (int i = 0; i < 8; i++)
  {
    display.draw_string(0 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i]));
    display.draw_string(5 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 1]));
    display.draw_string(10 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 2]));
    display.draw_string(15 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 3]));
    display.display();
    delay(0);
  }

  for (int i = 8; i < BIP39_NUMBER_OF_WORDS / 4; i++)
  {
    while (digitalRead(LEFT_BUTTON_PIN) == LOW)
    {
      delay(10);
    }
    display.scroll_up(OLED_FONT_HEIGHT, 0);
    display.draw_string(0 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i]));
    display.draw_string(5 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 1]));
    display.draw_string(10 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 2]));
    display.draw_string(15 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, prefix((char *)BIP39_WORDS[4 * i + 3]));
    display.display();
    delay(0);
  }
  delay(1000);
  display.clear();

  // Draw hollow circles
  for (uint_least8_t radius = 3; radius < 62; radius += 3)
  {
    delay(50);
    display.draw_circle(64, 16, radius);
    if (radius > 15)
    {
      display.draw_circle(64, 16, radius - 15, OLED::SOLID, OLED::BLACK);
    }
    display.display();
  }

  // Draw solid circles
  delay(500);
  display.draw_circle(36, 16, 14, OLED::SOLID);
  display.display();
  delay(500);
  display.draw_circle(36, 16, 7, OLED::SOLID, OLED::BLACK);
  display.display();

  // Draw rectangles
  delay(500);
  display.draw_rectangle(64, 0, 98, 31);
  display.display();
  delay(500);
  display.draw_rectangle(69, 5, 93, 26, OLED::SOLID);
  display.display();

  // scroll up
  delay(1000);
  display.scroll_up(32, 20);

  // Draw text with normal size
  display.draw_string(4, 2, "Hello");
  display.display();

  // Draw a line
  delay(1000);
  display.draw_line(4, 10, 34, 10);
  display.display();

  // Draw text from program memory with double size
  delay(1000);
  display.draw_string_P(16, 15, PSTR("World!"), OLED::DOUBLE_SIZE);
  display.display();

  // Draw a cross
  delay(1000);
  display.draw_line(16, 31, 88, 15);
  display.draw_line(16, 15, 88, 31);
  display.display();

  // Draw a raw bitmap
  delay(1000);
  display.draw_bitmap_P(100, 8, 22, 16, bitmap);
  display.display();

  // Demonstrate scrolling
  delay(1000);
  display.set_scrolling(OLED::HORIZONTAL_RIGHT);
  delay(3000);
  display.set_scrolling(OLED::HORIZONTAL_LEFT);
  delay(3000);
  display.set_scrolling(OLED::NO_SCROLLING);

  // Redraw after scrolling to get the original picture
  display.display();

  // Flash the display
  delay(1000);
  for (int i = 0; i < 10; i++)
  {
    display.set_invert(true);
    delay(200);
    display.set_invert(false);
    delay(200);
  }

  // Show contrast values
  contrast(128);
  contrast(64);
  contrast(32);
  contrast(16);
  contrast(8);
  contrast(4);
  contrast(2);
  contrast(1);
  contrast(2);
  contrast(4);
  contrast(8);
  contrast(16);
  contrast(32);
  contrast(64);
  contrast(128);
  contrast(255);
  contrast(128);
  delay(3000);

  display.clear();
}

void contrast(int value)
{
  char buffer[4];
  display.clear();
  display.draw_string(0, 0, "Contrast:");
  itoa(value, buffer, 10);
  display.draw_string(64, 0, buffer);
  display.draw_rectangle(0, 20, value / 2, 31, OLED::SOLID);
  display.display();
  display.set_contrast(value);
  delay(500);
}

char *prefix(char *str)
{
  static char out[5];

  if (strlen(str) <= 4)
  {
    sprintf(out, "%4s", str);
  }
  else
  {
    memcpy(out, str, 4);
    out[5] = '\0';
  }

  return out;
}
