/* udp4_client.c -- UDP (IPv4) client skeleton */

#include "einet.h"

/* main */
int main(int argc, char **argv, char **envp){
    struct sockaddr_in peer;
    int s, argn;

    init(argv[0], envp);
    argn = parse_options(argc, argv);

    s = socket(PF_INET, SOCK_DGRAM, 0); //ソケットを生成する。
    if(s < 0){
        error(1, errno, "socket call failed");
    }
    
    if(set_address4(argv[argn], iname, sname, &peer, "udp") == NULL){ //ソケットアドレス構造体変数（peer）に以下の値（相手）を設定する（set_address4）。
                                                                      //・IPアドレス（argv[argn]）。
                                                                      //・ポート番号（sname）。
        error(1, errno, "set address failed");
    }
    
    print_peer_name((struct sockaddr *)&peer);

    client(s, (struct sockaddr *)&peer, sizeof(peer)); //クライアント関数を呼び出す。

    if(close(s) < 0){ //ソケットを閉じる（close）。s：ソケットディスクリプタ。
        error(1, errno, "close call failed");
    }
    exit(0);
}