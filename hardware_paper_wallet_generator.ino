#include <Arduino.h>
#include <oled.h>
#include <string.h>

#include "bip39.h"
#include "bip39_test_vectors.h"
#include "bip39_words.h"

#define PIN_I2C_SDA 5
#define PIN_I2C_SCL 4
#define PIN_I2C_RESET NO_RESET_PIN
#define I2C_ADDRESS 0x3C

#define LEFT_BUTTON_PIN 25
#define LEFT_BUTTON_PIN_LOW 27
#define RIGHT_BUTTON_PIN 17
#define RIGHT_BUTTON_PIN_LOW 18
#define BUTTON_NO_PRESS 0
#define BUTTON_LEFT_PRESS 1
#define BUTTON_RIGHT_PRESS 2

#define ENTROPY_BITS 256
#define ENTROPY_BYTES (ENTROPY_BITS / 8)

#define OLED_BOTTOM_LINE (7 * OLED_FONT_HEIGHT)

// There are 2 different versions of the board
// OLED display=OLED(2,14,4);
// OLED display=OLED(4,5,16);
// OLED display=OLED(5,4,16); // working!!!! TTGO T1
// OLED display=OLED(5,4); // working!!!! TTGO T1
OLED display = OLED(PIN_I2C_SDA, PIN_I2C_SCL, PIN_I2C_RESET, OLED::W_128, OLED::H_64, OLED::CTRL_SSD1306, I2C_ADDRESS);

static const uint8_t bitmap[] = {
    1,   2,   4,   8,   16, 32, 64, 128, 128, 128, 192,
    192, 128, 128, 128, 64, 32, 16, 8,   4,   2,   1, // first page (8 vertical bits, 22 columns)
    255, 255, 255, 255, 15, 15, 15, 15,  15,  15,  15,
    15,  15,  15,  15,  15, 15, 15, 255, 255, 255, 255 // second page (8 vertical bits, 22 columns)
};

void setup() {
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

  randomSeed(analogRead(0));
}

void loop() {
  display.clear();
  display.draw_string(0, 2 * OLED_FONT_HEIGHT, "       Bitcoin       ");
  display.draw_string(0, 3 * OLED_FONT_HEIGHT, "     Paper Wallet    ");
  display.draw_string(0, 4 * OLED_FONT_HEIGHT, "      Generator      ");
  display.draw_string(0, 1 * OLED_BOTTOM_LINE, "    (wip: 2023-02-28)");
  display.display();
  delay(1500);

  /*****************************************************************************
  // GENERATE ENTROPY
  *****************************************************************************/
  // generate Random entropy (wip: not crypto safe)
  unsigned char entropy[ENTROPY_BYTES];
  generate_entropy(entropy, ENTROPY_BYTES);
  display_draw_entropy(entropy, ENTROPY_BYTES);

  // wait for right button to continue
  while (button_pressed() != BUTTON_RIGHT_PRESS) {
    delay(10);
  }
  display.clear();
  delay(500);

  /*****************************************************************************
  // GENERATE MNEMONIC
  *****************************************************************************/
  // generate mnemonic from entropy
  const char **mnemonic = entropy_to_words(entropy, ENTROPY_BYTES);
  display_draw_mnemonic_screen(mnemonic, ENTROPY_BITS);

  display.clear();
  delay(500);

  //
  //
  //
  //

  // Draw text with normal size
  display.draw_string(20, 2, "Sending text");
  display.draw_string(20, 15, "over serial");
  display.display();
  delay(400);

  display.clear();
  for (int i = 0; i < BIP39_VECTOR_COUNT; i++) {

    Serial.printf("### Test vector #%d\n", i);
    Serial.printf("entropy (byte): ");
    display_printf(0 * OLED_FONT_WIDTH, 0 * OLED_FONT_HEIGHT, "# Test vector #%d", i);

    unsigned char *entropy = (unsigned char *)malloc(vectorEntropyBytesLengthAtIndex(i));
    vectorEntropyAtIndex(entropy, i);

    for (int j = 0; j < vectorEntropyBytesLengthAtIndex(i); j++) {
      Serial.printf("%02x", entropy[j]);
      display_printf((2 * (j % 8)) * OLED_FONT_WIDTH, (j / 8 + 2) * OLED_FONT_HEIGHT, "%02x", entropy[j]);
    }
    Serial.printf("\n");

    display.display();
    delay(1000);
    display.clear();
  }

  for (int i = 0; i < 8; i++) {
    display.draw_string(0 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, getwordat(4 * i));
    display.draw_string(5 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, getwordat(4 * i + 1));
    display.draw_string(10 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, getwordat(4 * i + 2));
    display.draw_string(15 * OLED_FONT_WIDTH, i * OLED_FONT_HEIGHT, getwordat(4 * i + 3));
    display.display();
    delay(0);
  }

  for (int i = 8; i < BIP39_NUMBER_OF_WORDS / 4; i++) {
    while (digitalRead(LEFT_BUTTON_PIN) == LOW) {
      delay(10);
    }
    display.scroll_up(OLED_FONT_HEIGHT, 0);
    display.draw_string(0 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, getwordat(4 * i));
    display.draw_string(5 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, getwordat(4 * i + 1));
    display.draw_string(10 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, getwordat(4 * i + 2));
    display.draw_string(15 * OLED_FONT_WIDTH, 7 * OLED_FONT_HEIGHT, getwordat(4 * i + 3));
    display.display();
    delay(0);
  }
  delay(1000);
  display.clear();

  // Draw hollow circles
  for (uint_least8_t radius = 3; radius < 62; radius += 3) {
    delay(50);
    display.draw_circle(64, 16, radius);
    if (radius > 15) {
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
  for (int i = 0; i < 10; i++) {
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

void contrast(int value) {
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

char *getwordat(int i) {
  static char out[5];

  char str[40];
  bip39_word_at_index(str, i);

  if (strlen(str) <= 4) {
    sprintf(out, "%4s", str);
  } else {
    memcpy(out, str, 4);
    out[5] = '\0';
  }

  return out;
}

int button_pressed() {
  if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
    return BUTTON_LEFT_PRESS;
  }
  if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
    return BUTTON_RIGHT_PRESS;
  }
  return BUTTON_NO_PRESS;
}

void display_printf(unsigned int x, unsigned int y, const char *format, ...) {
  char buffer[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  display.draw_string(x, y, buffer);
}

void generate_entropy(unsigned char *entropy, size_t len) {
  for (size_t i = 0; i < len; i++) {
    entropy[i] = random(0, 256);
  }
}

void display_draw_entropy(const unsigned char *entropy, size_t len) {
  display.clear();
  display.draw_string(0, 0, "Random seed UNSAFE! :");

  // print left column index
  for (int i = 0; i < len; i += 8) {
    display_printf(0 * OLED_FONT_WIDTH, (i / 8 + 2) * OLED_FONT_HEIGHT, "%02d:", i);
  }
  // print bytes
  for (int j = 0; j < len; j++) {
    display_printf(((2 * (j % 8)) + 4) * OLED_FONT_WIDTH, (j / 8 + 2) * OLED_FONT_HEIGHT, "%02x", entropy[j]);
  }

  display.draw_string(0, OLED_BOTTOM_LINE, "        show words ->");
  display.display();
}

void display_draw_mnemonic(const char **mnemonic, unsigned int entropy_bits, int screen) {
  display.clear();
  display_printf(0, 0, "Mnemonics:     (%d/%d)", screen + 1, number_of_words_from_entropy_size(entropy_bits) / 4);
  display_printf(0, 2 * OLED_FONT_HEIGHT, "%02d: %s", screen * 4 + 1, mnemonic[screen * 4 + 0]);
  display_printf(0, 3 * OLED_FONT_HEIGHT, "%02d: %s", screen * 4 + 2, mnemonic[screen * 4 + 1]);
  display_printf(0, 4 * OLED_FONT_HEIGHT, "%02d: %s", screen * 4 + 3, mnemonic[screen * 4 + 2]);
  display_printf(0, 5 * OLED_FONT_HEIGHT, "%02d: %s", screen * 4 + 4, mnemonic[screen * 4 + 3]);

  display.draw_string(0, OLED_BOTTOM_LINE, "<- prev       next ->");

  display.display();
}

void display_draw_mnemonic_screen(const char **mnemonic, unsigned int entropy_bits) {
  int screen = 0, prev_screen = -1;

  while (true) {
    if (screen >= number_of_words_from_entropy_size(entropy_bits) / 4) {
      break;
    }

    if (screen != prev_screen) {
      display_draw_mnemonic(mnemonic, entropy_bits, screen);
      prev_screen = screen;
    }

    switch (button_pressed()) {
    case BUTTON_LEFT_PRESS:
      delay(200);
      screen > 0 && screen--;
      break;
    case BUTTON_RIGHT_PRESS:
      delay(200);
      screen++;
      break;
    default:
      delay(10);
      break;
    }
  }
}
