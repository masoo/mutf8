#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <locale.h>
#include "mutf8.h"

int main(void)
{
    setlocale(LC_ALL, "");

    // test m_utf8_ch_byte_size
    m_char8_t *test_str1 = u8"a";
    int test_str1_size = m_utf8_ch_byte_size(test_str1);
    assert(test_str1_size == 1);
    m_char8_t *test_str2 = u8"あ";
    int test_str2_size = m_utf8_ch_byte_size(test_str2);
    assert(test_str2_size == 3);
    m_char8_t *test_str3 = u8"🚀";
    int test_str3_size = m_utf8_ch_byte_size(test_str3);
    assert(test_str3_size == 4);

    // test m_utf8_ch_validate
    assert(m_utf8_ch_validate(test_str1, test_str1_size) == true);
    assert(m_utf8_ch_validate(test_str2, test_str2_size) == true);
    assert(m_utf8_ch_validate(test_str3, test_str3_size) == true);

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
    return 0;
}