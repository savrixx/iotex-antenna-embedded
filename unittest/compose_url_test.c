#include <stdio.h>
#include "request.h"


int main(int argc, char **argv) {

    uint32_t i;
    char url[256];

    for (i = 0; i < IotexReqTail; i++) {

        fprintf(stdout, "%s\n", req_compose_url(url, sizeof(url), (IotexHttpRequests)i));
    }

    return 0;
}
