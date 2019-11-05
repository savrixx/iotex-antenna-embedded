#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>


#include "debug.h"
#include "config.h"
#include "request.h"


typedef struct {
    uint32_t req;
    const char *paths[3];
    const char *args_fmt;
} iotex_st_request_conf;


typedef struct {
    char *data;
    size_t len;
} iotex_st_response_data;


/* Https api version */
static uint32_t __g_version = 1;

static const iotex_st_request_conf __g_req_configs[] = {
    {
        IotexReqGetAccount,
        {"accounts", NULL},
        "%s",
    },

    {
        IotexReqGetChainMeta,
        {"chainmeta", NULL}
    },

    {
        IotexReqGetActionsByAddr,
        {"actions", "addr", NULL},
        "%s?&start=%u&count=%u",
    },

    {
        IotexReqGetActionsByHash,
        {"actions", "hash", NULL},
        "%s",
    },

    {
        IotexReqGetTransfersByBlock,
        {"transfers", "block", NULL},
        "%u",
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


/*
 * @brief: free iotex_st_response_data
 */
static void _free_response_data(iotex_st_response_data *res) {

    if (!res || !res->data) {

        return;
    }

    free(res->data);
    res->len = 0;
    res->data = NULL;
}


/*
 * @brief: curl receive data callback, copy received data to iotex_st_response_data
 */
static size_t _curl_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {

    char *new_ptr = NULL;
    size_t new_len = size * nmemb;
    iotex_st_response_data *res = userdata;

    /* First time */
    if (res->data == NULL) {

        res->data = 0;

        if (!(res->data = malloc(new_len)))  {

            __ERROR_MSG__("malloc");
            return 0;
        }
    }
    else {

        if (!(new_ptr = realloc(res->data, res->len + new_len))) {

            __ERROR_MSG__("realloc");
            _free_response_data(res);
            return 0;
        }

        res->data = new_ptr;
    }

    /* Append new data to response data */
    memcpy(res->data + res->len, ptr, new_len);
    res->len += new_len;
    return new_len;
}


/*
 * @brief: compose https request url
 * #url: buffer to save composed url
 * #url_max_size: #url buffer max size(bytes)
 * #req: IotexHttpRequests request
 * $return: successed return composed url, failed return NULL
 *
 * TODO:
 * 1. find a way check va_args number
 */
char *req_compose_url(char *url, size_t url_max_size, IotexHttpRequests req, ...) {

    assert(url != NULL);

    int i;
    va_list ap;
    size_t path_len;
    char *url_tail = NULL;
    const iotex_st_request_conf *conf = NULL;

    /* Get request config */
    for (i = 0; __g_req_configs[i].paths[0] != NULL; i++) {

        if (__g_req_configs[i].req == req) {

            conf = __g_req_configs + i;
            break;
        }
    }

    if (!conf || !conf->paths[0]) {

        __WARN_MSG__("unknown request");
        return NULL;
    }

    /* Copy base url and version */
    memset(url, 0, url_max_size);
    snprintf(url, url_max_size, IOTEX_EMB_BASE_URL,  __g_version);

    /* Compose request url, without args */
    for (i = 0, url_tail = url + strlen(url); conf->paths[i]; i++) {

        path_len = strlen(conf->paths[i]);

        if ((url_tail - url + path_len + 1) < url_max_size) {

            memcpy(url_tail, conf->paths[i], path_len);
            url_tail += path_len;
            *url_tail = '/';
            url_tail++;
        }
        else {

            __WARN_MSG__("url buffer too short!");
            return NULL;
        }
    }

    /* No request args */
    if (!conf->args_fmt)  {

        --url_tail;
        *url_tail = 0;
        return url;
    }

    /* Append post args to url */
    va_start(ap, req);
    vsnprintf(url_tail, url_max_size - (url_tail - url), conf->args_fmt, ap);
    va_end(ap);

    return url;
}


/*
 * @brief: send a request to server and get response data
 * #url: request url, it should be composed with url and data
 * #response: store request response data
 * #response_max_size: #response buffer max len
 * $return: successed return 0, failed return negative error code
 *
 * TODO:
 * 1. add meaningful error code
 * 2. certs path and info configurable
 * 3. certs path and info auto search
 * 4. add zero copy version ? (don't forget release response)
 * 5. add two-way authentication support
 */
int req_send_request(const char *request, char *response, size_t response_max_size) {

    assert(request != NULL);
    assert(response != NULL);

    CURL *curl;
    CURLcode ret;
    iotex_st_response_data res = {};

    if (!(curl = curl_easy_init())) {

        __WARN_MSG__("curl_easy_init");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, request);

    /* Set the file with the certs vaildating the server */
    curl_easy_setopt(curl, CURLOPT_CAINFO, DEF_CA_INFO);
    curl_easy_setopt(curl, CURLOPT_CAPATH, DEF_CA_PATH);

    /* Disconnect if we can't validate server's cert */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    /* Verify the cert's name against host */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    /* Set write data and write function */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &res);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _curl_write_callback);

    if ((ret = curl_easy_perform(curl)) != CURLE_OK) {

        __WARN_MSG__(curl_easy_strerror(ret));
        curl_easy_cleanup(curl);
        return -1;
    }

    if (res.len + 1 > response_max_size) {

        _free_response_data(&res);
        curl_easy_cleanup(curl);
        return -1;
    }

    /* Copy data to response */
    memcpy(response, res.data, res.len);
    response[res.len] = 0;

    _free_response_data(&res);
    curl_easy_cleanup(curl);
    return 0;
}
