#include <stdio.h>
#include "debug.h"
#include "config.h"
#include "unittest.h"
#include "../src/request.h"
#include "../src/response.h"
#include "test_config.h"

char addr[128];
double chain_tps;
int64_t chain_height;

static json_parse_rule account_meta[] = {

    {
        "address", IOTEX_RES_TYPE_STR, NULL, addr, 128
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


static json_parse_rule epochs_rules[] = {

    {
        "num", IOTEX_RES_TYPE_INTEGER,
    },

    {
        "height", IOTEX_RES_TYPE_INTEGER, NULL, (void *) &chain_height
    },

    {
        "gravityChainStartHeight", IOTEX_RES_TYPE_INTEGER,
    },

    {
        NULL
    }
};


static json_parse_rule chain_meta_rule[] = {

    {
        "height", IOTEX_RES_TYPE_INTEGER,
    },

    {
        "numActions", IOTEX_RES_TYPE_INTEGER,
    },

    {
        "tps", IOTEX_RES_TYPE_INTEGER,
    },

    {
        "epoch", IOTEX_RES_TYPE_STRUCT, epochs_rules
    },

    {
        "tpsFloat", IOTEX_RES_TYPE_DOUBLE, NULL, (void *) &chain_tps
    },

    {
        NULL,
    }
};


/* json.c */
int json_parse_response(const char *response, json_parse_rule *rules);


int json_parse_chain_meta() {

    char response[1024];
    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), IotexReqGetChainMeta)) {

        __WARN_MSG__("compose url failed!");
        return -1;
    }

    if (req_send_request(url, response, sizeof(response)) != 0) {

        __WARN_MSG__("send request failed!");
        return -1;
    }

    if (json_parse_response(response, chain_meta_rule) != 0) {

        fprintf(stdout, "[%zu], %s\n", strlen(response), response);
        return -1;
    }
    else {

        fprintf(stdout, "Chain height: %ld, tps: %f\n", chain_height, chain_tps);
    }

    return 0;
}


int json_parse_account_meta() {

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
        return -1;
    }

    UNITTEST_ASSERT_STR_EQ(addr, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));
    UNITTEST_AUTO_PASS();
    return 0;
}


int main(int argc, char **argv) {

    json_parse_chain_meta();
    json_parse_account_meta();
    return 0;
}
