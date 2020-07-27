/* tcp_echo_srv.c -- TCP ECHO server */

#include "einet.h"

void server(int s, struct sockaddr *peer, socklen_t plen)
{
    static char recv_buf[RECV_BUF_SIZE];

    int slen, rlen, total = 0;

    //送受信のループで無限ループする。
    while (1) /* 送受信のループ、無限ループ */
    {
        rlen = recv(s, recv_buf, sizeof(recv_buf), 0); /* データを受信 */
        //クライアントからのデータを受信する。
        //・s：ソケットディスクリプタ。
        //・recv_buf：データを受信するバッファ。
        //・sizeof(recv_buf)：受信するバッファのサイズ。
        //recv関数の戻り値rlenの値によって動作を変える。
        //・rlen == 0：送信側からコネクションを切断された。受信を終了する。
        //・rlen > 0：受信バッファ(recv_buf)の内容を表示する。
        //・rlen < 0：エラーを表示する。
        if (rlen == 0)
            break; /* クライアントから切断された */
        else if (rlen > 0)
        {
            if (verbose)
                printfnl("recv: %6d", rlen); /* 受信データを表示 */
            else if (dump)
                dump_recv((const uint8_t *)recv_buf, rlen);
        }
        else
            error(1, errno, "recv failed"); /* エラーを表示 */
        total += rlen;

        if (!discard)
        {
            if (send(s, recv_buf, rlen, 0) < 0) /* 受信データを送り返す */
                                                //クライアントに受信したデータをエコーバックする。
                                                //・s：ソケットディスクリプタ。
                                                //・recv_buf：受信したデータが入ったバッファ。
                                                //・rlen：受信データの長さ。
                error(1, errno, "send failed"); /* エラーを表示 */
        }
        //sleep(1);
    }
    //コネクションを切断する。
    if (shutdown(s, SHUT_WR) < 0) /* コネクションを切断 */
        error(1, errno, "shutdown call failed");
    if (silent == 0)
        printfnl("recv total: %6d", total);
}