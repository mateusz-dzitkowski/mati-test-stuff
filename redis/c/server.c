#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib.c"

error process_request(int);

int main() {
    const auto fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("error creating the socket");
    }

    if (bind_socket(fd, ntohl(0), ntohs(1234)) != ERR_OK) {
        die("error binding the socket");
    }

    if (listen(fd, SOMAXCONN)) {
        die("error listening on the socket");
    }

    while (true) {
        struct sockaddr_in addr = {};
        socklen_t addr_len = sizeof(addr);
        const auto conn = accept(fd, (struct sockaddr*)&addr, &addr_len);
        if (conn < 0) {
            die("error accepting on the socket");
        }
        while (true) {
            if (process_request(conn) != ERR_OK) {
                break;
            }
        }
    }
}

error process_request(const int conn) {
    error err;
    char read_buffer[MESSAGE_SIZE_BYTES + MAX_MESSAGE_SIZE];

    if ((err = read_full(conn, read_buffer, MESSAGE_SIZE_BYTES)) != ERR_OK) {
        return err;
    }

    size_t len = 0;
    memcpy(&len, read_buffer, MESSAGE_SIZE_BYTES);
    if (len > MAX_MESSAGE_SIZE) {
        return ERR_MESSAGE_TOO_LONG;
    }

    if ((err = read_full(conn, &read_buffer[MESSAGE_SIZE_BYTES], len)) != ERR_OK) {
        return err;
    }

    msg("client says `%.*s`", len, &read_buffer[MESSAGE_SIZE_BYTES]);

    constexpr char reply[] = "hello, world";
    char write_buffer[MESSAGE_SIZE_BYTES + sizeof(reply)];
    len = strlen(reply);
    memcpy(write_buffer, &len, MESSAGE_SIZE_BYTES);
    memcpy(&write_buffer[MESSAGE_SIZE_BYTES], reply, len);

    if ((err = write_full(conn, write_buffer, MESSAGE_SIZE_BYTES + len)) != ERR_OK) {
        return err;
    }

    return ERR_OK;
}
