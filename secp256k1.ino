// #include <Arduino.h>
// #include <stdio.h>
// #include <string.h>
// #include "micro-ecc/uECC.h"

// const struct uECC_Curve_t *curve;

// static int RNG(uint8_t *dest, unsigned size) {
//   // Use the least-significant bits from the ADC for an unconnected pin (or connected to a source of
//   // random noise). This can take a long time to generate random data if the result of analogRead(0)
//   // doesn't change very frequently.
//   while (size) {
//     uint8_t val = 0;
//     for (unsigned i = 0; i < 8; ++i) {
//       int init = analogRead(0);
//       int count = 0;
//       while (analogRead(0) == init) {
// 	++count;
//       }

//       if (count == 0) {
// 	val = (val << 1) | (init & 0x01);
//       } else {
// 	val = (val << 1) | (count & 0x01);
//       }
//     }
//     *dest = val;
//     ++dest;
//     --size;
//   }
//   // NOTE: it would be a good idea to hash the resulting random data using SHA-256 or similar.
//   return 1;
// }

void secp256k1_setup() {
  // Serial.begin(115200);
  // Serial.print("Testing ecc\n");
  // uECC_set_rng(&RNG);
  // curve = uECC_secp256k1();
}

void secp256k1_loop() {
  // uint8_t private1[uECC_curve_private_key_size(curve) + 1];
  // uint8_t private2[uECC_curve_private_key_size(curve) + 1];

  // uint8_t public1[uECC_curve_public_key_size(curve)];
  // uint8_t public2[uECC_curve_public_key_size(curve)];

  // uint8_t secret1[uECC_curve_private_key_size(curve)];
  // uint8_t secret2[uECC_curve_private_key_size(curve)];

  // unsigned long a = millis();
  // uECC_make_key(public1, private1, curve);
  // // EccPoint_compute_public_key(_public, _private, curve)
  // unsigned long b = millis();
  // Serial.print("Made key 1 in ");
  // Serial.println(b - a);
  // a = millis();
  // print_bytes(public1, uECC_curve_public_key_size(curve));

  // uECC_make_key(public2, private2, curve);
  // b = millis();
  // Serial.print("Made key 2 in ");
  // Serial.println(b - a);
  // print_bytes(public2, uECC_curve_public_key_size(curve));

  // a = millis();
  // int r = uECC_shared_secret(public2, private1, secret1, curve);
  // b = millis();
  // Serial.print("Shared secret 1 in ");
  // Serial.println(b - a);
  // if (!r) {
  //   Serial.print("shared_secret() failed (1)\n");
  //   return;
  // }
  // print_bytes(secret1, uECC_curve_private_key_size(curve));

  // a = millis();
  // r = uECC_shared_secret(public1, private2, secret2, curve);
  // b = millis();
  // Serial.print("Shared secret 2 in ");
  // Serial.println(b - a);
  // if (!r) {
  //   Serial.print("shared_secret() failed (2)\n");
  //   return;
  // }
  // print_bytes(secret2, uECC_curve_private_key_size(curve));

  // if (memcmp(secret1, secret2, 20) != 0) {
  //   Serial.print("Shared secrets are not identical!\n");
  // } else {
  //   Serial.print("Shared secrets are identical\n");
  // }
  // Serial.print("\n\n\n");
  // delay(1000);
}

// void print_bytes(uint8_t* bytes, int len) {
//   for (int i = 0; i < len; i++) {
//     Serial.printf("%02x ", bytes[i]);
//   }
//   Serial.println();
// }
