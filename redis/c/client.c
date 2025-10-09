#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lib.c"

error query(int fd, const char *text);

int main(int argc, char *argv[]) {
    auto const fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        die("error creating the socket");
    }

    const struct sockaddr_in addr = get_addr(ntohl(INADDR_LOOPBACK), ntohs(1234));

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        die("failed to connect");
    }

    for (int i = 1; i < argc; i++) {
        if (query(fd, argv[i]) != ERR_OK) {
            die("error issuing query");
        }
    }

    return EXIT_SUCCESS;
}

error query(const int fd, const char *text) {
    error err;

    auto len = strlen(text);
    if (len > MAX_MESSAGE_SIZE) {
        return ERR_MESSAGE_TOO_LONG;
    }

    char write_buffer[MESSAGE_SIZE_BYTES + MAX_MESSAGE_SIZE];
    memcpy(write_buffer, &len, MESSAGE_SIZE_BYTES);
    memcpy(&write_buffer[MESSAGE_SIZE_BYTES], text, len);

    if ((err = write_full(fd, write_buffer, MESSAGE_SIZE_BYTES + len)) != ERR_OK) {
        return err;
    }

    char read_buffer[MESSAGE_SIZE_BYTES + MAX_MESSAGE_SIZE];
    if ((err = read_full(fd, read_buffer, MESSAGE_SIZE_BYTES)) != ERR_OK) {
        return err;
    }

    memcpy(&len, read_buffer, MESSAGE_SIZE_BYTES);
    if ((err = read_full(fd, &read_buffer[MESSAGE_SIZE_BYTES], len)) != ERR_OK) {
        return err;
    }

    msg("server says: `%.*s`", len, &read_buffer[MESSAGE_SIZE_BYTES]);

    return ERR_OK;
}
