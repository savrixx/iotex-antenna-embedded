#ifndef _IOTEX_EMB_RULE_H_
#define _IOTEX_EMB_RULE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "parse.h"

int rule_action_info_bind(json_parse_rule *rule_chain, void *element);

#ifdef	__cplusplus
}
#endif

#endif /* _IOTEX_EMB_RULE_H_ */
