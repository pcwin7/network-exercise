/* set_addr4.c -- set IPv4 address and port # */

#include "einet.h"

struct sockaddr_in *
set_address4(char *hname, char *iname, char *sname, struct sockaddr_in *sap, char *protocol)
{
    struct servent *sp;
    struct hostent *hp;
    char *endptr;
    short port;

    bzero(sap, sizeof(*sap));  //必ず、全体を0でクリアする。
    sap->sin_family = AF_INET; //アドレスファミリー(sin_family)にAF_INET(IPv4)を設定する。
#ifndef CYGWIN
    sap->sin_len = sizeof(*sap); //サイズ(sin_len)に構造体のサイズを設定する。
#endif

    if (hname == NULL)
    {                                             //実行例：% ./tcp4_echo_srv2 &
        sap->sin_addr.s_addr = htonl(INADDR_ANY); //<host>の省略時、hnameはNULLが設定されている。sap->sin_addrにhtonl(INADDR_ANY)を設定する。INADDR_ANYは0に設定されている。
    }
    else
    {
        if (!inet_pton(AF_INET, hname, &sap->sin_addr))
        { //実行例：% ./tcp4_echo_cli3 172.25.8.142　解説：<host>にリテラル(数値)を指定した時、hnameは、リテラル(数値)の文字列が設定されている。inet_pton関数でIPv4アドレスに変換し、sap->sin_addrに直接設定する。inet_ptonの戻り値で、1なら変換成功。0なら変換不成功。
            hp = gethostbyname2(hname, AF_INET);
            　 //実行例：% ./tcp4_echo_cli3 salem 解説：<host>にホスト名を指定した時、hnameは、ホスト名の文字列が設定されている。gethostbyname2関数でIPv4アドレスに変換し、sap->sin_addrにhp->h_addrを設定する。戻り値は10行目のstruct hostent構造体へのポインタ。NULLの時は、エラー、ホスト名を解決できなかった。
                if (hp == NULL)
            {
                error(0, 0, "unknown IPv4 host: %s", hname);
                return NULL;
            }
            memcpy((char *)&sap->sin_addr, (char *)hp->h_addr, hp->h_length);
        }
    }

    if (sname == NULL)
    {                                   //実行例：% ./tcp4_echo_srv2 &
        sap->sin_port = htons(PORT_NO); //解説：-p <port>の省略時、snameがNULLに設定されている。sap->sin_portにhtons(PORT_NO)を設定する。PORT_NOは(9500 + 出席番号)に設定されている。
    }
    else
    {
        port = strtol(sname, &endptr, 0); //実行例：% ./tcp6_echo_cli3 -p 7 winston
        if (*endptr == 0)
        {                                //解説：・-p <port>にリテラル(数値)を指定した時、snameは、リテラル(数値)の文字列が設定されている。
            sap->sin_port = htons(port); //　　　・strtol()でポート番号に変換し、sap->sin_portにhtons(port)を設定する。
        }
        else
        {
            sp = getservbyname(sname, protocol); //実行例：% ./tcp6_echo_cli3 -p echo tarheel 解説：-p <port>にサービス名を指定した時、snameには、サービス名の文字列が設定されている。getservbyname()でポート番号に変換し、sap->sin_portにsp->s_portを設定する。
            if (sp == NULL)
            {
                error(0, 0, "unknown service: %s", sname);
                return NULL;
            }
            sap->sin_port = sp->s_port;
        }
    }

    return sap;
}