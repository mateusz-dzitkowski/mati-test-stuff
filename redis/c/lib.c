#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/poll.h>

#include "vector.c"

make_vector_struct(int)
make_vector_struct(uint8_t)

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
    ERR_ACCEPT,
    ERR_IO,
    ERR_REQUEST,
} error;

typedef struct conn {
    int fd;
    bool want_read;
    bool want_write;
    bool want_close;
    uint8_t_vector_t incoming;
    uint8_t_vector_t outgoing;
} conn;

make_vector_struct(uintptr_t)

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

error fd_set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        return ERR_SOCKET_CONFIG;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        return ERR_SOCKET_CONFIG;
    }
    return ERR_OK;
}

error set_socket_config(const int fd) {
    constexpr auto config_so_reuseaddr = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &config_so_reuseaddr, sizeof(config_so_reuseaddr)) != 0) {
        return ERR_SOCKET_CONFIG;
    }
    return fd_set_non_blocking(fd);
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

error handle_accept(const int fd, conn* c) {
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    const int conn_fd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
    if (conn_fd < 0) {
        return ERR_ACCEPT;
    }
    const error err = fd_set_non_blocking(conn_fd);
    if (err != ERR_OK) {
        return err;
    }

    *c = (conn){};
    c->fd = conn_fd;
    c->want_read = true;

    return ERR_OK;
}

error try_one_request(conn* c) {
    if (c->incoming.size < 4) {
        return ERR_REQUEST;
    }

    uint32_t len = 0;
    memcpy(&len, c->incoming.elements, 4);
    if (len > MAX_MESSAGE_SIZE) {
        c->want_close = true;
        return ERR_REQUEST;
    }

    if (4 + len > c->incoming.size) {
        return ERR_REQUEST;
    }
    const uint8_t *request = &c->incoming.elements[4];

    uint8_t_vector_append(&c->outgoing, (uint8_t*)&len, 4);
    uint8_t_vector_append(&c->outgoing, request, len);
    uint8_t_vector_erase(&c->incoming, 0, 4+len);
    return ERR_OK;
}

error handle_read(conn* c) {
    uint8_t buf[64 * 1024];
    const ssize_t rv = read(c->fd, buf, sizeof(buf));
    if (rv == 0) {
        c->want_close = true;
        return ERR_OK;
    }
    if (rv < 0) {
        c->want_close = true;
        return ERR_IO;
    }

    uint8_t_vector_append(&c->incoming, buf, (size_t)rv);
    const error err = try_one_request(c);
    if (err != ERR_OK) {
        return err;
    }

    if (c->outgoing.size > 0) {
        c->want_read = false;
        c->want_write = true;
    }

    return ERR_OK;
}

error handle_write(conn* c) {
    if (c->outgoing.size <= 0) {
        return ERR_WRITE;
    }
    const ssize_t rv = write(c->fd, c->outgoing.elements, c->outgoing.size);
    if (rv < 0) {
        c->want_close = true;
        return ERR_WRITE;
    }

    uint8_t_vector_erase(&c->outgoing, 0, rv);

    if (c->outgoing.size == 0) {
        c->want_read = true;
        c->want_write = false;
    }

    return ERR_OK;
}
