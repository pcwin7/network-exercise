/* udp_echo_cli1.c -- UDP ECHO client with out time out */

#include "einet.h"

/* catch signal */

void sig_alarm(int signo){
}

void client(int s, struct sockaddr *peer, socklen_t plen){
    static char send_buf[SEND_BUF_SIZE];
    static char recv_buf[RECV_BUF_SIZE];

    struct sigaction siga;
    struct tm *now;
    time_t dummy;
    int slen, rlen, er;

    siga.sa_handler = sig_alarm;
    siga.sa_flags = 0;
    if(sigaction(SIGALRM, &siga, 0) < 0){           //タイムアウト処理関数を登録する。
        error(1, errno, "sigaction call failed");
    }

    while(fgets(send_buf, sizeof(send_buf), stdin) != NULL){ //送受信のループ、標準入力から送信データを入力する。fgets()の戻り値がNULLなら入力終了(CTRL-D)である。
        slen = strlen(send_buf);
        if((slen > 0) && (*(send_buf + slen - 1) == LINETERM)){
            *(send_buf + slen - 1) = 0;
            slen --;
        }

        if(sendto(s, send_buf, slen, 0, peer, plen) < 0){ //データを送信する。s:ソケットディスクリプタ。send_buf:送信データ。peer:送信先(サーバ)のアドレス構造体。plen:アドレス構造体(peer)のサイズ。
            error(1, errno, "sendto failed");
        }

#if 0
        alarm(5);           //タイムアウトを設定する。
#endif
        rlen = recvfrom(s, recv_buf, sizeof(recv_buf), 0, peer, &plen);
        alarm(0);           //タイムアウトを解除する。
        time(&dummy);
        if(rlen >= 0){      //全域変数errnoの値がEINTRなら、タイムアウトを表示する。
            fprintf(stderr, "%02d:%02d:%02d",
                            now->tm_hour, now->tm_min, now->tm_sec);
            write(fileno(stdout), recv_buf, rlen); //受信したデータを表示する。
            putnl();
        }else if(errno == EINTR){
            fprintfnl(stderr, "%02d:%02d:%02d receive time out",
                            now->tm_hour, now->tm_min, now->tm_sec);
        }else{
            error(1, errno, "recvfrom failed");
        }
    }
}