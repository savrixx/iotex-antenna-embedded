#ifndef _IOTEX_EMB_H_
#define _IOTEX_EBM_H_


#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "u128.h"

typedef struct {

    char address[64];
    uint128_t balance;
    uint128_t nonce;
    uint128_t pendingNonce;
    uint128_t numActions;
} iotex_st_account_meta;

typedef struct {

    uint128_t height;
    uint128_t numActions;
    uint128_t tps;
    struct {
        uint128_t num;
        uint128_t height;
        uint128_t gravityChainStartHeight;
    } epoch;
    double tpsFloat;
} iotex_st_chain_meta;


int iotex_emb_get_chainemeta(iotex_st_chain_meta *chain);
int iotex_emb_get_accountmeta(const char *account, iotex_st_account_meta *meta);

#ifdef	__cplusplus
}
#endif


#endif /* _IOTEX_EMB_H_ */
