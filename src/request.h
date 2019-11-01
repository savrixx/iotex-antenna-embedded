#ifndef _IOTEX_EMB_REQUEST_H_
#define _IOTEX_EMB_REQUEST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {

    IotexReqGetAccount,
    IotexReqGetChainMeta,
    IotexReqGetTransfersByBlock,

    IotexReqGetActionsByHash,
    IotexReqGetActionsByAddr,

    IotexReqGetMemberValidators,
    IotexReqGetMemberDelegations,

    IotexReqSendSignedActionBytes,


    IotexReqTail
} IotexHttpRequests;


char *req_compose__url(char *url, size_t url_max_size, IotexHttpRequests req);

#ifdef __cplusplus
}
#endif

#endif /* _IOTEX_EMB_REQUEST_H_ */
