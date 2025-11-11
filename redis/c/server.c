#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "lib.c"

typedef struct pollfd pollfd;
make_vector_struct(pollfd)

error process_request(int);

int main() {
    const auto fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("error creating the socket");
    }

    if (set_socket_config(fd)) {
        die("error configuring the socket");
    }

    if (bind_socket(fd, ntohl(0), ntohs(1234)) != ERR_OK) {
        die("error binding the socket");
    }

    if (listen(fd, SOMAXCONN)) {
        die("error listening on the socket");
    }

    uintptr_t_vector_t *fd2conn = uintptr_t_vector_new();
    uintptr_t_vector_init(fd2conn);

    pollfd_vector_t *poll_args = pollfd_vector_new();
    pollfd_vector_init(poll_args);

    while (true) {
        pollfd_vector_clear(poll_args);
        pollfd pfd = {fd, POLLIN, 0};
        pollfd_vector_push_back(poll_args, pfd);

        for (size_t i = 0; i < fd2conn->size; i++) {
            const conn* c = (conn*)uintptr_t_vector_at(fd2conn, i);
            if (c == nullptr) {
                continue;
            }

            pfd = (pollfd) {c->fd, POLLERR, 0};
            if (c->want_read) {
                pfd.events |= POLLIN;
            }
            if (c->want_write) {
                pfd.events |= POLLOUT;
            }
            pollfd_vector_push_back(poll_args, pfd);
        }

        const int rv = poll(poll_args->elements, (nfds_t)poll_args->size, -1);
        if (rv < 0 && errno == EINTR) {
            continue;
        }
        if (rv < 0) {
            die("poll");
        }

        // listening socket
        if (poll_args->elements[0].revents) {
            conn c = {};
            handle_accept(fd, &c);
            if ((int)fd2conn->size <= c.fd) {
                uintptr_t_vector_push_back(fd2conn, (uintptr_t)&c);
            }
        }

        // connection sockets
        for (size_t i = 1; i < poll_args->size; ++i) {
            const uint32_t ready = poll_args->elements[i].revents;
            conn* c = (conn*)fd2conn->elements[poll_args->elements[i].fd];
            if (c == nullptr) {
                continue;
            }
            if (ready & POLLIN) {
                handle_read(c);
            }
            if (ready & POLLOUT) {
                handle_write(c);
            }
            if ((ready & POLLERR) || c->want_close) {
                close(c->fd);

            }
        }
    }
}
