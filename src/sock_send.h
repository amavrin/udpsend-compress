#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

const int SBUFSIZ = 2048;
const int RBUFSIZ = 2048;
const int MSGBUFSIZ = 1024;

void error(const char *msg, int code) {
	std::cerr << "Error: " << msg << std::endl;
	exit(code);
}

class UDPSender {
	int sockfd;
	struct hostent *server;
	char msgbuf[MSGBUFSIZ];
	struct sockaddr_in serveraddr;
	int serverlen;
	int bpersec;
	struct timeval lasttime;
	struct timezone tz;
	int snooze;

	void sockInit();
	void setServerAddr(const char *, int);
	
public:
	UDPSender(const char *hostname, int port) {
		sockInit();
		setServerAddr(hostname, port);
		bpersec = 0; // no limit
		bzero(&lasttime, sizeof(lasttime));
		snooze = 100;
	}

	void setspeedlim(int bps) {
		bpersec = bps;
	}

	void throttle(int bytes) {
		if (lasttime.tv_sec == 0 && lasttime.tv_usec == 0) {
			gettimeofday(&lasttime, &tz);
			return; // first call
		}
		if (bpersec == 0) return;

		struct timeval now, then, wait;

		long usec = bytes*1000000 / bpersec; // wait so much microseconds
		wait.tv_sec = usec/1000000;
		wait.tv_usec = usec - wait.tv_sec*1000000;

		timeradd(&lasttime, &wait, &then);
		gettimeofday(&now, &tz);

		while ( timercmp(&now, &then, <) ) {
			usleep(snooze);
			gettimeofday(&now, &tz);
		}
		gettimeofday(&lasttime, &tz);
	}

	int send(char const *buf, int len) {
		throttle(len);
		serverlen = sizeof(serveraddr);
		int n = sendto(sockfd, buf, len, 0, (sockaddr*) &serveraddr, serverlen);
		if (n < 0) 
			error("ERROR in sendto", 1);
		return n;
	}
};

class UDPReceiver {
	int sockfd;
	char buf[RBUFSIZ];
	struct sockaddr_in serveraddr; /* server's addr */
	int nrec;

	void listenSockInit(int);
public:
	UDPReceiver(int listenport) {
		listenSockInit(listenport);
	}

	char *receive() {
		int n = recv(sockfd, buf, RBUFSIZ-1, 0);
		if (n < 0) error("error in recv", 1);
		nrec = n;
		return buf;
	}

	int len() {
		return(nrec);
	}
};

void UDPSender::sockInit() {
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket", 1);
}

void UDPReceiver::listenSockInit(int port) {
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket", 1);

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) error("failed to bind listen socket", 1);
}

void UDPSender::setServerAddr(const char *host, int port) {
	server = gethostbyname(host);
	if (server == NULL) {
		snprintf(msgbuf, MSGBUFSIZ, "can not resolve target: %s", host);
		error (msgbuf, 1);
	}
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(port);
}
