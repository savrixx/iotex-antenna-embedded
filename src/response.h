#ifndef _IOTEX_EMB_RESPONSE_H_
#define _IOTEX_EMB_RESPONSE_H_

#include <stdint.h>

typedef enum {
    IOTEX_RES_TYPE_STR,
    IOTEX_RES_TYPE_BOOL,
    IOTEX_RES_TYPE_TIME,
    IOTEX_RES_TYPE_ARRAY,
    IOTEX_RES_TYPE_FLOAT,
    IOTEX_RES_TYPE_STRUCT,
    IOTEX_RES_TYPE_INTEGER,
} IotexHttpResponseDataType;


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
} json_parse_rule;

#endif /* _IOTEX_EMB_RESPONSE_H_ */
