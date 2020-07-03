# Makefile for TCP/IP samples

UDP_CLI1_OBJS =	udp4_client.o \
		udp_echo_cli1.o

UDP_CLI2_OBJS =	udp4_client.o \
		udp_echo_cli2.o

UDP_SRV_OBJS =	udp4_server.o \
		udp_echo_srv.o

TCP_CLI1_OBJS =	tcp4_client.o \
		tcp_echo_cli1.o

TCP_CLI2_OBJS =	tcp4_client.o \
		tcp_echo_cli2.o

TCP6_CLI2_OBJS =	tcp6_client.o \
		tcp_echo_cli2.o

TCP64_CLI2_OBJS = \
		tcp64_client.o \
		tcp_echo_cli2.o

TCP_SRV1_OBJS =	tcp4_server1.o \
		tcp_echo_srv.o

TCP_SRV2_OBJS =	tcp4_server2.o \
		tcp_echo_srv.o

TCP_SRV3_OBJS =	tcp4_server3.o \
		tcp_echo_srv.o

TCP6_SRV3_OBJS =	tcp6_server3.o \
		tcp_echo_srv.o

TCP64M_SRV3_OBJS = \
		tcp64m_server3.o \
		tcp_echo_srv.o

WWWS4_OBJS =	tcp4_server3.o \
		wwws.o

WWWS6_OBJS =	tcp6_server3.o \
		wwws.o

COMOBJS  = einet.o
COM4OBJS = set_addr4.o
COM6OBJS = set_addr6.o

INSTALL_DIR = /usr/local/bin

all:	\
	tcp4_echo_cli1	tcp4_echo_srv1 \
	tcp4_echo_cli2	tcp4_echo_srv2 tcp4_echo_srv3 \
	tcp6_echo_srv3	\
	tcp64_echo_cli2	tcp64m_echo_srv3 \
	wwws6 wwws4 \
	udp4_echo_srv 	udp4_echo_cli1	udp4_echo_cli2

clean:
	rm -fr	*.o \
		tcp4_echo_cli1	tcp4_echo_srv1 \
		tcp4_echo_cli2	tcp4_echo_srv2 \
		tcp6_echo_cli2	tcp6_echo_srv2 \
		tcp64_echo_cli2	tcp64m_echo_srv2 \
		wwws6 wwws4 \
		udp4_echo_srv 	udp4_echo_cli1	udp4_echo_cli2

install:
	cp tcp4_echo_srv2 tcp6_echo_srv2 $(INSTALL_DIR)

udp4_echo_cli1: $(COMOBJS) $(COM4OBJS) $(UDP_CLI1_OBJS)
	cc -o udp4_echo_cli1 $(COMOBJS) $(COM4OBJS) $(UDP_CLI1_OBJS)

udp4_echo_cli2: $(COMOBJS) $(COM4OBJS) $(UDP_CLI2_OBJS)
	cc -o udp4_echo_cli2 $(COMOBJS) $(COM4OBJS) $(UDP_CLI2_OBJS)

udp4_echo_srv: $(COMOBJS) $(COM4OBJS) $(UDP_SRV_OBJS)
	cc -o udp4_echo_srv $(COMOBJS) $(COM4OBJS) $(UDP_SRV_OBJS)

tcp4_echo_cli1: $(COMOBJS) $(COM4OBJS) $(TCP_CLI1_OBJS)
	cc -o tcp4_echo_cli1 $(COMOBJS) $(COM4OBJS) $(TCP_CLI1_OBJS)

tcp4_echo_cli2: $(COMOBJS) $(COM4OBJS) $(TCP_CLI2_OBJS)
	cc -o tcp4_echo_cli2 $(COMOBJS) $(COM4OBJS) $(TCP_CLI2_OBJS)

tcp6_echo_cli2: $(COMOBJS) $(COM6OBJS) $(TCP6_CLI2_OBJS)
	cc -o tcp6_echo_cli2 $(COMOBJS) $(COM6OBJS) $(TCP6_CLI2_OBJS)

tcp64_echo_cli2: $(COMOBJS) $(COM4OBJS) $(COM6OBJS) $(TCP64_CLI2_OBJS)
	cc -o tcp64_echo_cli2 $(COMOBJS) $(COM4OBJS) $(COM6OBJS) $(TCP64_CLI2_OBJS)

tcp4_echo_srv1: $(COMOBJS) $(COM4OBJS) $(TCP_SRV1_OBJS)
	cc -o tcp4_echo_srv1 $(COMOBJS) $(COM4OBJS) $(TCP_SRV1_OBJS)

tcp4_echo_srv2: $(COMOBJS) $(COM4OBJS) $(TCP_SRV2_OBJS)
	cc -o tcp4_echo_srv2 $(COMOBJS) $(COM4OBJS) $(TCP_SRV2_OBJS)

tcp4_echo_srv3: $(COMOBJS) $(COM4OBJS) $(TCP_SRV3_OBJS)
	cc -o tcp4_echo_srv3 $(COMOBJS) $(COM4OBJS) $(TCP_SRV3_OBJS)

tcp6_echo_srv2: $(COMOBJS) $(COM6OBJS) $(TCP6_SRV2_OBJS)
	cc -o tcp6_echo_srv2 $(COMOBJS) $(COM6OBJS) $(TCP6_SRV2_OBJS)

tcp6_echo_srv3: $(COMOBJS) $(COM6OBJS) $(TCP6_SRV3_OBJS)
	cc -o tcp6_echo_srv3 $(COMOBJS) $(COM6OBJS) $(TCP6_SRV3_OBJS)

tcp64m_echo_srv3: $(COMOBJS) $(COM6OBJS) $(TCP64M_SRV3_OBJS)
	cc -o tcp64m_echo_srv3 $(COMOBJS) $(COM6OBJS) $(TCP64M_SRV3_OBJS)

wwws4: $(COMOBJS) $(COM4OBJS) $(WWWS4_OBJS)
	cc -o wwws4 $(COMOBJS) $(COM4OBJS) $(WWWS4_OBJS)

wwws6: $(COMOBJS) $(COM6OBJS) $(WWWS6_OBJS)
	cc -o wwws6 $(COMOBJS) $(COM6OBJS) $(WWWS6_OBJS)

set_addr4.o: einet.h econfig.h
set_addr6.o: einet.h econfig.h
init.o: einet.h econfig.h
einet.o: einet.h econfig.h
rline.o: einet.h econfig.h
sfield.o: einet.h econfig.h

udp4_client.o: einet.h econfig.h
udp6_client.o: einet.h econfig.h
#udp64_client.o: einet.h econfig.h
#udp64_client3.o: einet.h econfig.h
udp_echo_cli.o: einet.h econfig.h

udp4_server.o: einet.h econfig.h
udp_echo_srv.o: einet.h econfig.h

tcp4_client.o: einet.h econfig.h
tcp_echo_cli1.o: einet.h econfig.h
tcp_echo_cli2.o: einet.h econfig.h

tcp6_client.o: einet.h econfig.h
tcp64_client.o: einet.h econfig.h

tcp4_server1.o: einet.h econfig.h
tcp4_server2.o: einet.h econfig.h
tcp4_server3.o: einet.h econfig.h
tcp_echo_srv.o: einet.h econfig.h

tcp6_server1.o: einet.h econfig.h
tcp6_server2.o: einet.h econfig.h
tcp6_server3.o: einet.h econfig.h
tcp64m_server3.o: einet.h econfig.h

wwws.o: einet.h econfig.h
