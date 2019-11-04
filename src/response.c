#include "response.h"

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

static json_parse_rule account_rule = {

    "accountMeta", IOTEX_RES_TYPE_STRUCT, account_meta
};
