#include <stdio.h>
#include "unittest.h"
#include "../src/config.h"


void test_search_cert_dir() {

    fprintf(stdout, "Cert dir: %s\n", config_search_cert_dir());
}

void test_search_cert_file() {

    fprintf(stdout, "Cert file: %s\n", config_search_cert_file());
}

int main(int argc, char **argv) {

    test_search_cert_dir();
    test_search_cert_file();

    return 0;
}
