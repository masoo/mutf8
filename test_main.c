#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <locale.h>
#include "mutf8.h"

int main(void)
{
    setlocale(LC_ALL, "C");

    // test m_utf8_ch_byte_size
    {
        m_char8_t *str1 = u8"a";
        assert(m_utf8_ch_byte_size(str1) == 1);

        m_char8_t *str2 = u8"あ";
        assert(m_utf8_ch_byte_size(str2) == 3);

        m_char8_t *str3 = u8"🚀";
        assert(m_utf8_ch_byte_size(str3) == 4);
        assert(m_utf8_ch_byte_size(u8"🚀") == 4); // This usage is also ok.
    }

    // test m_utf8_ch_validate
    {
        m_char8_t *str1 = u8"a";
        uint8_t size_str1 = m_utf8_ch_byte_size(str1);
        assert(m_utf8_ch_validate(str1, size_str1) == true);

        m_char8_t *str2 = u8"あ";
        uint8_t size_str2 = m_utf8_ch_byte_size(str2);
        assert(m_utf8_ch_validate(str2, size_str2) == true);

        m_char8_t *str3 = u8"🚀";
        uint8_t size_str3 = m_utf8_ch_byte_size(str3);
        assert(m_utf8_ch_validate(str3, size_str3) == true);

        // url http://php.net/manual/en/reference.pcre.pattern.modifiers.php#54805
        assert(m_utf8_ch_validate(u8"a", 1) == true);                         //'Valid ASCII'
        assert(m_utf8_ch_validate(u8"\xc3\xb1", 2) == true);                  //'Valid 2 Octet Sequence'
        assert(m_utf8_ch_validate(u8"\xc3\x28", 2) == false);                 //'Invalid 2 Octet Sequence'
        assert(m_utf8_ch_validate(u8"\xa0\xa1", 2) == false);                 //'Invalid Sequence Identifier'
        assert(m_utf8_ch_validate(u8"\xe2\x82\xa1", 3) == true);              //'Valid 3 Octet Sequence'
        assert(m_utf8_ch_validate(u8"\xe2\x28\xa1", 3) == false);             //'Invalid 3 Octet Sequence (in 2nd Octet)'
        assert(m_utf8_ch_validate(u8"\xe2\x82\x28", 3) == false);             //'Invalid 3 Octet Sequence (in 3rd Octet)'
        assert(m_utf8_ch_validate(u8"\xf0\x90\x8c\xbc", 4) == true);          //'Valid 4 Octet Sequence'
        assert(m_utf8_ch_validate(u8"\xf0\x28\x8c\xbc", 4) == false);         //'Invalid 4 Octet Sequence (in 2nd Octet)'
        assert(m_utf8_ch_validate(u8"\xf0\x90\x28\xbc", 4) == false);         //'Invalid 4 Octet Sequence (in 3rd Octet)'
        assert(m_utf8_ch_validate(u8"\xf0\x28\x8c\x28", 4) == false);         //'Invalid 4 Octet Sequence (in 4th Octet)'
        assert(m_utf8_ch_validate(u8"\xf8\xa1\xa1\xa1\xa1", 5) == false);     //'Invalid 5 Octet Sequence'
        assert(m_utf8_ch_validate(u8"\xfc\xa1\xa1\xa1\xa1\xa1", 6) == false); //'Invalid 6 Octet Sequence'
    }

    // test m_utf8_to_unicode
    {
        m_char8_t *str1 = u8"a";
        assert(m_utf8_to_unicode(str1) == 0x00061);
        m_char8_t *str2 = u8"©";
        assert(m_utf8_to_unicode(str2) == 0x000A9);
        m_char8_t *str3 = u8"あ";
        assert(m_utf8_to_unicode(str3) == 0x03042);
        m_char8_t *str4 = u8"🚀";
        assert(m_utf8_to_unicode(str4) == 0x1F680);
    }

    // test m_utf8_display_width
    {
        setlocale(LC_ALL, "ja_JP.UTF-8");
        m_char8_t *str1 = u8"a";
        assert(m_utf8_display_width(str1) == 1);
        m_char8_t *str2 = u8"©";
        assert(m_utf8_display_width(str2) == 1);
        m_char8_t *str3 = u8"あ";
        assert(m_utf8_display_width(str3) == 2);
        m_char8_t *str4 = u8"🚀";
        assert(m_utf8_display_width(str4) == 2);
    }

    // test m_utf8_str_byte_size
    {
        assert(m_utf8_str_byte_size(u8"a", 100) == 2);
        assert(m_utf8_str_byte_size(u8"a", 2) == 2);
        assert(m_utf8_str_byte_size(u8"a", 1) == 1);
        assert(m_utf8_str_byte_size(u8"a", 0) == 0);
        assert(m_utf8_str_byte_size(u8"あ", 4) == 4);
        assert(m_utf8_str_byte_size(u8"あ", 3) == 1);
        assert(m_utf8_str_byte_size(u8"あ", 2) == 1);
        assert(m_utf8_str_byte_size(u8"あ", 1) == 1);
        assert(m_utf8_str_byte_size(u8"🚀", 5) == 5);
        assert(m_utf8_str_byte_size(u8"🚀", 4) == 1);
        assert(m_utf8_str_byte_size(u8"🚀", 3) == 1);
        assert(m_utf8_str_byte_size(u8"🚀", 2) == 1);
        assert(m_utf8_str_byte_size(u8"🚀", 1) == 1);
        assert(m_utf8_str_byte_size(u8"🚀", 0) == 0);
        assert(m_utf8_str_byte_size(u8"aa", 3) == 3);
        assert(m_utf8_str_byte_size(u8"aa", 2) == 2);
        assert(m_utf8_str_byte_size(u8"aa", 1) == 1);
        assert(m_utf8_str_byte_size(u8"aa", 0) == 0);
        assert(m_utf8_str_byte_size(u8"aあ", 5) == 5);
        assert(m_utf8_str_byte_size(u8"aあ", 4) == 2);
        assert(m_utf8_str_byte_size(u8"aあ", 3) == 2);
        assert(m_utf8_str_byte_size(u8"aあ", 2) == 2);
        assert(m_utf8_str_byte_size(u8"aあ", 1) == 1);
        assert(m_utf8_str_byte_size(u8"aあ", 0) == 0);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 9) == 9);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 8) == 6);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 7) == 6);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 6) == 6);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 5) == 5);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 4) == 1);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 3) == 1);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 2) == 1);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 1) == 1);
        assert(m_utf8_str_byte_size(u8"🚀aあ", 0) == 0);
    }

    // test m_utf8_str_validate
    {
        m_char8_t *str1 = u8"🚀aあ";
        int size_str1 = m_utf8_str_byte_size(str1, 9);
        assert(m_utf8_str_validate(str1, size_str1) == true);
        assert(m_utf8_str_validate(str1, size_str1 - 1) == false);

        m_char8_t *str2 = u8"🚀aあ\xf0\x28\x8c\xbc";
        int size_str2 = m_utf8_str_byte_size(str2, 13);
        assert(m_utf8_str_validate(str2, size_str2) == false);
    }

    // test m_utf8_str_display_count
    {
        m_char8_t *str1 = u8"a";
        int size_str1 = m_utf8_str_byte_size(str1, 2);
        assert(m_utf8_str_display_count(str1, size_str1) == 1);
        m_char8_t *str2 = u8"🚀aあ";
        int size_str2 = m_utf8_str_byte_size(str2, 9);
        assert(m_utf8_str_display_count(str2, size_str2) == 3);
        m_char8_t *str3 = "\u30DB\u309A"; // combine string 'ポ'
        int size_str3 = m_utf8_str_byte_size(str3, 9);
        assert(m_utf8_str_display_count(str3, size_str3) == 2);
    }

    return 0;
}