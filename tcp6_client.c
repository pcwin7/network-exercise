/* tcp6_client.c -- TCP (IPv6) client skeleton */

#include "einet.h"

int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in6 peer;
    int s, argn, pid;
    char hlit[NI_MAXHOST], slit[NI_MAXSERV];

    init(argv[0], envp);
    pid = getpid();
    argn = parse_options(argc, argv);

    s = socket(PF_INET6, SOCK_STREAM, 0);
    if (s < 0)
        error(1, errno, "socket call failed");

    if (set_address6(argv[argn], iname, sname, &peer, "tcp") == NULL)
        error(1, errno, "set address failed");

    if (connect(s, (struct sockaddr *)&peer, sizeof(peer)) < 0)
        error(1, errno, "connect call failed");

    client(s, (struct sockaddr *)&peer, sizeof(peer));

    if ((getpid() == pid) && (silent == 0))
    {
        print_local_name(s);
        print_peer_name((struct sockaddr *)&peer);
    }

    if (close(s) < 0)
        error(1, errno, "close call failed");
    exit(0);
}