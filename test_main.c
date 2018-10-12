#include <stdint.h>
#include <assert.h>
#include <locale.h>
#include "mutf8.h"

extern const uint8_t m_utf8_jump_table[];

int main(void)
{
    setlocale(LC_ALL, "");

    // test m_utf8_jump_table
    char *test_str1 = u8"a";
    int test_str1_size = m_utf8_jump_table[(uint8_t)(*test_str1)];
    assert(test_str1_size == 1);
    char *test_str2 = u8"あ";
    int test_str2_size = m_utf8_jump_table[(uint8_t)(*test_str2)];
    assert(test_str2_size == 3);
    char *test_str3 = u8"🚀";
    int test_str3_size = m_utf8_jump_table[(uint8_t)(*test_str3)];
    assert(test_str3_size == 4);

    // Don't insert value.
    // m_utf8_jump_table[0] = 3;
    return 0;
}