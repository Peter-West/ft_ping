#include "ping.h"

void	ft_error(char *err)
{
	printf("%s\n", err);
	exit(EXIT_FAILURE);
}

void	init_sock(t_env *e)
{
	int	ttl = 64;

	if ((e->sockfd = socket(AF_INET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP)) < 0)
		perror("socket");
	if (setsockopt(e->sockfd, IPPROTO_IP, IP_TTL, (void*)&ttl, sizeof(ttl)) < 0)
		perror("setsockopt");
}

struct addrinfo		*addr_infos(char **argv)
{
	static struct addrinfo *res;
	struct addrinfo	hints;

	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = 0;
	hints.ai_addr = NULL;
	hints.ai_canonname = NULL;
	hints.ai_next = NULL;
	getaddrinfo(argv[1], NULL, &hints, &res);
	perror("addr_infos");
	return (res);
}

void	init_req(t_env *e)
{
	static t_icmp_req	icmpreq;
	static int			id = 1;
	static int			seq = 1;
	int					i = 0;
	int					j = 0;
	struct timeval		tv;

	// char				buf[256];
	// unsigned long		hmm;

	icmpreq.imcp_hdr.type = 8;
	icmpreq.imcp_hdr.code = 0;
	icmpreq.imcp_hdr.chksm = 0;
	icmpreq.imcp_hdr.id = id;
	icmpreq.imcp_hdr.seqnum = seq++;
	if (gettimeofday(&tv, NULL) == 0)
		// printf("gettimeofday success\n");
	// else
		// perror("gettimeofday");
	icmpreq.tv = tv;
	ft_bzero(icmpreq.data, REQ_DATASIZE);
	while (i < REQ_DATASIZE) {
		if (('a' + j) > 'z')
			j = 0;
		icmpreq.data[i] = 'a' + j;
		i++;
		j++;
	}	
	icmpreq.imcp_hdr.chksm = in_ping_cksum((uint16_t *)&icmpreq, sizeof(t_icmp_req));
	e->icmpreq = &icmpreq;
	// printf("chksum : %d\n", icmpreq.imcp_hdr.chksm);
	// hmm = ((struct sockaddr_in *)(e->res->ai_addr))->sin_addr.s_addr;
	// inet_ntop(AF_INET, &hmm, buf, 256);
	// perror("inet_ntop");
	// printf("Address : %s\n", buf);
}

void	send_req(t_env *e)
{	
	ssize_t	ret;

	ret = sendto(e->sockfd, (void*)e->icmpreq, sizeof(t_icmp_req), 0, e->res->ai_addr, e->res->ai_addrlen);
	if (ret < 0)
		perror("sendto");
	// else
		// printf("ret sendto : %zu\n", ret);
}


void	read_msg(char *read)
{
	t_ip_hdr	*iphdr;
	t_icmp_req	*reply;

	iphdr = (t_ip_hdr*)read;
	// printf("ttl : %d\n", iphdr->ttl);
	// printf("hdr_checksum : %d\n", iphdr->hdr_chksum);

	unsigned long	hmm;
	char			buf[64];

	hmm = ((struct in_addr *)&(iphdr->src_addr))->s_addr;
	inet_ntop(AF_INET, &hmm, buf, 64);
	// printf("src addr : %s\n", buf);

	reply = (t_icmp_req*)((void*)read + sizeof(t_ip_hdr));
	// printf("type : %d\n",reply->imcp_hdr.type);
	// printf("payload : %s\n", reply->data);

	struct timeval tv;
	gettimeofday(&tv, NULL);
	// printf("time usec ago %ld\n", reply->tv.tv_usec);
	// printf("time sec ago %ld\n", reply->tv.tv_sec);
	// printf("time usec now %ld\n", tv.tv_usec);
	// printf("time sec now %ld\n", tv.tv_sec);
	// // if (tv.tv_usec - reply->tv.tv_usec)
	// printf("time between : %ld\n", tv.tv_usec - reply->tv.tv_usec);

	long int	diff = (tv.tv_usec - reply->tv.tv_usec);

	if (diff < 0)
		diff = diff + 1000000;

	printf("%zu bytes from %s: icmp_seq=%d ttl=%d time=%f ms\n", ft_strlen(reply->data), 
		buf, reply->imcp_hdr.seqnum, iphdr->ttl, (float)(diff)/1000.0);
}

int		timer = 2;

void	Alarm_handler(int sig)
{
	printf("done : %d\n", sig);
	timer = 0;
}

void	alarm_sig(t_env *e)
{
	int	ret = 0;

	signal(SIGALRM, Alarm_handler);
	ret = alarm(5);
	timer = 2;		
	while (1)
	{
		if (timer == 0)
			break ;
		recv_reply(e);
	}
}

void	recv_reply(t_env *e)
{
	ssize_t			ret_recv = 0;
	struct msghdr	msg;
	struct iovec	iov;
	char			buff[BUFF_SIZE + 1];

	ft_bzero(buff, BUFF_SIZE);
	iov.iov_base = buff;
	iov.iov_len = BUFF_SIZE;
	msg.msg_name = e->res->ai_addr;
	msg.msg_namelen = sizeof(struct sockaddr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = sizeof(struct iovec);
	msg.msg_control = 0;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;
	if ((ret_recv = recvmsg(e->sockfd, &msg, 0)) > 0)
	{
		// printf("ret_recv : %zu\n", ret_recv);
		read_msg(buff);
		timer = 0;
	}
}

uint16_t in_ping_cksum(uint16_t *addr, int len)
{
	int nleft = len;
	uint16_t *w = addr;
	uint16_t answer;
	int sum = 0;
	uint16_t u = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) 
	{
		*(uint8_t *)(&u) = *(uint8_t *)w ;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

int		main(int argc, char **argv)
{
	t_env e;

	e.res = NULL;
	e.icmpreq = NULL;
	if (argc != 2)
		ft_error(ARGS);
	e.res = addr_infos(argv);
	init_sock(&e);
	while (1) {
		init_req(&e);
		send_req(&e);
		alarm_sig(&e);
		sleep(1);
	}
	return (0);
}