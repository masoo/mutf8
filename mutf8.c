#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "mutf8.h"

/**
 * @private
 * @var uint8_t m_utf8_jump_table[256]
 * @author Atsushi Enomoto  <atsushi@ximian.com>
 * @sa https://github.com/corngood/mono/blob/master/eglib/src/gutf8.c
 * 
 * @note
 *  base var is "const guchar g_utf8_jump_table[256]".
 */
/*
 * Index into the table below with the first byte of a UTF-8 sequence to get
 * the number of bytes that are supposed to follow it to complete the sequence.
 *
 * Note that *legal* UTF-8 values can't have 5-bytes or 6-bytes. The table is left
 * as-is for anyone who may want to do such conversion, which was allowed in
 * earlier algorithms.
*/
static const uint8_t m_utf8_jump_table[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

/**
 * @public
 * @fn int64_t m_utf8_ch_byte_size(const m_char8_t *character)
 * @brief utf8 character byte size
 * @param[in] character - utf8 character
 * @return utf8 character byte size
 * @note
 *   do not use null-terminated string.
 *   that *legal* UTF-8 values can't have 5-bytes or 6-bytes.
 */
int64_t m_utf8_ch_byte_size(const m_char8_t *character)
{
    return m_utf8_jump_table[(uint8_t)(*character)];
}

/**
 * @public
 * @fn bool m_utf8_ch_validate(const m_char8_t *character, size_t character_bytesize)
 * @brief utf8 character validate
 * @params[in] character - utf8 character
 * @params[in] character_bytesize - utf8 character byte size
 * @return true valid, false invalid
 * @author Atsushi Enomoto  <atsushi@ximian.com>
 * @sa https://github.com/corngood/mono/blob/master/eglib/src/gutf8.c
 * @note
 *   do not use null-terminated string.
 *   base function is "static gboolean utf8_validate (const unsigned char *inptr, size_t len)".
 */
bool m_utf8_ch_validate(const m_char8_t *character, size_t character_size)
{
    uint8_t *inptr = (uint8_t *)character;
    const uint8_t *ptr = inptr + character_size;
    uint8_t c;

    /* Everything falls through when TRUE... */
    switch (character_size)
    {
    default:
        return false;
    case 4:
        if ((c = (*--ptr)) < 0x80 || c > 0xBF)
            return false;

        if ((c == 0xBF || c == 0xBE) && ptr[-1] == 0xBF)
        {
            if (ptr[-2] == 0x8F || ptr[-2] == 0x9F ||
                ptr[-2] == 0xAF || ptr[-2] == 0xBF)
                return false;
        }
        /* fall through */
    case 3:
        if ((c = (*--ptr)) < 0x80 || c > 0xBF)
            return false;
        /* fall through */
    case 2:
        if ((c = (*--ptr)) < 0x80 || c > 0xBF)
            return false;

        /* no fall-through in this inner switch */
        switch (*inptr)
        {
        case 0xE0:
            if (c < 0xA0)
                return false;
            break;
        case 0xED:
            if (c > 0x9F)
                return false;
            break;
        case 0xEF:
            if (c == 0xB7 && (ptr[1] > 0x8F && ptr[1] < 0xB0))
                return false;
            if (c == 0xBF && (ptr[1] == 0xBE || ptr[1] == 0xBF))
                return false;
            break;
        case 0xF0:
            if (c < 0x90)
                return false;
            break;
        case 0xF4:
            if (c > 0x8F)
                return false;
            break;
        default:
            if (c < 0x80)
                return false;
            break;
        }
    case 1:
        if (*inptr >= 0x80 && *inptr < 0xC2)
            return false;
    }

    if (*inptr > 0xF4)
        return false;

    return true;
}

/**
 * @public
 * @fn int64_t m_utf8_str_byte_size(const m_char8_t *str, size_t max_str_bytesize)
 * @brief utf8 string byte size
 * @params[in] str - utf8 string
 * @params[in] max_str_bytesize - utf8 string byte size( add null-terminated string size)
 * @return string byte size( add null-terminated string size)
 * @author FUNABARA Masao
 * @note
 *   add null-terminated string size.
 */
int64_t m_utf8_str_byte_size(const m_char8_t *str, size_t max_str_bytesize)
{
    const uint8_t *inptr = (uint8_t *)str;
    const int64_t max_size = (int64_t)max_str_bytesize;
    int64_t str_size = 0;
    int size = 0;

    while (*inptr && str_size < max_size)
    {
        size = m_utf8_jump_table[*inptr];
        str_size += size;
        inptr += size;
    }

    if (str_size >= max_size)
    {
        str_size -= size;
    }
    if (max_size > 0)
    {
        str_size += 1;
    }
    return str_size;
}
