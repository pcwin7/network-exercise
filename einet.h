/* einet.h -- Internet (TCP/IP) program samples */

#ifndef _EINET_H_
#define _EINET_H_

/* standerd include files */

#include <limits.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stdint.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "econfig.h"

#ifdef TARGET_KERNEL_BSD

#include <sys/endian.h>

#define _NET_CFG_LITTLE_ENDIAN	1234
#define _NET_CFG_BIG_ENDIAN	4321

#if _BYTE_ORDER == _LITTLE_ENDIAN

#define _NET_CFG_BYTE_ORDER	_NET_CFG_LITTLE_ENDIAN

#elif _BYTE_ORDER == _BIG_ENDIAN

#define _NET_CFG_BYTE_ORDER	_NET_CFG_BIG_ENDIAN

#else	/* of #if _BYTE_ORDER == _LITTLE_ENDIAN */

#error "_BYTE_ORDER" expected."

#endif	/* of #if _BYTE_ORDER == _LITTLE_ENDIAN */

#else	/* of #ifdef TARGET_KERNEL_BSD */

#include <netinet/in.h>

#endif	/* of #ifdef TARGET_KERNEL_BSD */

/* definitions */

#define HTML_NEWLINE	"\r\n"
#define NEWLINE		"\n"
#define LINETERM	'\n'

#define NLISTEN		5
#define DEF_NUM_ECHO	40
#define DEF_NUM_REP_PAT	40
#define MAX_NUM_ECHO	200
#define MAX_NUM_REP_PAT	400

#define PAT_BEGIN	0x20
#define PAT_END		0x7e
#define SEND_BUF_SIZE	((PAT_END - PAT_BEGIN + 1) * MAX_NUM_REP_PAT)
#define RECV_BUF_SIZE	0x4000

#define MAX_CAN_FRAME_SIZE	(sizeof(int)*2+10)

/* variables */

extern char *program_name;
extern char *sname;
extern char *iname;
extern char **cmd_argv;
extern int num_echo;
extern int num_rep;
extern int silent;
extern int verbose;
extern int conn_ipv4;
extern int discard;
extern int dump;
extern int binary;
extern int cmd_argc;
extern int cmd_argn;

/* functions */

extern void init (char *arg, char **envp);
extern int parse_options (int argc, char *argv[]);
extern char **split_fields (char *str, int fstrsize, int fdsize);
extern int recv_line (int s, char *buf, int size);
extern void putnl (void);
extern void printfnl (const char *format, ...);
extern void fprintfnl (FILE *file, const char *format, ...);

#ifndef TARGET_KERNEL_BSD

extern void local_name6 (int s, char *hlit, char *slit);
extern void local_name4 (int s, char *hlit, char *slit);
extern void print_local_name6 (int s);
extern void print_local_name4 (int s);
extern void print_local_name (int s);
extern void peer_name6 (struct sockaddr_in6 *peer, char *hlit, char *slit);
extern void peer_name4 (struct sockaddr_in *peer, char *hlit, char *slit);
extern void print_peer_name6 (struct sockaddr_in6 *peer);
extern void print_peer_name4 (struct sockaddr_in *peer);
extern void print_peer_name (struct sockaddr *peer);
extern void dump_recv (const uint8_t *buff, int len);
extern struct sockaddr_in *set_address4 (
	char *hname, char *iname, char *sname,
	struct sockaddr_in *sap, char *protocol);
extern struct sockaddr_in6 *set_address6 (
	char *hname, char *iname, char *sname,
	struct sockaddr_in6 *sap, char *protocol);
extern void error (int status, int err, char *fmt, ...);
extern void client (int s, struct sockaddr *peer, socklen_t plen);
extern void server (int s, struct sockaddr *peer, socklen_t plen);

#endif	/* of ifndef TARGET_KERNEL_BSD */

#endif	/* of #ifndef _EINET_H_ */
