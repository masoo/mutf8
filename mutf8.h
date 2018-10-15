#ifndef MUTF8_H
#define MUTF8_H

typedef char m_char8_t;
extern int64_t m_utf8_ch_byte_size(const m_char8_t *character);
extern bool m_utf8_ch_validate(const m_char8_t *character, size_t character_size);

#endif /* end MUTF_8 */
