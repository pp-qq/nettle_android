/* aes-meta.c */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "nettle-meta.h"

#include "aes.h"

const struct nettle_cipher nettle_aes256
= _BYOD_CIPHER_SAME(aes, AES, 256);