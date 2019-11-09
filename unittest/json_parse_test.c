#include <stdio.h>
#include "debug.h"
#include "config.h"
#include "unittest.h"
#include "test_config.h"
#include "../src/u128.h"
#include "../src/parse.h"
#include "../src/request.h"
#include "../src/iotex_emb.h"


void json_parse_chain_meta() {

    iotex_st_chain_meta chainmeta;
    UNITTEST_AUTO_TRUE(iotex_emb_get_chainemeta(&chainmeta) == 0);
}


void json_parse_account_meta() {

    iotex_st_account_meta account;

    UNITTEST_ASSERT_EQ(0, iotex_emb_get_accountmeta(TEST_ACCOUNT_ADDR, &account));
    UNITTEST_ASSERT_STR_EQ(account.address, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));

    u128_print(account.balance);
    u128_print(account.nonce);
    u128_print(account.pendingNonce);
    u128_print(account.numActions);
    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {

    json_parse_chain_meta();
    json_parse_account_meta();
    return 0;
}
