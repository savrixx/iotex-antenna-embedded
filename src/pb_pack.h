#ifndef _IOTEX_PB_PACK_H_
#define _IOTEX_PB_PACK_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef enum {
    PB_WT_VARINT = 0,
    PB_WT_64 = 1,
    PB_WT_LD = 2,
    PB_WT_32 = 5
} pb_em_wtype;

typedef enum {
    PB_NO_ERR,
    PB_TYPE_ERR,
    PB_BUFF_ERR
} pb_em_error;


/* NULL means it's an optional field */
typedef struct {
    pb_em_wtype type;
    uint8_t field;
    void *data;
} pb_st_item;

#define PB_MAX_TYPE (PB_WT_32)
#define PB_VARINT_MASK 0x7f
#define PB_VARINT_MORE 0x80
#define PB_KEY(field, type) (((field & 0xf) << 3) | ((type) & 0x7))


typedef int (*PB_SERIALIZE_FUNC)(const pb_st_item *item, uint8_t *buffer);


int pb_pack(const pb_st_item *messages, size_t len, uint8_t *buffer, size_t size);

#ifdef	__cplusplus
}
#endif


#endif /* _IOTEX_PB_PACK_H_ */
