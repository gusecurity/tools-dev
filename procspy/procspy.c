/*
 * procspy: Monitor processes for all users on Linux
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include "dynarr.h"

void getproc(char *pid, char *buf, size_t buf_size)
{
	int fd;
	size_t l;
	char *ptr;

	snprintf(buf, buf_size, "/proc/%s/cmdline", pid);
	fd = open(buf, O_RDONLY);
	if (-1 == fd) {
		perror(buf);
		return;
	}
	l = read(fd, buf, buf_size);
	close(fd);
	if (l) {
		for (ptr = buf; ptr < buf + l - 1; ++ptr) {
			if (!*ptr)
				*ptr = ' ';
		}
	}
	else {
		snprintf(buf, buf_size, "/proc/%s/comm", pid);
		fd = open(buf, O_RDONLY);
		if (-1 == fd) {
			perror(buf);
			return;
		}
		l = read(fd, buf, buf_size);
		buf[l - 1] = 0;
		close(fd);
	}
}

static dynarr pids;

void getprocs()
{
	DIR *proc;
	struct dirent *ent;
	char buf[PATH_MAX];
	pid_t *cur, pid;

	proc = opendir("/proc");
	while (ent = readdir(proc)) {
		if (!isdigit(ent->d_name[0]))
			continue;

		pid = strtol(ent->d_name, NULL, 10);
		for (cur = dynarr_ptr(&pids, 0); cur <
				(pid_t *) pids.buffer + pids.elem_cnt; ++cur) {
			if (pid == *cur) {
				goto skip_proc;
			}
		}

		dynarr_add(&pids, 1, &pid);
		getproc(ent->d_name, buf, sizeof(buf));
		printf("%d\t%s\n", pid, buf);
skip_proc:;
	}
	closedir(proc);
}

int main()
{
	pid_t t = 15;

	setvbuf(stdout, NULL, _IONBF, 0);
	dynarr_alloc(&pids, sizeof(pid_t));
	for (;;) {
		getprocs();
		usleep(100000);
	}
	dynarr_free(&pids);
}
