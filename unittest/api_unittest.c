#include <stdio.h>
#include <stdlib.h>
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
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1"), action_info.action.core.version));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("2"), action_info.action.core.nonce));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("20000"), action_info.action.core.gasLimit));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1000000000000"), action_info.action.core.gasPrice));

    /* iotex_st_action_info.action.core.transfer */
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1000000000000000"), action_info.action.core.transfer.amount));
    UNITTEST_ASSERT_STR_EQ(action_info.action.core.transfer.recipient, TEST_ACCOUNT_ADDR, strlen(TEST_ACCOUNT_ADDR));

    UNITTEST_AUTO_PASS();
}


void test_get_action_by_hash() {

    iotex_st_action_info action_info;
    UNITTEST_ASSERT_EQ(1, iotex_emb_get_action_by_hash(TEST_ACTION_HASH, &action_info));

    /* iotex_st_action_info */
    UNITTEST_ASSERT_STR_EQ(action_info.actHash, TEST_ACTION_HASH, strlen(action_info.actHash));
    UNITTEST_ASSERT_STR_EQ(action_info.blkHash, "33e1d2858cec24059f22348b862a2f415a21bb14b7d96733249a12e96c542969", strlen(action_info.blkHash));
    UNITTEST_ASSERT_EQ(1, u128_equal(action_info.blkHeight, construct_u128("222656")));
    UNITTEST_ASSERT_STR_EQ(action_info.sender, "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr", strlen(action_info.sender));
    UNITTEST_ASSERT_STR_EQ(action_info.timestamp, "2019-05-17T23:26:20Z", strlen(action_info.timestamp));
    UNITTEST_ASSERT_EQ(1, u128_equal(action_info.gasFee, construct_u128("10000000000000000")));

    /* iotex_st_action_info.action */
    UNITTEST_ASSERT_STR_EQ(action_info.action.signature, "awRLFCvU4X5SVyz2IDU5rdjmKjUk3BOchmt/3bmvgi9GJJW3pat4I0i/qqROowPbVJ8nj+eZNQ5Okhgt6ezPgAE=", strlen(action_info.action.signature));
    UNITTEST_ASSERT_STR_EQ(action_info.action.senderPubKey, "BLhgbOGdny7iNzyHe9axp5KWTb8sMJzad78+bc5cTYRAUqVNF6igy5t9z2jqM2Zneiw17d6xSgbokcDnVRxmuM8=", strlen(action_info.action.senderPubKey));

    /* iotex_st_action_info.action.core */
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1"), action_info.action.core.version));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1"), action_info.action.core.nonce));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("20000"), action_info.action.core.gasLimit));
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1000000000000"), action_info.action.core.gasPrice));

    /* iotex_st_action_info.action.core.transfer */
    UNITTEST_ASSERT_EQ(1, u128_equal(construct_u128("1000000000000000"), action_info.action.core.transfer.amount));
    UNITTEST_ASSERT_STR_EQ(action_info.action.core.transfer.recipient, "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr", strlen(action_info.action.core.transfer.recipient));

    UNITTEST_AUTO_PASS();
}


void test_get_action_by_addr() {

    int total_actions;
    size_t count = 100;
    iotex_st_action_info *actions;

    if (!(actions = calloc(sizeof(iotex_st_action_info), count))) {

        UNITTEST_FAIL("calloc");
    }

    UNITTEST_ASSERT_NE(-1, (total_actions = iotex_emb_get_action_by_addr(TEST_ACCOUNT_ADDR, 0, count, actions, count)));
    fprintf(stdout, "Address: %s, actual actions number: %d\n", TEST_ACCOUNT_ADDR, total_actions);
    UNITTEST_AUTO_PASS();
}

void test_get_validators() {

    size_t validators_actual_size, i;
    iotex_st_validator validators[100], *m;

    char locktime[UINT128_RAW_MAX_LEN];
    char annual_reward[UINT128_RAW_MAX_LEN];
    char minimum_amount[UINT128_RAW_MAX_LEN];

    UNITTEST_ASSERT_EQ(0, iotex_emb_get_validators(validators, 100, &validators_actual_size));

    fprintf(stdout, "Validators actual size: %zu, first id: %s, last id: %s\n",
            validators_actual_size, validators[0].id, validators[validators_actual_size - 1].id);

    for (i = 0, m = validators; i < validators_actual_size; i++, m++) {

        fprintf(stdout, "[%03zu] id: %s, status: %s, annual reward: %s, locktime: %s, minimum_amount: %s\n",
                i, m->id, m->status ? "true" : "false",
                u1282str(m->details.reward.annual, annual_reward, sizeof(annual_reward)),
                u1282str(m->details.locktime, locktime, sizeof(locktime)),
                u1282str(m->details.minimum_amount, minimum_amount, sizeof(minimum_amount)));
    }

    UNITTEST_AUTO_PASS();
}

int main(int argc, char **argv) {

    iotex_emb_init(NULL);

    test_get_chain_meta();
    test_get_account_meta();
    test_get_transfer_by_block();

    test_get_action_by_hash();
    test_get_action_by_addr();

    test_get_validators();
    return 0;
}
