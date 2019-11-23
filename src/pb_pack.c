#include <string.h>
#include <endian.h>
#include "pb_pack.h"


int pb_pack_varint(const pb_st_item *item, uint8_t *buffer) {

    uint8_t *key = buffer;
    uint8_t *value = buffer + 1;
    uint64_t varint = *((uint64_t *)item->data);

    *key = PB_KEY(item->field, PB_WT_VARINT);

    do {

        *value = PB_VARINT_MORE | (varint & PB_VARINT_MASK);
        varint >>= 7;
        value++;

    } while (varint);

    *(--value) &= ~PB_VARINT_MORE;

    return value - buffer + 1;
}


int pb_pack_64bit(const pb_st_item *item, uint8_t *buffer) {

    uint8_t *key = buffer;
    uint8_t *value = buffer + 1;
    uint64_t fixed64 = htole64(*((uint64_t *)item->data));

    *key = PB_KEY(item->field, PB_WT_64);
    memcpy(value, &fixed64, sizeof(fixed64));

    return sizeof(fixed64) + 1;
}


int pb_pack_32bit(const pb_st_item *item, uint8_t *buffer) {

    uint8_t *key = buffer;
    uint8_t *value = buffer + 1;
    uint32_t fixed32 = htole32(*((uint32_t *)item->data));

    *key = PB_KEY(item->field, PB_WT_32);
    memcpy(value, &fixed32, sizeof(fixed32));

    return sizeof(fixed32) + 1;
}


int pb_pack_ld(const pb_st_item *item, uint8_t *buffer) {

    return 0;
}


int pb_pack(const pb_st_item *messages, size_t len, uint8_t *buffer, size_t size) {

    int i = 0;
    int packed_len = 0;
    PB_SERIALIZE_FUNC serialize = NULL;
    static const PB_SERIALIZE_FUNC serialize_funcs[] = {

        pb_pack_varint,
        pb_pack_64bit,
        pb_pack_ld,
        NULL,
        NULL,
        pb_pack_32bit
    };

    for (i = 0; i < len; i++, messages++) {

        if (messages->type > PB_MAX_TYPE) {

            return -PB_TYPE_ERR;
        }

        /* Skip deprecated type and optional field */
        if (!(serialize = serialize_funcs[messages->type]) || !messages->data) {

            continue;
        }

        packed_len += serialize(messages, buffer + packed_len);

        /* Buffer to short */
        if (packed_len >= size) {

            return -PB_BUFF_ERR;
        }
    }

    return packed_len;
}
