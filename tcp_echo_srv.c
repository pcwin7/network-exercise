/* tcp_echo_srv.c -- TCP ECHO server */

#include "einet.h"

void server(int s, struct sockaddr *peer, socklen_t plen)
{
    static char recv_buf[RECV_BUF_SIZE];

    int slen, rlen, total = 0;

    while (1)
    {
        rlen = recv(s, recv_buf, sizeof(recv_buf), 0);
        if (rlen == 0)
            break;
        else if (rlen > 0)
        {
            if (verbose)
                printfnl("recv: %6d", rlen);
            else if (dump)
                dump_recv((const uint8_t *)recv_buf, rlen);
        }
        else
            error(1, errno, "recv failed");
        total += rlen;

        if (!discard)
        {
            if (send(s, recv_buf, rlen, 0) < 0)
                error(1, errno, "send failed");
        }
        //sleep(1);
    }
    if (shutdown(s, SHUT_WR) < 0)
        error(1, errno, "shutdown call failed");
    if (silent == 0)
        printfnl("recv total: %6d", total);
}