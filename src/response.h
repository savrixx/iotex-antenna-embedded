#ifndef _IOTEX_EMB_RESPONSE_H_
#define _IOTEX_EMB_RESPONSE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef enum {
    IOTEX_RES_TYPE_STR,
    IOTEX_RES_TYPE_BOOL,
    IOTEX_RES_TYPE_TIME,
    IOTEX_RES_TYPE_ARRAY,
    IOTEX_RES_TYPE_FLOAT,
    IOTEX_RES_TYPE_STRUCT,
    IOTEX_RES_TYPE_INTEGER,
} IotexHttpResponseDataType;


/* Descript json hierarchy and data type */
typedef struct _json_parse_rule {
    const char *key;
    IotexHttpResponseDataType type;
    struct _json_parse_rule *sub;
    union {
        void *s_value;
        char *c_value;
        float *f_value;
        uint64_t *u_value;
    } value;
    size_t value_len;
} json_parse_rule;


#ifdef	__cplusplus
}
#endif

#endif /* _IOTEX_EMB_RESPONSE_H_ */
