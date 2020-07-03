/* tcp4_client.c -- TCP client (IPv4) skeleton */

#include "einet.h"

/* main */

int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in peer;
    int s, argn, pid;

    init(argv[0], envp);
    pid = getpid();
    argn = parse_options(argc, argv);

    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
        error(1, errno, "socket call failed");

    if (set_address4(argv[argn], iname, sname, &peer, "tcp") == NULL)
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