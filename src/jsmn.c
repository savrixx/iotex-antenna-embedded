#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "debug.h"
#include "response.h"


#define MIN(a, b) ((a) < (b) ? (a) : (b))


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }

    return -1;
}


/*
 * @brief: parse json object according to json_parse_rule
 * #json: raw json string to parse
 * #tok: json object token after call jsmn_parse
 * #tok_count: #tok count
 * #rules: json_parse_rule descript json hierarchy, ends with NULL
 * $return: successed return 0, failed return -1
 */
static int json_parse_object(const char *json, jsmntok_t *tok, size_t tok_count, json_parse_rule *rules) {

    size_t i = 0;
    json_parse_rule *rule = NULL;

#ifdef _DEBUG_JSON_PARSE_
    fprintf(stdout, "Raw json: %s\n", json);
#endif

    while (i < tok_count) {

        for (rule = rules; rule && rule->key; rule++) {

            if (jsoneq(json, tok, rule->key) == 0) {


#ifdef _DEBUG_JSON_PARSE_
                fprintf(stdout, "R%d, T%d %s: %.*s\n", rule->type, tok[1].type, rule->key, tok[1].end - tok[1].start, json + tok[1].start);
#endif

                switch (rule->type) {
                    case IOTEX_RES_TYPE_STR:
                        if (tok[1].type == JSMN_STRING && rule->value && rule->value_len) {

                            memset(rule->value, 0, rule->value_len);
                            memcpy(rule->value, json + tok[1].start, MIN(rule->value_len, tok[1].end - tok[1].start));
                        }

                        break;

                    case IOTEX_RES_TYPE_TIME:
                        break;

                    case IOTEX_RES_TYPE_ARRAY:
                        if (tok[1].type == JSMN_ARRAY && rule->value && rule->value_len) {

                        }

                        break;

                    case IOTEX_RES_TYPE_DOUBLE:
                        if (tok[1].type == JSMN_PRIMITIVE && rule->value) {

                            char *value = strndup(json + tok[1].start, tok[1].end - tok[1].start);
                            *(double *)(rule->value) = atof(value);
                            free(value);
                        }

                        break;

                    case IOTEX_RES_TYPE_STRUCT:
                        if (tok[1].type == JSMN_OBJECT && rule->sub) {

                            if (json_parse_object(json, tok + 2, tok_count - i - 2, rule->sub) != 0) {

                                return -1;
                            }
                        }

                        break;

                    case IOTEX_RES_TYPE_INTEGER:
                        if ((tok[1].type == JSMN_STRING || tok[i].type == JSMN_PRIMITIVE) && rule->value) {

                            char *value = strndup(json + tok[1].start, tok[1].end - tok[1].start);
                            *(int64_t *)(rule->value) = atoll(value);
                            free(value);
                        }

                        break;

                    default:
                        fprintf(stderr, "Key: %s, unknown data type[%d]\n", rule->key, rule->type);
                        break;
                }
            }
        }

        i++;
        tok++;
    }

    return 0;
}


/*
 * @brief: parse response json string
 * #response: https response json string
 * #rule: response json hierarchy, ends with NULL
 * $return: successed return 0, failed return -1
 */
int json_parse_response(const char *response, json_parse_rule *rules) {

    int ret;
    jsmn_parser parser;
    jsmntok_t token[128];

    jsmn_init(&parser);

    if ((ret = jsmn_parse(&parser,
                          response, strlen(response),
                          token, sizeof(token) / sizeof(token[0]))) < 0) {

        fprintf(stderr, "Json parse response failed: %d\n", ret);
        return -1;
    }

    if (ret < 1 || token[0].type != JSMN_OBJECT) {

        fprintf(stderr, "Json parse failed: object expected\n");
        return -1;
    }

    return json_parse_object(response, token, ret - 1, rules);
}
