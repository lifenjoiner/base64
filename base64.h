/* ${filename} */
/*
Description: base64 encoder and decoder (lib)
Copyright (C) ${author}, All Rights Reserved.
License: ${license} || See License.txt for details.
Author:  YX Hao
Version: 201407
Updates: ${website}
Changes: ${log}
*/

#ifndef _BASE64_H_
#define _BASE64_H_

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
#ifndef VARIANT
#define VARIANT_CHARS "+/"
#endif

static const char *std_s64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" VARIANT_CHARS;
static const char std_padding = '=';
*/

//
size_t get_encoded_len( size_t de_len);
size_t get_decoded_len( size_t en_len);

// common methods
size_t _base64_encode(const unsigned char *in, size_t in_len, char *out, size_t out_len,
    const char *vrt_s64, const char cur_padding);
size_t _base64_decode(const char *in, size_t in_len, unsigned char *out, size_t out_len,
    const char *vrt_s64, const char cur_padding, int strict);

// standard base64
size_t _base64encode(const unsigned char *in, size_t in_len, char *out, size_t out_len);
size_t _base64decode(const char *in, size_t in_len, unsigned char *out, size_t out_len);

// only for string
char *base64encode(const unsigned char *in, size_t in_len, size_t *out_len);
unsigned char *base64decode(const char *in, size_t in_len, size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif