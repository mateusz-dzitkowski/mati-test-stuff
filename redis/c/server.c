#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "setup.c"

void process_conn(int);

int main() {
    const auto fd = get_bound_socket(htonl(0), htons(1234));
    if (fd == ERR_CODE) {
        return EXIT_FAILURE;
    }
    if (listen(fd, SOMAXCONN)) {return ERR_CODE;}

    while (true) {
        struct sockaddr_in addr = {};
        socklen_t addr_len = sizeof(addr);
        const auto conn = accept(fd, (struct sockaddr*)&addr, &addr_len);
        if (conn < 0) {return EXIT_FAILURE;}
        process_conn(conn);
    }
}

void process_conn(const int conn) {
    char read_buffer[64] = {};
    const auto n = read(conn, read_buffer, sizeof(read_buffer) - 1);
    if (n < 0) {return;}
    printf("client says %s\n", read_buffer);

    constexpr char write_buffer[] = "hello, world";
    write(conn, write_buffer, sizeof(write_buffer));
}