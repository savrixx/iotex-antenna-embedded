#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "unittest.h"
#include "../src/debug.h"
#include "../src/request.h"
#include "test_config.h"


#define MAX_RESPONSE_LEN (16 * 1024)
static char response_buf[MAX_RESPONSE_LEN];

int basic_request(IotexHttpRequests req, const char *args) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), req, args)) {

        return -1;
    }

    __INFO_MSG__(url);

    if (req_send_request(url, response_buf, sizeof(response_buf)) != 0) {

        return -1;
    }

    __INFO_MSG__(response_buf);
    return 0;
}


void test_get_account_info(const char *addr) {

    UNITTEST_AUTO_TRUE(basic_request(IotexReqGetAccount, addr) == 0);
}


void test_get_chainmeta() {

    UNITTEST_AUTO_TRUE(basic_request(IotexReqGetChainMeta, NULL) == 0);
}

void test_get_transfers_by_block(const char *block) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), IotexReqGetTransfersByBlock, atoi(block))) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_send_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}

void test_get_actions_by_hash(const char *hash) {

    UNITTEST_AUTO_TRUE(basic_request(IotexReqGetActionsByHash, hash) == 0);
}

void test_get_actions_by_addr(const char *addr, uint32_t start, uint32_t count) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), IotexReqGetActionsByAddr, addr, start, count)) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_send_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {

    test_get_chainmeta();
    test_get_account_info(TEST_ACCOUNT_ADDR);
    test_get_actions_by_hash(TEST_ACTION_HASH);
    test_get_actions_by_addr(TEST_ACCOUNT_ADDR, 0, 2);
    test_get_transfers_by_block(TEST_TRANSFERS_BLOCK);
}
