/* udp4_server.c -- UDP server (IPv4) skeleton */

#include "einet.h"

/* main */

int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in local, peer;
    int argn, s;

    init(argv[0], envp);
    argn = parse_options(argc, argv);

    s = socket(PF_INET, SOCK_DGRAM, 0); //ソケットを生成する。
    if (s < 0)
        error(1, errno, "socket call failed");

    if (set_address4(argv[argn], iname, sname, &local, "udp") == NULL) /*ソケットアドレス構造体変数（local）に以下の値（自分）を設定する（set_address4）。
                                                                         ・IPアドレス（argv[argn]）。
                                                                         ・ポート番号（sname）。*/
        error(1, errno, "set address failed");

    if (bind(s, (struct sockaddr *)&local, sizeof(local)) < 0) /*ソケットアドレス構造体をソケットに関連付ける。
                                                                          ・s：ソケットディスクリプタ。
                                                                          ・&local：自分のアドレス構造体のアドレス（ポインタ）。
                                                                          　・(struct sockaddr*)でキャストする。
                                                                          ・sizeof(local)：自分のアドレス構造体のサイズ。*/
        error(1, errno, "bind call failed");

    server(s, (struct sockaddr *)&peer, sizeof(peer)); /*サーバ関数を呼び出す。
                                                                        　・s：ソケットディスクリプタ。
                                                                        　・&peer：送信元クライアントのアドレス構造体のアドレス（ポインタ）。
                                                                        　　・(struct sockaddr*)でキャストする。
                                                                        　・sizeof(peer)：送信元クライアントのアドレス構造体のサイズ。*/

    if (close(s) < 0) //ソケットを閉じる。s：ソケットディスクリプタ。
        error(1, errno, "close call failed");
    exit(0);
}