/* set_addr6.c -- set IPv6 address, Interface ID and port # */

#include "einet.h"

struct sockaddr_in6 *
set_address6(char *hname, char *iname, char *sname,
             struct sockaddr_in6 *sap, char *protocol)
{
    struct servent *sp;
    struct hostent *hp;
    char *endptr;
    short port;

    bzero(sap, sizeof(*sap));
    sap->sin6_family = AF_INET6;
    sap->sin6_flowinfo = 0;
#ifndef CYGWIN
    sap->sin6_len = sizeof(*sap);
#endif

    if (hname == NULL)
        memset((char *)&sap->sin6_addr, 0, sizeof(sap->sin6_addr));
    else
    {
        if (!inet_pton(AF_INET6, hname, &sap->sin6_addr))
        {
            hp = gethostbyname2(hname, AF_INET6);
            if (hp == NULL)
            {
                error(0, 0, "unknown IPv6 host: %s", hname);
                return NULL;
            }
            memcpy((char *)&sap->sin6_addr, (char *)hp->h_addr,
                   hp->h_length);
        }
    }

    if (iname == NULL)
        sap->sin6_scope_id = 0;
    else
        sap->sin6_scope_id = if_nametoindex(iname);

    if (sname == NULL)
        sap->sin6_port = htons(PORT_NO);
    else
    {
        port = strtol(sname, &endptr, 0);
        if (*endptr == 0)
            sap->sin6_port = htons(port);
        else
        {
            sp = getservbyname(sname, protocol);
            if (sp == NULL)
            {
                error(0, 0, "unknown service: %s", sname);
                return NULL;
            }
            sap->sin6_port = sp->s_port;
        }
    }

    return sap;
}