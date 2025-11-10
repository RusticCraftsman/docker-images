#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"

#define LOG_FILE "/data/ddns.log"
#define HOST "freedns.afraid.org"

void log_msg(const char *msg) {
    FILE *f = fopen(LOG_FILE, "a");
    if (!f) return;
    time_t t = time(NULL);
    char *ts = ctime(&t);
    if (ts) ts[strlen(ts)-1] = 0;
    fprintf(f, "[%s] %s\n", ts ? ts : "?", msg);
    fclose(f);
}

int update(const char *hash) {
    int ret;
    mbedtls_net_context server_fd;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    
    mbedtls_net_init(&server_fd);
    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);
    
    // Seed RNG
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) != 0) {
        log_msg("RNG seed failed");
        goto cleanup;
    }
    
    // Connect
    if ((ret = mbedtls_net_connect(&server_fd, HOST, "443", MBEDTLS_NET_PROTO_TCP)) != 0) {
        log_msg("Connect failed");
        goto cleanup;
    }
    
    // Setup SSL
    if ((ret = mbedtls_ssl_config_defaults(&conf,
                MBEDTLS_SSL_IS_CLIENT,
                MBEDTLS_SSL_TRANSPORT_STREAM,
                MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
        goto cleanup;
    }
    
    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE); // Skip cert verification
    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    
    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0) {
        goto cleanup;
    }
    
    if ((ret = mbedtls_ssl_set_hostname(&ssl, HOST)) != 0) {
        goto cleanup;
    }
    
    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);
    
    // Handshake
    while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            log_msg("Handshake failed");
            goto cleanup;
        }
    }
    
    // Send request
    char req[512];
    int reqlen = snprintf(req, sizeof(req),
             "GET /dynamic/update.php?%s HTTP/1.0\r\n"
             "Host: %s\r\n\r\n", hash, HOST);
    
    while ((ret = mbedtls_ssl_write(&ssl, (unsigned char*)req, reqlen)) <= 0) {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
            log_msg("Write failed");
            goto cleanup;
        }
    }
    
    // Read response
    char buf[512];
    ret = mbedtls_ssl_read(&ssl, (unsigned char*)buf, sizeof(buf)-1);
    
    if (ret > 0) {
        buf[ret] = 0;
        if (strstr(buf, "Updated") || strstr(buf, "has not changed")) {
            log_msg("OK");
            ret = 0;
        } else {
            log_msg("FAIL");
            ret = -1;
        }
    }
    
cleanup:
    mbedtls_net_free(&server_fd);
    mbedtls_ssl_free(&ssl);
    mbedtls_ssl_config_free(&conf);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    
    return ret;
}

int main() {
    const char *hash = getenv("FREEDNS_HASH");
    if (!hash) {
        fprintf(stderr, "ERROR: FREEDNS_HASH not set\n");
        return 1;
    }
    
    int interval = 300;
    const char *iv = getenv("INTERVAL");
    if (iv) interval = atoi(iv);
    
    log_msg("Started (mbedTLS)");
    
    while (1) {
        update(hash);
        sleep(interval);
    }
}
