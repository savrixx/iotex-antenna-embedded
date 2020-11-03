#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abi_read_contract.h"
#include "endian_conv.h"

// input is the bytes returned by reading contract's getDeviceOrderByID()
// parse the bytes and return start height
// returns 0 for error
uint64_t abi_get_order_start(const char *input) {
    if (strlen(input) < 32) {
        return 0;
    }

    uint64_t start = *(uint64_t *)(input + 24);
    if (!endian_is_bigendian()) {
        start = endian_swap64(start);
    }
    return start;
}

// input is the bytes returned by reading contract's getDeviceOrderByID()
// parse the bytes and return duration
// returns 0 for error
uint64_t abi_get_order_duration(const char *input) {
    if (strlen(input) < 64) {
        return 0;
    }

    uint64_t duration = *(uint64_t *)(input + 56);
    if (!endian_is_bigendian()) {
        duration = endian_swap64(duration);
    }
    return duration;
}

// input is the bytes returned by reading contract's getDeviceOrderByID()
// parse the bytes and return subscriber's storage endpoint
// returns NULL for error
const char *abi_get_order_endpoint(const char *input) {
    return NULL;
}

// input is the bytes returned by reading contract's getDeviceOrderByID()
// parse the bytes and return subscriber's storage access token
// returns NULL for error
const char *abi_get_order_token(const char *input) {
    return NULL;
}