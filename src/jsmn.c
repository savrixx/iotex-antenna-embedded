#include "jsmn.h"
#include "debug.h"
#include "response.h"


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
                fprintf(stdout, "%s: %.*s\n", rule->key, tok[1].end - tok[1].start, json + tok[1].start);
#endif

                switch (rule->type) {
                    case IOTEX_RES_TYPE_STR:
                        if (tok[1].type == JSMN_STRING && rule->value.c_value && rule->value_len) {

                        }

                        break;

                    case IOTEX_RES_TYPE_BOOL:
                        break;

                    case IOTEX_RES_TYPE_TIME:
                        break;

                    case IOTEX_RES_TYPE_ARRAY:
                        break;

                    case IOTEX_RES_TYPE_FLOAT:
                        break;

                    case IOTEX_RES_TYPE_STRUCT:
                        if (tok[1].type == JSMN_OBJECT && rule->sub) {

                            return json_parse_object(json, tok + 2, tok_count - i - 2, rule->sub);
                        }

                        break;

                    case IOTEX_RES_TYPE_INTEGER:
                        break;

                    default:
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
