#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

constexpr size_t MAX_MESSAGE_SIZE = 4096;
constexpr size_t MESSAGE_SIZE_BYTES = 4;

typedef enum {
    ERR_OK = 0,
    ERR_READ,
    ERR_WRITE,
    ERR_MESSAGE_TOO_LONG,
    ERR_SOCKET_CONFIG,
    ERR_SOCKET_BIND,
    ERR_EOF,
} error;

void msg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void die(const char *msg) {
    fprintf(stderr, "[%d] - %s\n", errno, msg);
    abort();
}

error read_full(const int fd, char *buf, size_t n) {
    while (n > 0) {
        const ssize_t bytes_read = read(fd, buf, n);
        if (bytes_read == 0) {
            return ERR_EOF;
        }
        if (bytes_read < 0) {
            return ERR_READ;
        }
        if ((size_t)bytes_read > n) {
            return ERR_MESSAGE_TOO_LONG;
        }
        n -= (size_t)bytes_read;
        buf += bytes_read;
    }
    return ERR_OK;
}

error write_full(const int fd, const char *buf, size_t n) {
    while (n > 0) {
        const ssize_t bytes_written = write(fd, buf, n);
        if (bytes_written < 0) {
            return ERR_WRITE;
        }
        if ((size_t)bytes_written > n) {
            return ERR_MESSAGE_TOO_LONG;
        }
        n -= (size_t)bytes_written;
        buf += bytes_written;
    }
    return ERR_OK;
}

error set_socket_config(const int fd) {
    constexpr auto config_so_reuseaddr = true;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &config_so_reuseaddr, sizeof(config_so_reuseaddr)) != 0) {
        return ERR_SOCKET_CONFIG;
    }
    return ERR_OK;
}

struct sockaddr_in get_addr(const in_addr_t ip, const in_port_t port) {
    const struct sockaddr_in sock_addr = {
        .sin_family = AF_INET,
        .sin_port = port,
        .sin_addr = {
            .s_addr = ip,
        },
    };
    return sock_addr;
}

error bind_socket(const int fd, const in_addr_t ip, const in_port_t port) {
    const struct sockaddr_in sock_addr = get_addr(ip, port);
    if (bind(fd, (const struct sockaddr*)&sock_addr, sizeof(sock_addr)) != 0) {
        return ERR_SOCKET_BIND;
    }
    return ERR_OK;
}
