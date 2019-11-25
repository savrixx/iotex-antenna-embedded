#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "unittest.h"
#include "test_config.h"
#include "../src/debug.h"
#include "../src/request.h"
#include "../src/iotex_emb.h"


#define MAX_RESPONSE_LEN (16 * 1024)
static char response_buf[MAX_RESPONSE_LEN];

int basic_request(iotex_em_request req, const char *args) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), req, args)) {

        return -1;
    }

    __INFO_MSG__(url);

    if (req_get_request(url, response_buf, sizeof(response_buf)) != 0) {

        return -1;
    }

    __INFO_MSG__(response_buf);
    return 0;
}


void test_get_account_info(const char *addr) {

    UNITTEST_AUTO_TRUE(basic_request(REQ_GET_ACCOUNT, addr) == 0);
}


void test_get_chainmeta() {

    UNITTEST_AUTO_TRUE(basic_request(REQ_GET_CHAINMETA, NULL) == 0);
}

void test_get_transfers_by_block(const char *block) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), REQ_GET_TRANSFERS_BY_BLOCK, block)) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_get_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}

void test_get_actions_by_hash(const char *hash) {

    UNITTEST_AUTO_TRUE(basic_request(REQ_GET_ACTIONS_BY_HASH, hash) == 0);
}

void test_get_actions_by_addr(const char *addr, uint32_t start, uint32_t count) {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), REQ_GET_ACTIONS_BY_ADDR, addr, start, count)) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_get_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}

void test_get_member_validators() {

    char url[IOTEX_EMB_MAX_URL_LEN];

    if (!req_compose_url(url, sizeof(url), REQ_GET_MEMBER_VALIDATORS)) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_get_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}

void test_act_transfer() {

    char url[IOTEX_EMB_MAX_URL_LEN];
    const char *signed_action_bytes = "0a4c0801107b18f8062203393939523e0a033435361"
                                      "229696f313837777a703038766e686a6a706b79646e"
                                      "723937716c68386b683064706b6b797466616d386a1"
                                      "a0c68656c6c6f20776f726c64211241044e18306ae9"
                                      "ef4ec9d07bf6e705442d4d1a75e6cdf750330ca2d88"
                                      "0f2cc54607c9c33deb9eae9c06e06e04fe9ce3d4396"
                                      "2cc67d5aa34fbeb71270d4bad3d648d91a41555cc8a"
                                      "f4181bf85c044c3201462eeeb95374f78aa48c67b87"
                                      "510ee63d5e502372e53082f03e9a11c1e351de539ce"
                                      "df85d8dff87de9d003cb9f92243541541a000";


    if (!req_compose_url(url, sizeof(url), REQ_SEND_SIGNED_ACTION_BYTES, signed_action_bytes)) {

        UNITTEST_FAIL("req_compose_url");
        return;
    }

    __INFO_MSG__(url);

    if (req_post_request(url, response_buf, sizeof(response_buf)) != 0) {

        UNITTEST_FAIL("req_send_request");
        return;
    }

    __INFO_MSG__(response_buf);
    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {

    iotex_emb_init(NULL);

    test_get_chainmeta();
    test_get_account_info(TEST_ACCOUNT_ADDR);
    test_get_actions_by_hash(TEST_ACTION_HASH);
    test_get_actions_by_addr(TEST_ACCOUNT_ADDR, 0, 2);
    test_get_transfers_by_block(TEST_TRANSFERS_BLOCK);
    test_get_member_validators();

    test_act_transfer();
}
