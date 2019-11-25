#include <stdlib.h>
#include "rule.h"
#include "debug.h"
#include "parse.h"
#include "config.h"
#include "request.h"
#include "response.h"
#include "iotex_emb.h"

/*
 * @brief: send request to http server and get response data then follow the rule parse json string to struct data
 * #request: http request should include base url and post data
 * #rule: json_parse_rule tell the parser how to parse response data
 * $return: successed return 0, (the data will saved to the rule binded struct), failed return negative value
 */
int res_get_data(const char *request, json_parse_rule *rules) {

    char *response = NULL;

    if ((response = malloc(IOTEX_EBM_MAX_RES_LEN)) == NULL) {

        return -1;
    }

#ifdef _DEBUG_HTTP_
    __INFO_MSG__(request);
#endif

    if (req_get_request(request, response, IOTEX_EBM_MAX_RES_LEN) != 0) {

        free(response);
        return -1;
    }

#ifdef _DEBUG_HTTP_
    __INFO_MSG__(response);
#endif

    if (json_parse_response(response, rules) != 0) {

        free(response);
        return -1;
    }

    free(response);
    return 0;
}


/*
 * @brief: get actions info list
 * #request: http request should include base url and post data
 * #actions: an array to save action
 * #max_size: #actions array size
 * $return: successed return actual get actions size, failed -1
 */
int res_get_actions(const char *request, struct iotex_st_action_info *actions, size_t max_size) {

    int ret;
    uint128_t actual_size = 0;

    json_parse_rule core_transfer_rule[] = {

        {"amount", JSON_TYPE_NUMBER},
        {"recipient", JSON_TYPE_STR},
        {NULL}
    };

    json_parse_rule action_core_rule[] = {

        {"nonce", JSON_TYPE_NUMBER},
        {"version", JSON_TYPE_NUMBER},
        {"gasLimit", JSON_TYPE_NUMBER},
        {"gasPrice", JSON_TYPE_NUMBER},
        {"transfer", JSON_TYPE_OBJECT, core_transfer_rule},
        {NULL}
    };

    json_parse_rule action_rule[] = {

        {"signature", JSON_TYPE_STR},
        {"senderPubKey", JSON_TYPE_STR},
        {"core", JSON_TYPE_OBJECT, action_core_rule},
        {NULL}
    };

    json_parse_rule action_info_rule[] = {

        {"action", JSON_TYPE_OBJECT, action_rule},
        {"actHash", JSON_TYPE_STR},
        {"blkHash", JSON_TYPE_STR},
        {"blkHeight", JSON_TYPE_NUMBER},
        {"sender", JSON_TYPE_STR},
        {"gasFee", JSON_TYPE_NUMBER},
        {"timestamp", JSON_TYPE_STR},
        {NULL}
    };

    json_parse_rule top_rule[] = {

        {"total", JSON_TYPE_NUMBER, NULL, (void *) &actual_size},
        /* key, json value type, array element parse rule, an array to save object, object array size, array element type, single object size, rule data bind callback */
        {"actionInfo", JSON_TYPE_ARRAY, action_info_rule, (void *)actions, max_size, JSON_TYPE_OBJECT, sizeof(iotex_st_action_info), rule_action_info_bind},
        {NULL}
    };

    if ((ret = res_get_data(request, top_rule)) != 0) {

        fprintf(stderr, "get actions failed: %d\n", ret);
        return ret;
    }

    return actual_size;
}
