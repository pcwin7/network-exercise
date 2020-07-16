/* tcp4_server3.c -- TCP server (IPv4) skeleton */

#include "einet.h"

/* catch signal */
void sig_child(int signo) //子プロセスの終了状態を取得する関数。
{
    int status;

    waitpid(-1, &status, WNOHANG);
}

/* main */
int main(int argc, char **argv, char **envp)
{
    struct sockaddr_in local, peer;
    socklen_t plen;
    int argn, s, p, pid;

    init(argv[0], envp);
    argn = parse_options(argc, argv);

    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
        error(1, errno, "socket call failed");
    if (set_address4(argv[argn], iname, sname, &local, "tcp") == NULL)
        error(1, errno, "set address failed");
    if (bind(s, (struct sockaddr *)&local, sizeof(local)) < 0)
        error(1, errno, "bind call failed");
    if (listen(s, NLISTEN) < 0) //接続待ちキューの長さ(NLISTEN)を設定し、受動オープンする。
                                //・s：ソケットディスクリプタ。
                                //・NLISTEN：接続待ちキューの長さ
                                //接続待ちは次のaccept関数で行い、listen関数はすぐ戻ってくる。
        error(1, errno, "listen call failed");
#if 1
    if (signal(SIGCHLD, sig_child) == SIG_ERR) //子プロセスの終了シグナルを捕捉する関数(sig_child)を登録する。
        error(1, errno, "signal call failed");
#endif
    plen = sizeof(peer);
    while (1) //コネクション確立要求待ちのループ
              //・コネクションの確立要求を待つ(accept)。
              //　・必ず戻り値のソケットディスクリプタ(p)を、以降の処理に使用する。
              //・サーバプログラムを呼び出す(server)。
              //　・引数は、acceptの戻り値のソケットディスクリプタ
              //・ソケットを閉じる(close)。
              //　・引数は、acceptの戻り値のソケットディスクリプタ
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