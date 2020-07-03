/* tcp64_client.c -- TCP (IPv6/IPv4) client skeleton */

#include "einet.h"

int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in peer4;
    struct sockaddr_in6 peer6, *peer6p;
    int s, argn, pid;

    init(argv[0], envp);
    pid = getpid();
    argn = parse_options(argc, argv);

    s = socket(PF_INET6, SOCK_STREAM, 0);
    if (s < 0)
        error(1, errno, "socket call failed");

    peer6p = set_address6(argv[argn], iname, sname, &peer6, "tcp");
    if (peer6p != NULL)
    {

        if (connect(s, (struct sockaddr *)&peer6, sizeof(peer6)) < 0)
        {
            fprintfnl(stderr, "connect call failed via IPv6.");
            peer6p = NULL;
        }
        else
        {
            client(s, (struct sockaddr *)&peer6, sizeof(peer6));
            if ((getpid() == pid) && (silent == 0))
            {
                printfnl("connected via IPv6.");
                print_local_name(s);
                print_peer_name((struct sockaddr *)&peer6);
            }
        }
    }

    if (peer6p == NULL)
    {

        if (close(s) < 0)
            error(1, errno, "close call failed");

        s = socket(PF_INET, SOCK_STREAM, 0);
        if (s < 0)
            error(1, errno, "socket call failed");

        if (set_address4(argv[argn], iname, sname, &peer4, "tcp") == NULL)
            error(1, errno, "set_address failed");

        if (connect(s, (struct sockaddr *)&peer4, sizeof(peer4)) < 0)
        {
            fprintfnl(stderr, "connect call failed via IPv4.");
            exit(1);
        }

        client(s, (struct sockaddr *)&peer4, sizeof(peer4));
        if ((getpid() == pid) && (silent == 0))
        {
            printfnl("connected via IPv4.");
            print_local_name(s);
            print_peer_name((struct sockaddr *)&peer4);
        }
    }

    if (close(s) < 0)
        error(1, errno, "close call failed");
    exit(0);
}