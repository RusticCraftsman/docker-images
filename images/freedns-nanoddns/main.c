#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "/data/ddns.log"
#define HOST "freedns.afraid.org"

void log_msg(const char *msg) {
    FILE *f = fopen(LOG_FILE, "a");
    if (!f) return;
    time_t t = time(NULL);
    char *ts = ctime(&t);
    ts[strlen(ts)-1] = 0;
    fprintf(f, "[%s] %s\n", ts, msg);
    fclose(f);
}

int update(const char *hash) {
    static struct in_addr cached_addr;
    static int addr_cached = 0;

    if (!addr_cached) {
        struct hostent *he = gethostbyname(HOST);
        if (!he) return -1;
        memcpy(&cached_addr, he->h_addr_list[0], sizeof(cached_addr));
        addr_cached = 1;
    }
    
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr, &cached_addr, sizeof(cached_addr));
    
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(sock);
        return -1;
    }
    
    char req[512];
    snprintf(req, sizeof(req),
             "GET /dynamic/update.php?%s HTTP/1.0\r\n"
             "Host: %s\r\n\r\n", hash, HOST);
    
    write(sock, req, strlen(req));
    
    char buf[512];
    int n = read(sock, buf, sizeof(buf)-1);
    close(sock);
    
    if (n > 0) {
        buf[n] = 0;
        if (strstr(buf, "Updated") || strstr(buf, "has not changed")) {
            log_msg("OK");
            return 0;
        }
    }
    
    log_msg("FAIL");
    return -1;
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
    
    
    log_msg("Started (HTTP)");
    
    while (1) {
        update(hash);
        sleep(interval);
    }
}
