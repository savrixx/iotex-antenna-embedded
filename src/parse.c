#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "u128.h"
#include "jsmn.h"
#include "debug.h"
#include "parse.h"


#define MIN(a, b) ((a) < (b) ? (a) : (b))
static int json_parse_array(const char *json, jsmntok_t *tok, int tok_count, json_parse_rule *rule, int array_size);


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }

    return -1;
}

#ifdef _DEBUG_JSON_PARSE_
static const char *jsmn_type_str(jsmntype_t type) {

    static const char *__g_jsmn_type_str[] = {"UNDEFINED", "OBJECT", "ARRAY", "STRING", "PRIMITIVE"};

    if (type >= 0 && type < sizeof(__g_jsmn_type_str) / sizeof(__g_jsmn_type_str[0])) {

        return __g_jsmn_type_str[type];
    }
    else {

        return "Unknown type";
    }
}

static const char *json_type_str(json_datatype type) {

    static const char *__g_json_type_str[] = {"Undefined", "String", "Time", "Array", "Double", "Object", "Number"};

    if (type >= 0 && type < sizeof(__g_json_type_str) / sizeof(__g_json_type_str[0])) {

        return __g_json_type_str[type];
    }
    else {

        return "Unknown type";
    }
}
#endif

/*
 * @brief: parse json object according to json_parse_rule and save data to struct
 * #json: raw json string to parse
 * #tok: json object token after call jsmn_parse
 * #tok_count: #tok count
 * #rules: an array to describe json hierarchy, ends with NULL
 * $return: successed return processed jsontok_t size, failed return -1
 *
 * TODO: retrun meaningful error code
 */
static int json_parse_object(const char *json, jsmntok_t *tok, int tok_count, json_parse_rule *rules) {

    assert(json != NULL);
    assert(tok != NULL);
    assert(rules != NULL);

    size_t i = 0;
    int processed_tok = 0, ret;
    json_parse_rule *rule = NULL;

    while (i < tok_count) {

        for (rule = rules; rule && rule->key; rule++) {

            if (jsoneq(json, tok, rule->key) != 0) {

                continue;
            }

#ifdef _DEBUG_JSON_PARSE_
            fprintf(stdout, "\t[%s, %s] %s: %.*s\n", \
                    jsmn_type_str(tok[1].type), json_type_str(rule->type), rule->key, tok[1].end - tok[1].start, json + tok[1].start);
#endif

            switch (rule->type) {
                /* TODO: Convert time string to struct tm */
                case JSON_TYPE_TIME:
                case JSON_TYPE_STR:
                    if (tok[1].type == JSMN_STRING && rule->value && rule->value_len) {

                        processed_tok += 2;
                        memset(rule->value, 0, rule->value_len);
                        memcpy(rule->value, json + tok[1].start, MIN(rule->value_len, tok[1].end - tok[1].start));
                    }

                    break;

                case JSON_TYPE_ARRAY:
                    if (tok[1].type == JSMN_ARRAY && rule->value && rule->value_len) {

                        if ((ret = json_parse_array(json, tok + 2, tok_count - i - 2, rule, tok[1].size)) < 0) {

                            return ret;
                        }
                        else {

                            i += ret + 1;
                            tok += ret + 1;
                            processed_tok += ret + 2;
                        }
                    }

                    break;

                case JSON_TYPE_DOUBLE:
                    if (tok[1].type == JSMN_PRIMITIVE && rule->value) {

                        processed_tok += 2;
                        char *value = strndup(json + tok[1].start, tok[1].end - tok[1].start);
                        *(double *)(rule->value) = atof(value);
                        free(value);
                    }

                    break;

                case JSON_TYPE_OBJECT:
                    if (tok[1].type == JSMN_OBJECT && rule->sub) {

#ifdef _DEBUG_JSON_PARSE_
                        fprintf(stdout, "Object[%s] size: %d\n", rule->key, tok[1].size);
#endif

                        if ((ret = json_parse_object(json, tok + 2, tok_count - i - 2, rule->sub)) < 0) {

                            return ret;
                        }
                        else {

                            i += ret + 1;
                            tok += ret + 1;
                            processed_tok += ret + 2;
                        }
                    }

                    break;

                case JSON_TYPE_NUMBER:
                    if ((tok[1].type == JSMN_STRING || tok[1].type == JSMN_PRIMITIVE) && rule->value) {

                        processed_tok += 2;
                        char *value = strndup(json + tok[1].start, tok[1].end - tok[1].start);
                        str2u128(value, (uint128_t *)(rule->value));
                        free(value);
                    }

                    break;

                default:
                    fprintf(stderr, "Key: %s, unknown data type[%d]!\n", rule->key, rule->type);
                    break;

            } /* end of switch */
        }

        i++;
        tok++;
    }

#ifdef _DEBUG_JSON_PARSE_
    fprintf(stdout, "Total: %d, processed: %d\n", tok_count, processed_tok);
#endif

    return processed_tok;
}


/*
 * @brief: parse json array
 * #json: raw json string to parse
 * #tok: json object token after call jsmn_parse
 * #tok_count: #tok count
 * #rule: json array parse rule(include array element type, element parse rule if it's an object, and bind element data to rule callback)
 * $return: successed return processed jsontok_t size, failed return -1
 */
static int json_parse_array(const char *json, jsmntok_t *tok, int tok_count, json_parse_rule *rule, int array_size) {

    int i, ret;
    int processed_tok = 0;

    assert(json != NULL);
    assert(tok != NULL);
    assert(rule != NULL);

#ifdef _DEBUG_JSON_PARSE_
    fprintf(stdout, "Array[%s] size: %d, type: %s, array buffer size: %zu, single element size: %zu\n", \
            rule->key, array_size, json_type_str(rule->array_element_type), rule->value_len, rule->single_array_element_size);
#endif

    for (i = 0; i < array_size; i++) {

        if (tok->type == JSMN_OBJECT && rule->array_element_type == JSON_TYPE_OBJECT) {

            /* Don't know how to parse */
            if (!rule->sub || !rule->array_element_bind || rule->single_array_element_size == 0) {

                return -1;
            }

            /* Bind array element data to parse rule */
            if (rule->array_element_bind(rule->sub, rule->value + i * rule->single_array_element_size) != 0) {

                return -1;
            }

            /* Parse array object data */
            if ((ret = json_parse_object(json, tok, tok_count, rule->sub)) == -1) {

                return -1;
            }

            tok_count -= ret;
            processed_tok += ret;
        }
        else {

            /* TODO: handle other type data */
        }
    }

    /* Add array itself */
    return processed_tok + 1;
}


/*
 * @brief: parse response json string
 * #response: https response json string
 * #rule: response json hierarchy, ends with NULL
 * $return: successed return 0, failed return -1
 *
 * TODO: retrun meaningful error code
 */
int json_parse_response(const char *response, json_parse_rule *rules) {

    int tok_total;
    int processed_tok;
    jsmn_parser parser;
    jsmntok_t token[128];

    jsmn_init(&parser);

    if ((tok_total = jsmn_parse(&parser,
                                response, strlen(response),
                                token, sizeof(token) / sizeof(token[0]))) < 0) {

        fprintf(stderr, "Json parse response failed: %d\n", tok_total);
        return -1;
    }

    if (tok_total < 1 || token[0].type != JSMN_OBJECT) {

        fprintf(stderr, "Json parse failed: object expected\n");
        return -1;
    }

    if ((processed_tok = json_parse_object(response, token, tok_total - 1, rules)) != tok_total - 1) {

        fprintf(stderr, "Json parse failed, total token: %d, processed token: %d\n", tok_total - 1, processed_tok);
        return -1;
    }

    return 0;
}
