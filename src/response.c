#include <stdlib.h>
#include "parse.h"
#include "config.h"
#include "request.h"
#include "response.h"


int res_get_data(const char *request, json_parse_rule *rules) {

    char *response = NULL;

    if ((response = malloc(IOTEX_EBM_MAX_RES_LEN)) == NULL) {

        return -1;
    }

    if (req_send_request(request, response, IOTEX_EBM_MAX_RES_LEN) != 0) {

        free(response);
        return -1;
    }

    if (json_parse_response(response, rules) != 0) {

        free(response);
        return -1;
    }

    free(response);
    return 0;
}
