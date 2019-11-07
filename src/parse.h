#ifndef _IOTEX_EMB_PARSE_H_
#define _IOTEX_EMB_PARSE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>


typedef enum {
    JSON_TYPE_STR,
    JSON_TYPE_TIME,
    JSON_TYPE_ARRAY,
    JSON_TYPE_DOUBLE,
    JSON_TYPE_OBJECT,
    JSON_TYPE_NUMBER,
} json_datatype;


/* Descript json hierarchy and data type */
typedef struct _json_parse_rule {
    const char *key;
    json_datatype type;
    struct _json_parse_rule *sub;
    void *value;
    size_t value_len;
} json_parse_rule;


int json_parse_response(const char *response, json_parse_rule *rules);

#ifdef	__cplusplus
}
#endif

#endif /* _IOTEX_EMB_PARSE_H_ */
