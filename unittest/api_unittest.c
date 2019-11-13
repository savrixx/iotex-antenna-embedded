#include <stdio.h>
#include "debug.h"
#include "config.h"
#include "unittest.h"
#include "test_config.h"
#include "../src/u128.h"
#include "../src/debug.h"
#include "../src/parse.h"
#include "../src/request.h"
#include "../src/iotex_emb.h"


void test_get_chain_meta() {

    iotex_st_chain_meta chainmeta;
    UNITTEST_ASSERT_EQ(0, iotex_emb_get_chain_meta(&chainmeta));

    UNITTEST_AUTO_TRUE(iotex_emb_get_chain_meta(&chainmeta) == 0);

    UNITTEST_ASSERT_GT(chainmeta.height, 1704889);
    UNITTEST_ASSERT_GT(chainmeta.numActions, 1836031);

    UNITTEST_ASSERT_GT(chainmeta.epoch.num, 4736);
    UNITTEST_ASSERT_GT(chainmeta.epoch.height, 1704601);
    UNITTEST_ASSERT_GT(chainmeta.epoch.gravityChainStartHeight, 8887300);
}


void test_get_account_meta() {

    iotex_st_account_meta account;

    UNITTEST_ASSERT_EQ(0, iotex_emb_get_account_meta(TEST_ACCOUNT_ADDR, &account));
    UNITTEST_ASSERT_STR_EQ(account.address, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));

    UNITTEST_ASSERT_GT(account.nonce, 30);
    UNITTEST_ASSERT_GT(account.numActions, 43);
    UNITTEST_AUTO_PASS();
}


void test_get_transfer_by_block() {

    iotex_st_action_info action_info;
    UNITTEST_ASSERT_EQ(1, iotex_emb_get_transfer_block(construct_u128(TEST_TRANSFERS_BLOCK), &action_info));

    /* iotex_st_action_info */
    UNITTEST_ASSERT_STR_EQ(action_info.actHash, TEST_TRANSFERS_BLOCK_ACTHASH, strlen(TEST_TRANSFERS_BLOCK_ACTHASH));
    UNITTEST_ASSERT_STR_EQ(action_info.blkHash, TEST_TRANSFERS_BLOCK_BLKHASH, strlen(TEST_TRANSFERS_BLOCK_BLKHASH));
    UNITTEST_ASSERT_EQ(1, u128_equal(action_info.blkHeight, construct_u128(TEST_TRANSFERS_BLOCK)));
    UNITTEST_ASSERT_STR_EQ(action_info.sender, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));
    UNITTEST_ASSERT_STR_EQ(action_info.timestamp, TEST_TRANSFERS_BLOCK_TIMESTAMP, strlen(TEST_TRANSFERS_BLOCK_TIMESTAMP));

    /* iotex_st_action_info.action */
    UNITTEST_ASSERT_STR_EQ(action_info.action.signature, TEST_TRANSFERS_BLOCK_SIGNATURE, strlen(TEST_TRANSFERS_BLOCK_SIGNATURE));
    UNITTEST_ASSERT_STR_EQ(action_info.action.senderPubKey, TEST_TRANSFERS_BLOCK_SENDER_PUBKEY, strlen(TEST_TRANSFERS_BLOCK_SENDER_PUBKEY));

    /* iotex_st_action_info.action.core */
    UNITTEST_ASSERT_EQ(1, u128_equal(1, action_info.action.core.version));
    UNITTEST_ASSERT_EQ(1, u128_equal(2, action_info.action.core.nonce));
    UNITTEST_ASSERT_EQ(1, u128_equal(20000, action_info.action.core.gasLimit));
    UNITTEST_ASSERT_EQ(1, u128_equal(1000000000000, action_info.action.core.gasPrice));

    /* iotex_st_action_info.action.core.transfer */
    UNITTEST_ASSERT_EQ(1, u128_equal(1000000000000000, action_info.action.core.transfer.amount));
    UNITTEST_ASSERT_STR_EQ(action_info.action.core.transfer.recipient, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));

    UNITTEST_AUTO_PASS();
}


int main(int argc, char **argv) {

    test_get_chain_meta();
    test_get_account_meta();
    test_get_transfer_by_block();
    return 0;
}
