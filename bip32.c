// #include "bip32.h"
// #include "micro-ecc/uECC.h"
// #include <stdio.h>
// #include <string.h>

// const struct uECC_Curve_t *curve;

// uECC_Curve new_curve() {
//   return uECC_secp256k1();
// };

// void setup_curve() {
//   if (curve == NULL) {
//     curve = new_curve();
//   }
// }

// /* ===Master key generation===
// * OK - Generate a seed byte sequence S of a chosen length (between 128 and 512 bits; 256 bits is advised) from a (P)RNG.
// * Calculate I = HMAC-SHA512(Key = "Bitcoin seed", Data = S)
// * Split I into two 32-byte sequences, I<sub>L</sub> and I<sub>R</sub>.
// * Use parse<sub>256</sub>(I<sub>L</sub>) as master secret key, and I<sub>R</sub> as master chain code.
// In case parse<sub>256</sub>(I<sub>L</sub>) is 0 or parse<sub>256</sub>(I<sub>L</sub>) ≥ n, the master key is invalid.
// */

// /*
// secret_key and chain_code are malloc'd in this function and must be freed by the caller.
// */
// Generate_Master_private_key(uint8_t *seed, uint8_t key_size, uint8_t *secret_key, uint8_t *chain_code) {
//   secret_key = malloc(32 * sizeof(uint8_t));
//   chain_code = malloc(32 * sizeof(uint8_t));

//   uint8_t I[64];

//   HMAC_SHA512(seed, key_size, "Bitcoin seed", 12, I);
//   memcpy(secret_key, I, 32);
//   memcpy(chain_code, I + 32, 32);
// }

// #define SHA512_BLOCK_LENGTH 128
// #define SHA512_DIGEST_LENGTH 64

// typedef struct SHA512_CTX {
//   uint32_t state[8];
//   uint64_t bitcount;
//   uint8_t buffer[SHA512_BLOCK_LENGTH];
// } SHA512_CTX;

// extern void SHA512_Init(SHA512_CTX *ctx);
// extern void SHA512_Update(SHA512_CTX *ctx, const uint8_t *message, size_t message_size);
// extern void SHA512_Final(uint8_t digest[SHA512_DIGEST_LENGTH], SHA512_CTX *ctx);

// typedef struct SHA512_HashContext {
//   uECC_HashContext uECC;
//   SHA512_CTX ctx;
// } SHA512_HashContext;

// static void init_SHA512(const uECC_HashContext *base) {
//   SHA512_HashContext *context = (SHA512_HashContext *)base;
//   SHA512_Init(&context->ctx);
// }

// static void update_SHA512(const uECC_HashContext *base, const uint8_t *message, unsigned message_size) {
//   SHA512_HashContext *context = (SHA512_HashContext *)base;
//   SHA512_Update(&context->ctx, message, message_size);
// }

// static void finish_SHA512(const uECC_HashContext *base, uint8_t *hash_result) {
//   SHA512_HashContext *context = (SHA512_HashContext *)base;
//   SHA512_Final(hash_result, &context->ctx);
// }

// uint8_t HMAC_SHA512(uint8_t *key, uint8_t key_size, uint8_t *data, uint8_t data_size, uint8_t *out) {
//   setup_curve();

//   int i, c;
//   uint8_t private[32] = {0};
//   uint8_t public[64] = {0};
//   uint8_t hash[32] = {0};
//   uint8_t sig[64] = {0};

//   uint8_t tmp[2 * SHA512_DIGEST_LENGTH + SHA512_BLOCK_LENGTH];
//   SHA512_HashContext ctx = {{
//       &init_SHA512,         // (*init_hash)
//       &update_SHA512,       // (*update_hash)
//       &finish_SHA512,       // (*finish_hash)
//       SHA512_BLOCK_LENGTH,  // block_size
//       SHA512_DIGEST_LENGTH, // result_size
//       tmp                   // /* Must point to a buffer of at least (2 * result_size + block_size) bytes. */
//   }};

//   HMAC_init(ctx, key);
// }
