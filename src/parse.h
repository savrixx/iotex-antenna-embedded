#ifndef _IOTEX_EMB_PARSE_H_
#define _IOTEX_EMB_PARSE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>


typedef enum {
    JONN_TYPE_UNDEFINED,
    JSON_TYPE_STR,
    JSON_TYPE_TIME,
    JSON_TYPE_ARRAY,
    JSON_TYPE_DOUBLE,
    JSON_TYPE_OBJECT,
    JSON_TYPE_NUMBER,
} json_datatype;


/* Describe json hierarchy and data type */
typedef struct json_parse_rule {
    /* json key */
    const char *key;

    /* json value type */
    json_datatype type;

    /* sub object parse rule for JSON_TYPE_OBJECT or JSON_TYPE_OBJECT array */
    struct json_parse_rule *sub;

    /* pointed to char[]/double/uint128_t or array to save json value */
    void *value;

    /* for JSON_TYPE_STR means string buffer max size, for JSON_TYPE_ARRAY means array max size */
    size_t value_len;

    /* for JSON_TYPE_ARRAY type use, indicate array element type */
    json_datatype array_element_type;

    /* for JSON_TYPE_ARRAY type use, indicate single array element size in bytes */
    size_t single_array_element_size;

    /* for JSON_TYPE_OBJECT array use, bind parse rule and array element */
    int (*array_element_bind)(struct json_parse_rule *rule, void *data);
} json_parse_rule;


/*
 * parse response
 * in: raw response data(json string) and parse rule
 * out: save json value to struct according to json_parse_rule
 * successed retrun 0, failed retrun -1
 */
int json_parse_response(const char *response, json_parse_rule *rules);

#ifdef	__cplusplus
}
#endif

#endif /* _IOTEX_EMB_PARSE_H_ */
