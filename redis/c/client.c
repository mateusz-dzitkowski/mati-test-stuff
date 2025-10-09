#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "setup.c"

int main() {
    auto const fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {return EXIT_FAILURE;}

    const struct sockaddr_in addr = get_addr(ntohl(INADDR_LOOPBACK), ntohs(1234));

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        printf("failed to connect\n");
        return EXIT_FAILURE;
    }

    constexpr char write_buffer[] = "hey guy";
    write(fd, &write_buffer, sizeof(write_buffer));

    char read_buffer[64] = {};
    const auto n = read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (n < 0) {
        printf("failed to read\n");
        return EXIT_FAILURE;
    }
    printf("server says: %s", read_buffer);

    return EXIT_SUCCESS;
}
