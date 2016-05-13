#ifndef PING_H
#define PING_H
#define ARGS "usage : ./ft_ping [HOST]"
#define REQ_DATASIZE	64
#define BUFF_SIZE		1024

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

typedef struct	s_ip_hdr
{
	uint8_t		vihl;
	uint8_t		type;
	uint16_t	len;
	uint16_t	id;
	uint16_t	flag_off;
	uint8_t		ttl;
	uint8_t		proto;
	uint16_t	hdr_chksum;
	struct in_addr	src_addr;
	struct in_addr	dst_addr;
}				t_ip_hdr;

typedef struct	s_icmp_hdr
{
	uint8_t		type;
	uint8_t		code;
	uint16_t	chksm;
	uint16_t	id;
	uint16_t	seqnum;
}				t_icmp_hdr;

typedef struct	s_icmp_req
{
	t_icmp_hdr	imcp_hdr;
	struct timeval	tv;
	char		data[REQ_DATASIZE];
}				t_icmp_req;

typedef struct	s_icmp_rep
{
	t_ip_hdr	ip_hdr;
	t_icmp_hdr	imcp_hdr;
	struct timeval	tv;
	char		data[REQ_DATASIZE];
	char		filler[256];
}				t_icmp_rep;

typedef struct	s_env
{
	t_icmp_req			*icmpreq;
	struct addrinfo		*res;
	int					sockfd;

}				t_env;

void	recv_reply();
uint16_t in_ping_cksum(uint16_t *addr, int len);

#endif