/* tcp6_server3.c -- TCP (IPv6) server skeleton */

#include "einet.h"

/* catch signal */
void sig_child(int signo)
{
    int status;

    waitpid(-1, &status, WNOHANG);
}

/* main */
int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in6 local, peer;
    socklen_t plen;
    int argn, s, p, pid;

    init(argv[0], envp);
    argn = parse_options(argc, argv);

    s = socket(PF_INET6, SOCK_STREAM, 0);
    if (s < 0)
        error(1, errno, "socket call failed");
    if (set_address6(argv[argn], iname, sname, &local, "tcp") == NULL)
        error(1, errno, "set address failed");
    if (bind(s, (struct sockaddr *)&local, sizeof(local)) < 0)
        error(1, errno, "bind call failed");
    if (listen(s, NLISTEN) < 0)
        error(1, errno, "listen call failed");

    if (signal(SIGCHLD, sig_child) == SIG_ERR)
        error(1, errno, "signal call failed");

    plen = sizeof(peer);
    while (1)
    {
        if ((p = accept(s, (struct sockaddr *)&peer, &plen)) < 0)
            error(1, errno, "accept call failed");
        pid = fork();
        if (pid < 0)
            error(1, errno, "fork call failed");
        else if (pid == 0)
        { /* child */
            server(p, (struct sockaddr *)&peer, sizeof(peer));
            if (silent == 0)
            {
                print_local_name(s);
                print_peer_name((struct sockaddr *)&peer);
            }

            if (close(p) < 0)
                error(1, errno, "close call failed");
            exit(0);
        }
        else if (silent == 0)
        {
            putnl();
            printfnl("ppid: %d", getpid());
            printfnl("cpid: %d", pid);
        }
    }
    exit(0);
}