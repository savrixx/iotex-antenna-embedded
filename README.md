# iotex-antenna-embedded 

iotex-antenna-embedded is our C/C++ SDK allowing you to interact with a local or remote iotex blockchain node, using https connection.



## Build

```
cd iotex-antenna-embedded && make 

or 

cd iotex-antenna-embedded && make CROSS_COMPILE=xxxxxx
```

## Configure

For Linux Distributions or mac OS , you may do not need any configure, just call `iotex_emb_init(NULL)`,  then using the APIs interact  with a iotex blockchain node. 

For embedded Linux device, you need configure the cert file or directory, to tell iotex\_emb library where to find the SSL Certificate. Configure cert by passing a `iotex_emb_config` structure to `iotex_emb_init`.

## Interfaces

All of iotex-antenna-embedded APIs, return zero means succeeded, failed will return a negative error code, code's meaning you can reference `iotex_em_error`. 

For some system do not support ` __uint128_t`,  define Macro `_NO_128INT_` (make  no\_int128  ), it will use 40 bytes raw string instead. Also `u128` module provides a series of functions to uniform process 128 bit numbers.

## Example

```c
#include <stdio.h>
#include "u128.h"
#include "iotex_emb.h"


int main(int argc, char **argv) {

    int ret;
    iotex_st_config config = {0};
    iotex_st_account_meta account;
    char u128_str[UINT128_RAW_MAX_LEN];
    const char *account_address = (argc >= 2) ? argv[1] : "io1e2nqsyt7fkpzs5x7zf2uk0jj72teu5n6aku3tr";

    /* Configure cert file */
    config.cert_file = "cacert.pem";

    if ((ret = iotex_emb_init(&config)) != 0) {

        fprintf(stderr, "Initialize iotex emb failed, error code: %d\n", ret);
        return -1;
    }

    /* Get account metadata */
    if ((ret = iotex_emb_get_account_meta(account_address, &account)) != 0) {

        fprintf(stderr, "Get account metadata failed, error code: %d\n", ret);
        return -1;
    }

    fprintf(stdout, "Account: %s, balance: %s\n", account.address, u1282str(account.balance, u128_str, sizeof(u128_str)));

    /* Not required */
    iotex_emb_exit();
    return 0;
}
```



## Run example

```
cd example && make

source ../scripts/env.sh

../objs/account_meta
```

