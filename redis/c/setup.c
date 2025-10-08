#include <netinet/in.h>

constexpr int ERR_CODE = -1;

void set_socket_config(const int fd) {
    constexpr auto config_so_reuseaddr = true;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &config_so_reuseaddr, sizeof(config_so_reuseaddr));
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

int bind_socket(const int fd, const in_addr_t ip, const in_port_t port) {
    const struct sockaddr_in sock_addr = get_addr(ip, port);
    return bind(fd, (const struct sockaddr*)&sock_addr, sizeof(sock_addr));
}

int get_bound_socket(const in_addr_t ip, const in_port_t port) {
    auto const fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {return ERR_CODE;}
    set_socket_config(fd);
    if (bind_socket(fd, ip, port) != 0) {return ERR_CODE;}
    return fd;
}
