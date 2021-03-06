/* aes-set-encrypt-key.c
 *
 * Key setup for the aes/rijndael block cipher.
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include "aes-internal.h"
#include "macros.h"

void
aes_set_encrypt_key(struct aes_ctx *ctx,
		    size_t keysize, const uint8_t *key)
{
  static const uint8_t rcon[10] = {
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36,
  };
  unsigned nk, nr, i, lastkey;
  uint32_t temp;
  const uint8_t *rp;

  assert(keysize >= AES_MIN_KEY_SIZE);
  assert(keysize <= AES_MAX_KEY_SIZE);
  
  /* Truncate keysizes to the valid key sizes provided by Rijndael */
  if (keysize == 32) {
    nk = 8;
    nr = 14;
  } else if (keysize >= 24) {
    nk = 6;
    nr = 12;
  } else { /* must be 16 or more */
    nk = 4;
    nr = 10;
  }

  lastkey = (AES_BLOCK_SIZE/4) * (nr + 1);
  ctx->nrounds = nr;

  for (i=0, rp = rcon; i<nk; i++)
    ctx->keys[i] = LE_READ_UINT32(key + i*4);

  for (i=nk; i<lastkey; i++)
    {
      temp = ctx->keys[i-1];
      if (i % nk == 0)
	temp = SUBBYTE(ROTL32(24, temp), aes_sbox) ^ *rp++;

      else if (nk > 6 && (i%nk) == 4)
	temp = SUBBYTE(temp, aes_sbox);

      ctx->keys[i] = ctx->keys[i-nk] ^ temp;
    }
}
  
