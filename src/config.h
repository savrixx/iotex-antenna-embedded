#ifndef _IOTEX_EMB_CONFIG_H_
#define _IOTEX_EMB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/* URL */
#define IOTEX_EMB_BASE_URL "https://pharos.iotex.io/v%d/"
#define IOTEX_EMB_MAX_URL_LEN 256

/* Response */
#define IOTEX_EBM_MAX_RES_LEN (32 * 1024)

/*
 * Certs
 * TODO: certs path and info auto search and configurable
 */
#ifdef __APPLE__
#define DEF_CA_INFO "/etc/ssl/cert.pem"
#else
#define DEF_CA_INFO "/etc/ssl/certs/ca-certificates.crt"
#endif

#define DEF_CA_PATH "/etc/ssl/certs"

/* Data type length */
#define IOTEX_EMB_LIMIT_ACCOUNT_LEN 42
#define IOTEX_EMB_LIMIT_TIME_LEN 21
#define IOTEX_EMB_LIMIT_HASH_LEN 65
#define IOTEX_EMB_LIMIT_KEY_LEN 89
#define IOTEX_EMB_LIMIT_ID_LEN 16

#ifdef __cplusplus
}
#endif

#endif /* _IOTEX_EMB_CONFIG_H_ */
