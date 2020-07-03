/* tcp_echo_cli2.c -- TCP ECHO client (2 process) */

#include "einet.h"

void sender(int s)
{
    static char send_buf[SEND_BUF_SIZE];

    char *ptr;
    int pat, count, slen, total = 0;

    ptr = send_buf;
    for (count = num_rep; count-- > 0;)
        for (pat = PAT_BEGIN; pat <= PAT_END; pat++)
            *ptr++ = pat;

    for (count = num_echo; count-- > 0;)
    {
        slen = send(s, send_buf, (PAT_END - PAT_BEGIN + 1) * num_rep, 0);
        if (slen >= 0)
        {
            total += slen;
            if (verbose)
                printfnl("send: %6d", slen);
        }
        else
            error(1, errno, "send failed");
    }
    if (silent == 0)
        printfnl("send total: %6d", total);

    if (shutdown(s, SHUT_WR) < 0)
        error(1, errno, "shutdown call failed");
}

void receiver(int s)
{
    static char recv_buf[RECV_BUF_SIZE];

    int count, rlen, total = 0;

    while (1)
    {
        rlen = recv(s, recv_buf, sizeof(recv_buf), 0);
        if (rlen == 0)
            break;
        else if (rlen > 0)
        {
            total += rlen;
            if (verbose)
                printfnl("recv: %6d", rlen);
        }
        else
            error(1, errno, "recv failed");
    }
    if (verbose)
        printfnl("recv total: %6d", total);
}
