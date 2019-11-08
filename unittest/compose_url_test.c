#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "unittest.h"
#include "test_config.h"
#include "../src/request.h"

static char url[IOTEX_EMB_MAX_URL_LEN];


void test_get_account() {

    const char *accounts[] = {

        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL
    };

    for (int i = 0; accounts[i]; i++) {

        UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_ACCOUNT, accounts[i]), NULL);
        UNITTEST_ASSERT_EQ(url[strlen(url) - 1], i + '0');
    }

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_ACCOUNT, TEST_ACCOUNT_ADDR), NULL);
    UNITTEST_ASSERT_STR_EQ(url + strlen(url) - strlen(TEST_ACCOUNT_ADDR), TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));

    UNITTEST_AUTO_PASS();
}


void test_get_chainmeta() {

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_CHAINMETA), NULL);
    UNITTEST_ASSERT_NE(url[strlen(url) - 1], '/');

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_CHAINMETA, 1, 2, 3), NULL);
    UNITTEST_ASSERT_NE(url[strlen(url) - 1], '/');

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_CHAINMETA, "nothing"), NULL);
    UNITTEST_ASSERT_NE(url[strlen(url) - 1], '/');

    UNITTEST_AUTO_PASS();
}


void test_get_action_by_hash() {

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_ACTIONS_BY_HASH, TEST_ACTION_HASH), NULL);
    UNITTEST_ASSERT_STR_EQ(url + strlen(url) - strlen(TEST_ACTION_HASH), TEST_ACTION_HASH, strlen(TEST_ACTION_HASH));

    UNITTEST_AUTO_PASS();
}


void test_get_action_by_addr() {

    uint32_t start, count;
    const char *start_ptr, *count_ptr, *addr_ptr;

    for (int i = 0; i < 100; i++) {

        start = random();
        count = random();

        UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_ACTIONS_BY_ADDR, TEST_ACTION_ADDR, start, count), NULL);
        UNITTEST_ASSERT_NE((start_ptr = strstr(url, "start")), NULL);
        UNITTEST_ASSERT_NE((count_ptr = strstr(url, "count")), NULL);
        UNITTEST_ASSERT_NE((addr_ptr = strstr(url, TEST_ACTION_ADDR)), NULL);
        UNITTEST_ASSERT_STR_EQ(addr_ptr, TEST_ACTION_ADDR, strlen(TEST_ACTION_ADDR));
    }

    UNITTEST_AUTO_PASS();
}


void test_transfers_by_block() {


    for (int i = 0; i < 10; i++) {

        UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_TRANSFERS_BY_BLOCK, i), NULL);
        UNITTEST_ASSERT_EQ(url[strlen(url) - 1], i + '0');
    }

    UNITTEST_ASSERT_NE(req_compose_url(url, sizeof(url), REQ_GET_TRANSFERS_BY_BLOCK, atoi(TEST_TRANSFERS_BLOCK)), NULL);
    UNITTEST_ASSERT_STR_EQ(url + strlen(url) - strlen(TEST_TRANSFERS_BLOCK), TEST_TRANSFERS_BLOCK, strlen(TEST_TRANSFERS_BLOCK));

    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {


    test_get_account();
    test_get_chainmeta();
    test_get_action_by_hash();
    test_get_action_by_addr();
    test_transfers_by_block();

    return 0;
}
