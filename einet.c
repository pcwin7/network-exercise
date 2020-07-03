/* einet.c -- Internet (TCP/IP) program samples */

#include "einet.h"

/* variables */

char *program_name;
char *sname	= NULL;
char *iname	= NULL;
char **cmd_argv = NULL;
int num_echo	= DEF_NUM_ECHO;
int num_rep	= DEF_NUM_REP_PAT;
int silent	= 0;
int verbose	= 0;
int conn_ipv4	= 0;
int discard	= 0;
int dump	= 0;
int binary	= 0;
int cmd_argc	= 0;
int cmd_argn	= 0;

void
error (int status, int err, char *fmt, ...)
{
	va_list	ap;

	fprintf(stderr, "%s: ", program_name);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (err)
		fprintf(stderr, ": %s (%d)\n", strerror(err), err);
	else
		fputc('\n', stderr);

	if (status)
		exit(status);
	}

/* get_value -- get value from <key>=<value> */

char *
get_value (char *env, char *key)
{
	int	ix;
	
	for (ix = 0; env[ix] == key[ix]; ix ++)
		;

	return env[ix] == '=' ? &env[ix + 1] : NULL;
	}

/* init.c -- initialize */

void
init (char *arg, char **envp)
{
	int	ix;
	char*	UserID = NULL;

	program_name = strrchr(arg, '/');
	if (program_name == NULL)
		program_name = arg;
	else
		program_name ++;

	for (ix = 0; envp[ix] != NULL; ix ++) {
		if ((UserID = get_value(envp[ix], "USER")) != NULL)
			break;
		}
	fprintf(stderr, "%s: #%02d %s\n", program_name, STUDENT_NO, UserID);
	}

/* parse options */

int parse_options (int argc, char *argv[])
{
	int	opt;

	cmd_argc = argc;
	cmd_argv = argv;
	opterr = 0;
	while ((opt = getopt(argc, argv, "bdusv4i:p:e:r:")) != EOF) {
		switch (opt) {
		case 'd':		/* discard */
			discard = 1;
			sname = "9";
			break;
		case 'u':		/* dump receive data */
			dump = 1;
			break;
		case 'b':		/* dump binray data */
			binary = 1;
			dump = 1;
			break;
		case 's':		/* silent */
			silent = 1;
			break;
		case 'v':		/* verbose */
			verbose = 1;
			break;
		case '4':		/* connect via IPv4 */
			conn_ipv4 = 1;
			break;
		case 'i':		/* interface name */
			iname = optarg;
			break;
		case 'p':		/* service name */
			sname = optarg;
			break;
		case 'e':		/* number of echos */
			num_echo = atoi(optarg);
			if (num_echo > MAX_NUM_ECHO) {
				fprintf(stderr, "tcp_echo_cli: max echos = %d\n",
				                MAX_NUM_ECHO);
				num_echo = MAX_NUM_ECHO;
				}
			break;
		case 'r':		/* number of repeats */
			num_rep = atoi(optarg);
			if (num_rep > MAX_NUM_REP_PAT) {
				fprintf(stderr, "tcp_rep_cli: max repeats = %d\n",
				                MAX_NUM_REP_PAT);
				num_rep = MAX_NUM_REP_PAT;
				}
			break;
			}
		}
	return cmd_argn = optind;
	}

/* recv_line (wwws) */

int recv_line (int s, char *buf, int size)
{
	char *ptr = buf;
	int len = 0;

	while (size -- > 0 && recv(s, ptr, 1, 0) > 0 && *ptr != '\n') {
		ptr ++;
		len ++;
		}
	*ptr = '\0';
	if (len > 0) {
		len --;
		*(buf + len) = '\0';
		}
	return len;
	}

/* split_fields (wwws) */

char **split_fields (char *str, int fstrsize, int fdsize)
{
	char **fields, **fdbase;
	char *buf;

	fdbase = fields = (char **)malloc(sizeof(char *) * fdsize);
	while (fdsize -- > 0 && *str) {
		while (*str == ' ')
			str ++;
		if (*str) {
			*fields ++ = buf = (char *)malloc(sizeof(char) * fstrsize);
			while (*str && *str != ' ')
				*buf ++ = *str ++;
			*buf = '\0';
			}
		}
	*fields = NULL;
	return fdbase;
	}

/* put New Line */

void
putnl (void)
{
	fputs(NEWLINE, stdout);
	}

/* print formated line which New Line */

void
printfnl (const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	putnl();
	}

/* print formated line which New Line to discriptor */

void
fprintfnl (FILE *file, const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	vfprintf(file, format, ap);
	va_end(ap);
	putnl();
	}

/* local name (IPv6) */

void
local_name6 (int s, char *hlit, char *slit)
{
	struct sockaddr_in6	local;
	socklen_t		llen;
	int	er;

	llen = sizeof(local);
	if (getsockname(s, (struct sockaddr*)&local, &llen) < 0)
		error(1, errno, "getsockname call failed");

	er = getnameinfo((struct sockaddr *)&local, sizeof(local),
	                 hlit, NI_MAXHOST,
	                 slit, NI_MAXSERV,
	                 NI_NUMERICHOST | NI_NUMERICSERV);
	if (er < 0)
		error(1, errno, "getnameinfo call failed");
	}

/* local name (IPv4) */

void
local_name4 (int s, char *hlit, char *slit)
{
	struct sockaddr_in	local;
	socklen_t		llen;
	int	er;

	llen = sizeof(local);
	if (getsockname(s, (struct sockaddr*)&local, &llen) < 0)
		error(1, errno, "getsockname call failed");

	er = getnameinfo((struct sockaddr *)&local, sizeof(local),
	                 hlit, NI_MAXHOST,
	                 slit, NI_MAXSERV,
	                 NI_NUMERICHOST | NI_NUMERICSERV);
	if (er < 0)
		error(1, errno, "getnameinfo call failed");
	}

/* print local name (IPv6) */

void
print_local_name6 (int s)
{
	char	hlit[NI_MAXHOST], slit[NI_MAXSERV];

	local_name6(s, hlit, slit);
	printf("Local:      %s.%s\n", hlit, slit);
	}

/* print local name (IPv4) */

void
print_local_name4 (int s)
{
	char	hlit[NI_MAXHOST], slit[NI_MAXSERV];

	local_name4(s, hlit, slit);
	printf("Local:      %s.%s\n", hlit, slit);
	}

/* print local name (IPv6/IPv4) */

void
print_local_name (int s)
{
	struct sockaddr	local;
	socklen_t		llen;

	llen = sizeof(local);
	if (getsockname(s, (struct sockaddr*)&local, &llen) < 0)
		error(1, errno, "getsockname call failed");

	if (local.sa_family == AF_INET6)
		print_local_name6(s);
	else
		print_local_name4(s);
	}

/* peer name (IPv6) */

void
peer_name6 (struct sockaddr_in6 *peer, char *hlit, char *slit)
{
	int	er;

	er = getnameinfo((struct sockaddr *)peer, sizeof(*peer),
	                 hlit, NI_MAXHOST,
	                 slit, NI_MAXSERV,
	                 NI_NUMERICHOST | NI_NUMERICSERV);
	if (er < 0)
		error(1, errno, "getnameinfo call failed");
	}

/* peer name (IPv4) */

void
peer_name4 (struct sockaddr_in *peer, char *hlit, char *slit)
{
	int	er;

	er = getnameinfo((struct sockaddr *)peer, sizeof(*peer),
	                 hlit, NI_MAXHOST,
	                 slit, NI_MAXSERV,
	                 NI_NUMERICHOST | NI_NUMERICSERV);
	if (er < 0)
		error(1, errno, "getnameinfo call failed");
	}

/* print peer name (IPv6) */

void
print_peer_name6 (struct sockaddr_in6 *peer)
{
	char	hlit[NI_MAXHOST], slit[NI_MAXSERV];

	peer_name6(peer, hlit, slit);
	printf("Peer:       %s.%s\n", hlit, slit);
	}

/* print peer name (IPv4) */

void
print_peer_name4 (struct sockaddr_in *peer)
{
	char	hlit[NI_MAXHOST], slit[NI_MAXSERV];

	peer_name4(peer, hlit, slit);
	printf("Peer:       %s.%s\n", hlit, slit);
	}

/* print peer name (IPv6/IPv4) */

void
print_peer_name (struct sockaddr *peer)
{
	if (peer->sa_family == AF_INET6)
		print_peer_name6((struct sockaddr_in6 *)peer);
	else
		print_peer_name4((struct sockaddr_in  *)peer);
	}

/* dump CAN frame binary (CAN) */

void
dump_canframe_binary (const uint8_t *buff)
{
	int	DLC, len;

	/* time */
	len = sizeof(uint32_t);
	while (len --> 0)
		printf("%02x", *buff ++);
	putchar(':');

	/* ID */
	len = sizeof(uint32_t);
	while (len --> 0)
		printf("%02x", *buff ++);
	putchar(':');

	/* flags */
	printf("%02x:", *buff ++);

	/* DLC */
	len = *buff;
	printf("%02x:", *buff ++);

	/* data */
	while (len --> 0)
		printf("%02x", *buff ++);
	}

/* dump CAN binary (CAN) */

void
dump_can_binary (const uint8_t *buff, int len)
{
	static uint8_t	buffer[MAX_CAN_FRAME_SIZE * 2];
	static int	bufoff = 0;

	int	dlc, fsize, off;

	off = 0;
	while (1) {

		if (len < (sizeof(uint32_t) * 2 + 2))		/* time, ID, flags, DLC */
			break;

		dlc = *(buff + sizeof(uint32_t) * 2 + 1);	/* time, ID, flags */
		fsize = sizeof(uint32_t) * 2 + dlc + 2;		/* time, ID, flags, dlc, data */
		if (len < fsize)
			break;

		dump_canframe_binary (buff + off);
		off += fsize;
		len -= fsize;
		}

	if (len > 0) {
		memcpy(buffer + bufoff, buff + off, len);
		bufoff += len;
		}

	printf("\n\n");
	}

/* dump recv (CAN) */

void
dump_recv (const uint8_t *buff, int len)
{
	if (binary)
		dump_can_binary(buff, len);
	else {
		while (len -- > 0)
			putchar((int)*buff ++);
		}
	}
