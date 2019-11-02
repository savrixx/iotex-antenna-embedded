#ifndef _IOTEX_EMB_CONFIG_H_
#define _IOTEX_EMB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif


/* URL */
#define IOTEX_EMB_BASE_URL "https://pharos.iotex.io/v%d/"
#define IOTEX_EMB_MAX_URL_LEN 256

/* Certs */
#define DEF_CA_INFO "/etc/ssl/certs/ca-certificates.crt"
#define DEF_CA_PATH "/etc/ssl/certs"


#ifdef __cplusplus
}
#endif

#endif /* _IOTEX_EMB_CONFIG_H_ */
