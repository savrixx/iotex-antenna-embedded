#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include "request.h"
#include "config.h"


typedef struct {

    uint32_t req;
    const char *paths[3];
} iotex_st_request_conf;


/* Https api version */
static uint32_t __g_version = 1;

static const iotex_st_request_conf __g_req_configs[] = {

    {
        IotexReqGetAccount,
        {"accounts", NULL}
    },

    {
        IotexReqGetChainMeta,
        {"chainmeta", NULL}
    },

    {
        IotexReqGetActionsByAddr,
        {"actions", "addr", NULL}
    },

    {
        IotexReqGetActionsByHash,
        {"actions", "hash", NULL}
    },

    {
        IotexReqGetTransfersByBlock,
        {"transfers", "block", NULL}
    },

    {
        IotexReqGetMemberValidators,
        {"staking", "validators", NULL}
    },

    {
        IotexReqGetMemberDelegations,
        {"staking", "delegations", NULL}
    },

    {
        IotexReqSendSignedActionBytes,
        {"actionbytes", "signedbytes", NULL}
    },

    {
        IotexReqTail, {NULL}
    }
};



char *req_compose__url(char *url, size_t url_max_size, IotexHttpRequests req) {

    int i;
    size_t path_len;
    char *url_tail = NULL;
    const iotex_st_request_conf *conf = NULL;

    /* Get request config */
    for (i = 0; __g_req_configs[i].paths != NULL; i++) {

        if (__g_req_configs[i].req == req) {

            conf = __g_req_configs + i;
            break;
        }
    }

    if (!conf || !conf->paths) {

        return NULL;
    }

    /* Copy base url and version */
    memset(url, 0, url_max_size);
    snprintf(url, url_max_size, IOTEX_EMB_BASE_URL,  __g_version);

    for (i = 0, url_tail = url + strlen(url); conf->paths[i]; i++) {

        path_len = strlen(conf->paths[i]);

        if ((url_tail - url + path_len + 1) < url_max_size) {

            memcpy(url_tail, conf->paths[i], path_len);
            url_tail += path_len;
            *url_tail = '/';
            url_tail++;
        }
        else {

            fprintf(stderr, "url too short!\n");
            return NULL;
        }
    }

    return url;
}
