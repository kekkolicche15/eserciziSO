#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

struct args{
	char *D, *F;
};

void handle_events(int fd, int *wd, struct args arg){
	char buf[4096];
	const struct inotify_event *evt;
	ssize_t len = read(fd, buf, sizeof(buf));
	evt = (const struct inotify_event*)buf;
	FILE *outpt = fopen(arg.F, "a+");
	fwrite(evt->name, 1, strlen(evt->name), outpt); //copy name
	putc('\n', outpt);
	putc('\t', outpt);
	fflush(outpt);
	char *newfile_name = malloc(strlen(evt->name)+strlen(arg.D)+2);
	strcpy(newfile_name, arg.D);
	strcat(strcat(newfile_name, "/"), evt->name);
	struct stat st;
	stat(newfile_name, &st);
	if(st.st_mode & S_IXUSR){
		int status;
		if(fork()==0){
			char *cmd = malloc(3+strlen(newfile_name));
			strcpy(cmd, "./");
			strcat(cmd, newfile_name);
			char* ex_argv[] = {newfile_name, NULL};
			dup2(fileno(outpt), STDOUT_FILENO);
			execvp(ex_argv[0], ex_argv);
			_exit(0);
		}
		else{
			wait(&status);
			unlink(newfile_name);
		}
	}
	else{
		FILE *newfile = fopen(newfile_name, "r");
		int b;
		while((b=fgetc(newfile))!=EOF){
			fputc(b, outpt);
			if(b=='\n')
				putc('\t', outpt);
		}
		putc('\n', outpt);
		unlink(newfile_name);
		fclose(newfile);
	}
	fclose(outpt);
}
int main(int argc, char* argv[]){
	char buf;
	int fd, poll_num, wd;
	nfds_t nfds;
	struct pollfd fds;

	if (argc < 3) {
		printf("Usage: %s PATH FILE\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	fd = inotify_init1(IN_NONBLOCK);
	if (fd == -1) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}
	wd = inotify_add_watch(fd, argv[1], IN_ONLYDIR|IN_CREATE|IN_MOVED_TO);
	nfds = 1;
	fds.fd = fd;
	fds.events = POLLIN;
	while (1) {
		poll_num = poll(&fds, nfds, -1);
		if (poll_num == -1) {
			if (errno == EINTR)
				continue;
			perror("poll");
			exit(EXIT_FAILURE);
		}
		if (poll_num > 0 && fds.revents & POLLIN) {
			struct args arg;
			arg.D = malloc(strlen(argv[1])+1);
			arg.F = malloc(strlen(argv[2])+1);
			strcpy(arg.D, argv[1]);
			strcpy(arg.F, argv[2]);
			handle_events(fd, &wd, arg);
		}
	}
	printf("Listening for events stopped.\n");
	close(fd);
	free(&wd);
	exit(EXIT_SUCCESS);
}
