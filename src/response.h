#ifndef _IOTEX_EMB_RESPONSE_H_
#define _IOTEX_EMB_RESPONSE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "parse.h"

int res_get_data(const char *request, json_parse_rule *rules);


#ifdef __cplusplus
}
#endif

#endif /* _IOTEX_EMB_RESPONSE_H_ */
