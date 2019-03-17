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

#include "base64.h"

#ifndef VARIANT
#define VARIANT_CHARS "+/"
#endif

static const char *std_s64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" VARIANT_CHARS;
static const char std_padding = '=';

//
size_t get_encoded_len( size_t de_len) {
    size_t r;
    r = de_len % 3;
    return de_len / 3 * 4 + (r ? 4 : 0);
}

size_t get_decoded_len( size_t en_len) {
    size_t n;
    n = en_len * 6;
    return n / 8 + (n % 8 ? 8 : 0);
}

// common methods
/*
    left_len = in_len % 3
    out_len = ( in_len - left_len + (left_len ? 3 : 0) ) * 8 / 6
*/
size_t _base64_encode(const unsigned char *in, size_t in_len, char *out, size_t out_len,
    const char *vrt_s64, const char cur_padding)
{
    size_t left_len;
    const unsigned char *i;
    char *o;
    const char *cur_s64;
    //
    if ( get_encoded_len(in_len) > out_len ) return 0;
    // to use cosnt, initialized const won't change
    if (strlen(vrt_s64) == 64) {
        cur_s64 = vrt_s64;
    }
    else{
        cur_s64 = std_s64;
    }
    //
    left_len = in_len;
    i = in;
    o = out;
    while (left_len > 2) {
        *o++ = cur_s64[  i[0] >> 2 ];
        *o++ = cur_s64[ ((i[0] & 0x03) << 4) | (i[1] >> 4) ];
        *o++ = cur_s64[ ((i[1] & 0x0f) << 2) | (i[2] >> 6) ];
        *o++ = cur_s64[  i[2] & 0x3f ];
        i += 3;
        left_len -= 3;
    }
    //
    if (left_len > 0) {
        *o++ = cur_s64[ i[0] >> 2 ];
        if (left_len > 1) {
            *o++ = cur_s64[ ((i[0] & 0x03) << 4) | (i[1] >> 4) ];
            *o++ = cur_s64[ ((i[1] & 0x0f) << 2) ];
            if (cur_padding != NULL) *o++ = cur_padding;
        }
        else {
            *o++ = cur_s64[ ((i[0] & 0x03) << 4) ];
            if (cur_padding != NULL) {
                *o++ = cur_padding;
                *o++ = cur_padding;
            }
        }
    }
    // append NULL
    *o = '\0';
    return (size_t)o - (size_t)out;
}

/*
    out_len = in_len * 6 / 8, more than exact.
    other char not in cur_s64 will be skipped!
*/
size_t _base64_decode(const char *in, size_t in_len, unsigned char *out, size_t out_len,
    const char *vrt_s64, const char cur_padding, int strict)
{
    int de_table[256];
    const unsigned char *i;
    char *o;
    int c, legal_counter;
    const char *cur_s64;
    //
    if ( out_len < get_decoded_len(in_len) ) return 0;
    // to use cosnt, initialized const won't change
    if (strlen(vrt_s64) == 64) {
        cur_s64 = vrt_s64;
    }
    else {
        cur_s64 = std_s64;
    }
    //
    legal_counter = 0;
    i = in;
    o = out;
    // update decode table
    for (c = 0; c < 256; c++) {
        if ( c == 9 || c == 10 || c == 13 || c == 32 ) {
            de_table[c] = -1;
        }
        else {
            de_table[c] = (int)(c ? strchr(cur_s64, c) : 0) - (int)cur_s64;
        }
    }
    if (cur_padding != NULL) de_table[cur_padding] = -1;
    //for (c = 0; c < 256; c++) printf("%d\t%d\n", c, de_table[c]);
    //
    while ( (size_t)i - (size_t)in < in_len ) {
        c = de_table[*i];
        if (c > -1) {
            switch ( legal_counter % 4 ) {
            case 0:
                *o = c << 2;
                break;
            case 1:
                *o |= c >> 4;
                o++;
                *o = c << 4;
                break;
            case 2:
                *o |= c >> 2;
                o++;
                *o = c << 6;
                break;
            case 3:
                *o |= c;
                o++;
                break;
            }
            //
            legal_counter++;
        }
        else if (strict && c < -1) {
            *out = '\0';
            return 0;
        }
        i++;
    }
    //
    if (strict && legal_counter % 4 != 0 && *o != 0) {
        *out = '\0';
        return 0;
    }
    // don't append NULL for binary data
    return (size_t)o - (size_t)out;
}

// standard base64
size_t _base64encode(const unsigned char *in, size_t in_len, char *out, size_t out_len)
{
    return _base64_encode(in, in_len, out, out_len, "", '=');
}

size_t _base64decode(const char *in, size_t in_len, unsigned char *out, size_t out_len)
{
    return _base64_decode(in, in_len, out, out_len, "", '=', 1);
}

// easy use
char *base64encode(const unsigned char *in, size_t in_len, size_t *out_len)
{
    char *out;
    *out_len = get_encoded_len(in_len);
    out = calloc(*out_len + 1, 1);
    *out_len = _base64encode(in, in_len, out, *out_len);
    return out;
}

unsigned char *base64decode(const char *in, size_t in_len, size_t *out_len)
{
    unsigned char *out;
    *out_len = get_decoded_len(in_len);
    out = calloc(*out_len + 1, 1);
    *out_len = _base64decode(in, in_len, out, *out_len);
    return out;
}
