/* tcp_echo_cli2.c -- TCP ECHO client (2 process) */

#include "einet.h"

void sender(int s)
{
    static char send_buf[SEND_BUF_SIZE];

    char *ptr;
    int pat, count, slen, total = 0;

    ptr = send_buf; /* 送信パターン作成 */
    for (count = num_rep; count-- > 0;)
        for (pat = PAT_BEGIN; pat <= PAT_END; pat++)
            *ptr++ = pat;

    for (count = num_echo; count-- > 0;) /* 送信のループ */
    {
        slen = send(s, send_buf, (PAT_END - PAT_BEGIN + 1) * num_rep, 0);
        if (slen >= 0) /* 送信 */
        {
            total += slen;
            if (verbose)
                printfnl("send: %6d", slen); /* 送信データ数表示 */
        }
        else
            error(1, errno, "send failed");
    }
    if (silent == 0)
        printfnl("send total: %6d", total);

    if (shutdown(s, SHUT_WR) < 0)                /* コネクション切断 */
        error(1, errno, "shutdown call failed"); /* エラー表示 */
}

void receiver(int s)
{
    static char recv_buf[RECV_BUF_SIZE];

    int count, rlen, total = 0;

    while (1) /* 受信のループ */
    {
        rlen = recv(s, recv_buf, sizeof(recv_buf), 0); /* 受信 */
        if (rlen == 0)
            break; /* サーバから切断された */
        else if (rlen > 0)
        {
            total += rlen;
            if (verbose)
                printfnl("recv: %6d", rlen); /* 受信データ数を表示 */
        }
        else
            error(1, errno, "recv failed");
    }
    if (verbose)
        printfnl("recv total: %6d", total);
}
void client(int s, struct sockaddr *peer, socklen_t plen) 60:
{
    int pid;
    pid = fork();
    if (pid < 0)
        error(1, errno, "fork");
    else if (pid == 0) /* child */
        sender(s);
    else
        receiver(s);
    //fork関数の戻り値pidの値によって動作を変える。
    //・pid < 0：エラー
    //・pid == 0：子プロセス、送信関数senderを呼び出す。
    //・pid > 0：親プロセス、受信関数receiverを呼び出す。
}
