#ifndef _IOTEX_ABI_READ_CONTRACT_H_
#define _IOTEX_ABI_READ_CONTRACT_H_

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

uint64_t abi_get_order_start(const char *input);
uint64_t abi_get_order_duration(const char *input);
const char *abi_get_order_endpoint(const char *input);
const char *abi_get_order_token(const char *input);

#ifdef	__cplusplus
}
#endif

#endif /* _IOTEX_ABI_READ_CONTRACT_H_ */
