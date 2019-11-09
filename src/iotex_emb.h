#ifndef _IOTEX_EMB_H_
#define _IOTEX_EBM_H_


#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "u128.h"
#include "config.h"

/* Typedef */
typedef char iotex_t_account[IOTEX_EMB_LIMIT_ACCOUNT_LEN];
typedef char iotex_t_hash[IOTEX_EMB_LIMIT_HASH_LEN];
typedef char iotex_t_time[IOTEX_EMB_LIMIT_TIME_LEN];
typedef char iotex_t_key[IOTEX_EMB_LIMIT_KEY_LEN];
typedef uint128_t iotex_t_number;


typedef struct {
    iotex_t_account address;
    iotex_t_number balance;
    iotex_t_number nonce;
    iotex_t_number pendingNonce;
    iotex_t_number numActions;
} iotex_st_account_meta;


typedef struct {
    iotex_t_number height;
    iotex_t_number numActions;
    iotex_t_number tps;
    struct {
        iotex_t_number num;
        iotex_t_number height;
        iotex_t_number gravityChainStartHeight;
    } epoch;
    double tpsFloat;
} iotex_st_chain_meta;


typedef struct iotex_st_action_info {
    struct {
        struct {
            iotex_t_number nonce;
            iotex_t_number version;
            iotex_t_number gasLimit;
            iotex_t_number gasPrice;
            struct {
                iotex_t_number amount;
                iotex_t_account recipient;
            } transfer;
        } core;
        iotex_t_key senderPubKey;
        iotex_t_key signature;
    } action;

    iotex_t_hash actHash;
    iotex_t_hash blkHash;
    iotex_t_number blkHeight;
    iotex_t_account sender;
    iotex_t_number gasFee;
    iotex_t_time timestamp;
} iotex_st_action_info;


int iotex_emb_get_chainemeta(iotex_st_chain_meta *chain);
int iotex_emb_get_accountmeta(const char *account, iotex_st_account_meta *meta);

#ifdef	__cplusplus
}
#endif


#endif /* _IOTEX_EMB_H_ */
