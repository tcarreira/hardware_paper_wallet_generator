#include <Arduino.h>
#include <oled.h>
#include <string.h>

// #include "bcl/src/Sha512.hpp"
// #include "bcl/test/TestHelper.hpp"

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
#define BUTTON_LEFT_PRESS 1 << 0
#define BUTTON_RIGHT_PRESS 1 << 1

#define ENTROPY_BITS 256 // 128 (12words), 160 (15w), 192 (18w), 224 (21w), 256 (24w)
#define ENTROPY_BYTES (ENTROPY_BITS / 8)

#define OLED_BOTTOM_LINE (7 * OLED_FONT_HEIGHT)

// There are 2 different versions of the board
// OLED display=OLED(2,14,4);
// OLED display=OLED(4,5,16);
// OLED display=OLED(5,4,16); // working!!!! TTGO T1
// OLED display=OLED(5,4); // working!!!! TTGO T1
OLED display = OLED(PIN_I2C_SDA, PIN_I2C_SCL, PIN_I2C_RESET, OLED::W_128, OLED::H_64, OLED::CTRL_SSD1306, I2C_ADDRESS);

void main_setup() {
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

void main_loop() {
  display.clear();
  display.draw_string(0, 2 * OLED_FONT_HEIGHT, "       Bitcoin       ");
  display.draw_string(0, 3 * OLED_FONT_HEIGHT, "     Paper Wallet    ");
  display.draw_string(0, 4 * OLED_FONT_HEIGHT, "      Generator      ");
  display.draw_string(0, 1 * OLED_BOTTOM_LINE, "2023-05-26 |  23-5-26");
  display.display();
  delay(1500);

  /*****************************************************************************
  // GENERATE ENTROPY
  *****************************************************************************/
  // generate Random entropy (wip: not crypto safe)
  unsigned char entropy[ENTROPY_BYTES];
  memset(entropy, 0, ENTROPY_BYTES);

  display_choose_entropy_mode();
  if (wait_for_button(BUTTON_LEFT_PRESS, BUTTON_RIGHT_PRESS)) { // Fast, but unsafe
    generate_entropy_unsafe(entropy, ENTROPY_BYTES);
    display_draw_entropy(entropy, ENTROPY_BYTES, "Random Seed (UNSAFE):");
  } else { // Slow. Safe mode. Recommended for production.
    char buf[40]; sprintf(buf, "generating (0/%d):", ENTROPY_BITS);
    display_draw_entropy(entropy, ENTROPY_BYTES, buf);
    generate_entropy_manually(entropy, ENTROPY_BYTES);
    display_draw_entropy(entropy, ENTROPY_BYTES, "Random Seed:");
  }
  display.draw_string(0, OLED_BOTTOM_LINE, "        show words ->");
  display.display();

  // wait for right button to continue
  while ( ! (button_pressed() & BUTTON_RIGHT_PRESS) ) {
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

  /*****************************************************************************
  // Derive HMAC-SHA512
  *****************************************************************************/
  // Bytes gotHash[bcl::Sha512::HASH_LEN];
  // Bytes key = hexBytes("0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B");
  // Bytes message = asciiBytes("Hi There");
  // bcl::Sha512::getHmac(entropy, ENTROPY_BYTES, message.data(), message.size(), gotHash);

  /*****************************************************************************
  // End - Thank you
  *****************************************************************************/
  display.clear();
  display.draw_string(16, 10, "Thank   ", OLED::DOUBLE_SIZE);
  display.draw_string(16, 30, "    You!", OLED::DOUBLE_SIZE);
  display.display();
  delay(2000);

  display.clear();
}

unsigned int button_pressed() {
  unsigned int pressed = 0;
  if (digitalRead(LEFT_BUTTON_PIN) == LOW) {
    pressed |= BUTTON_LEFT_PRESS;
  }
  if (digitalRead(RIGHT_BUTTON_PIN) == LOW) {
    pressed |= BUTTON_RIGHT_PRESS;
  }
  return pressed;
}

void display_printf(unsigned int x, unsigned int y, const char *format, ...) {
  char buffer[128];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  display.draw_string(x, y, buffer);
}

void display_choose_entropy_mode() {
  display.clear();
  display.draw_string(0, 0 * OLED_FONT_HEIGHT, " Choose Entropy Mode ");
  display.draw_string(0, 1 * OLED_FONT_HEIGHT, "                     ");
  display.draw_string(0, 2 * OLED_FONT_HEIGHT, "Fast       TrueRandom");
  display.draw_string(0, 3 * OLED_FONT_HEIGHT, "(UNSAFE)       (slow)");
  display.draw_string(0, 4 * OLED_FONT_HEIGHT, " |                 | ");
  display.draw_string(0, 5 * OLED_FONT_HEIGHT, " |                 | ");
  display.draw_string(0, 6 * OLED_FONT_HEIGHT, " |                 | ");
  display.draw_string(0, 7 * OLED_FONT_HEIGHT, " v (L)         (R) v ");
  display.draw_line(1 * OLED_FONT_WIDTH, 2 * OLED_FONT_HEIGHT + 2, 2 * OLED_FONT_WIDTH, 2 * OLED_FONT_HEIGHT + 2);
  display.display();
}

void generate_entropy_unsafe(unsigned char *entropy, size_t len) {
  for (size_t i = 0; i < len; i++) {
    entropy[i] = random(0, 256);
  }
}

void generate_entropy_manually(unsigned char *entropy, size_t len) {
  unsigned int state = button_pressed();
  unsigned char byte = 0;
  int i = 0;
  while (i < len * 8) {
    delay(2);
    int rand = random(1024);
    unsigned int b = button_pressed();
    if (b != state) {
      state = b;
      byte = (byte << 1) + rand % 2;
      i++;

      entropy[i / 8] = byte;

      char buf[50];
      sprintf(buf, "generating (%d/%d):", i, len * 8);
      display_draw_entropy(entropy, len, buf);
    }
  }
}

void display_draw_entropy(const unsigned char *entropy, size_t len, const char *header) {
  display.clear();
  display.draw_string(0, 0, header);

  // print left column index
  for (int i = 0; i < len; i += 8) {
    display_printf(0 * OLED_FONT_WIDTH, (i / 8 + 2) * OLED_FONT_HEIGHT, "%02d:", i);
  }
  // print bytes
  for (int j = 0; j < len; j++) {
    display_printf(((2 * (j % 8)) + 4) * OLED_FONT_WIDTH, (j / 8 + 2) * OLED_FONT_HEIGHT, "%02x", entropy[j]);
  }

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

    unsigned int pressed = button_pressed();
    if (pressed & BUTTON_LEFT_PRESS) {
      delay(200);
      screen > 0 && screen--;
    } else if (pressed & BUTTON_RIGHT_PRESS) {
      delay(200);
      screen++;
    } else {
      delay(10);
    }
  }
}

bool wait_for_button(int aTrue, int bFalse) {
  while (true) {
    int pressed = button_pressed();
    if (pressed == aTrue) {
      return true;
    }
    if (pressed == bFalse) {
      return false;
    }
    delay(10);
  }
}

int main_number = 2; // 0 = main, 1 = qr_code, 2 = secp256k1

void setup() {
  switch (main_number){
  case 0:
    main_setup();
    break;
  case 1:
    qr_code_setup();
    break;
  case 2:
    secp256k1_setup();
  default:
    main_setup();
  }
}
void loop() {

  switch (main_number){
  case 0:
    main_loop();
    break;
  case 1:
    qr_code_loop();
    break;
  case 2:
    secp256k1_loop();
  default:
    main_loop();
  }
}
