#include <stdio.h>
#include "debug.h"
#include "config.h"
#include "request.h"
#include "response.h"
#include "test_config.h"


static json_parse_rule account_meta[] = {

    {
        "address", IOTEX_RES_TYPE_STR
    },

    {
        "balance", IOTEX_RES_TYPE_INTEGER,
    },

    {
        "nonce", IOTEX_RES_TYPE_INTEGER
    },

    {
        "pendingNonce", IOTEX_RES_TYPE_INTEGER
    },

    {
        "numActions", IOTEX_RES_TYPE_INTEGER
    },
};


static json_parse_rule account_rules[] = {

    {
        "accountMeta", IOTEX_RES_TYPE_STRUCT, account_meta
    },

    {
        NULL
    },
};


/* json.c */
int json_parse_response(const char *response, json_parse_rule *rules);


int main(int argc, char **argv) {

    char response[1024];
    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), IotexReqGetAccount, TEST_ACCOUNT_ADDR)) {

        __WARN_MSG__("compose url failed!");
        return -1;
    }

    if (req_send_request(url, response, sizeof(response)) != 0) {

        __WARN_MSG__("send request failed!");
        return -1;
    }

    if (json_parse_response(response, account_rules) != 0) {

        fprintf(stdout, "[%zu], %s\n", strlen(response), response);
    }

    return 0;
}
