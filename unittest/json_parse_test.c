#include <stdio.h>
#include "debug.h"
#include "config.h"
#include "unittest.h"
#include "test_config.h"
#include "../src/parse.h"
#include "../src/request.h"

char addr[128];
double chain_tps;
int64_t chain_height;

static json_parse_rule account_meta[] = {

    {
        "address", JSON_TYPE_STR, NULL, addr, 128
    },

    {
        "balance", JSON_TYPE_INT32,
    },

    {
        "nonce", JSON_TYPE_INT32
    },

    {
        "pendingNonce", JSON_TYPE_INT32
    },

    {
        "numActions", JSON_TYPE_INT32
    },
};


static json_parse_rule account_rules[] = {

    {
        "accountMeta", JSON_TYPE_OBJECT, account_meta
    },

    {
        NULL
    },
};


static json_parse_rule epochs_rules[] = {

    {
        "num", JSON_TYPE_INT32,
    },

    {
        "height", JSON_TYPE_INT32, NULL, (void *) &chain_height
    },

    {
        "gravityChainStartHeight", JSON_TYPE_INT32,
    },

    {
        NULL
    }
};


static json_parse_rule chain_meta_rule[] = {

    {
        "height", JSON_TYPE_INT32,
    },

    {
        "numActions", JSON_TYPE_INT32,
    },

    {
        "tps", JSON_TYPE_INT32,
    },

    {
        "epoch", JSON_TYPE_OBJECT, epochs_rules
    },

    {
        "tpsFloat", JSON_TYPE_DOUBLE, NULL, (void *) &chain_tps
    },

    {
        NULL,
    }
};



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
