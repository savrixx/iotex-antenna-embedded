#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"


static const char *__g_cert_files[] = {

    "/etc/ssl/certs/ca-certificates.crt",                // Debian/Ubuntu/Gentoo etc.
    "/etc/pki/tls/certs/ca-bundle.crt",                  // Fedora/RHEL 6
    "/etc/ssl/ca-bundle.pem",                            // OpenSUSE
    "/etc/pki/tls/cacert.pem",                           // OpenELEC
    "/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem", // CentOS/RHEL 7
    "/etc/ssl/cert.pem",                                 // Alpine Linux, macOS
    NULL
};


static const char *__g_cert_dirs[] = {

    "/etc/ssl/certs",               // SLES10/SLES11, https://golang.org/issue/12139
    "/system/etc/security/cacerts", // Android
    "/usr/local/share/certs",       // FreeBSD
    "/etc/pki/tls/certs",           // Fedora/RHEL
    "/etc/openssl/certs",           // NetBSD
    "/var/ssl/certs",               // AIX,
    NULL
};

/* Get file type */
static mode_t __get_file_type(const char *path) {

    struct stat attr;
    memset(&attr, 0, sizeof(attr));

    if (stat(path, &attr) != 0) {

        return -1;
    }

    return attr.st_mode;
}

/* Search cert file */
const char *config_search_cert_file() {

    const char **file = NULL;

    for (file = __g_cert_files; *file; file++) {

        if (S_ISREG(__get_file_type(*file))) {

            return *file;
        }
    }

    return NULL;
}

/* Search cert dir */
const char *config_search_cert_dir() {

    const char **dir = NULL;

    for (dir = __g_cert_dirs; *dir; dir++) {

        if (S_ISDIR(__get_file_type(*dir))) {

            return *dir;
        }
    }

    return NULL;
}
