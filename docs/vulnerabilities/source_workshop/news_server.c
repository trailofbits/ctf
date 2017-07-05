/*
   NYU Polytechnic Institute
   CS6573: Penetration Testing and Vulnerability Analysis
   Code Auditing Homework Assignment 

   There are a number of security holes in this network service,
   but your assignment is to only find 3.
   They could be both architectural or implementation problems.
   Look for bad logic and memory mismanagement.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define PORT 9090
#define USERNAME 0x01
#define PASSWORD 0x02
#define BADUSER "\x33\x44 BAD USERNAME!"
#define BADPASS "\x33\x45 BAD PASSWORD!"
#define READY   "\x41\x41 READY!"
#define USERPATH "./users/"
#define ARTICLEPATH "./articles/"	
#define LISTCOMMAND "ls ./articles/ > list.txt"
#define FILENOTAVAIL "\x33\x31 FILE NOT AVAILABLE!"
#define BEGINFILE "\x41\x41 BEGIN FILE: END WITH '!!!'"
#define ARTICLEWROTE "\x41\x42 ARTICLE HAS BEEN WRITTEN!"
#define LIST_ARTICLES 0x22
#define READ_ARTICLE 0x23
#define WRITE_ARTICLE 0x24
#define COMMAND 0x25
#define ADD_USER 0x26

void logData(FILE *logfile, char *format, ...);
int setupSock(FILE *logf, unsigned short port);
int writeSock(int sock, char *buf, size_t len);
int readSock(int sock, char *buf, size_t len);
void mainLoop(FILE *logf, int sock);
void handleConnection(FILE *logfile, int sock);
int userFunctions(FILE *logfile, int sock, char *user);
char *findarg(char *argbuf, char argtype);
int authenticate(FILE *logfile, char *user, char *pass);

int writeSock(int sock, char *buf, size_t len)
{
	ssize_t byteswrote = 0;
	ssize_t ret = 0;

	while (byteswrote < len)
	{
		ret = send(sock, buf + byteswrote, len - byteswrote, 0);
		
		if (ret < 0)
		{
			return -1;
		}

		if (ret == 0)
		{
			break;
		}
		
		byteswrote += ret;
	}

	return byteswrote;
}

int readSock(int sock, char *buf, size_t len)
{
	ssize_t ret = 0;
	ssize_t bytesread = 0;

	while (bytesread < len)
	{
		ret = recv(sock, buf + bytesread, len - bytesread, 0);

		if (ret == 0)
		{
			break;
		}	

		if (ret < 0)
		{
			return -1;
		}

		bytesread += ret;
	}
	
	return bytesread;	
}

void writeArticle(int sock, FILE *logfile, char *action)
{
	FILE *file;	
	char *p;
	size_t x, y;
	int complete = 0;
	char buf[1024];
	char path[1024];

	strcpy(path, ARTICLEPATH);
	strncat(path, &action[1], sizeof(path));

	logData(logfile, "user writing article: %s", path);

	file = fopen(&action[1], "w");

	if (!file)
	{
		writeSock(sock, FILENOTAVAIL, sizeof(FILENOTAVAIL));
		return;
	}

	writeSock(sock, BEGINFILE, sizeof(BEGINFILE));

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		x = readSock(sock, buf, sizeof(buf)-1);
		for (y = 0; y < x; ++y)
		{
			if (buf[y] == '!')
			{
				if (buf[y+1] == '!' && buf[y+2] == '!')
				{
					buf[y] = 0x0;
					complete = 1;
				}
			}
		}	
		fputs(buf, file);
		if (complete)
		{
			break;
		}
	}

	writeSock(sock, ARTICLEWROTE, sizeof(ARTICLEWROTE));
	fclose(file);
}


void readArticle(int sock, FILE *logfile, char *action)
{
	FILE *file;
	char buf[100];
	char path[100];

	logData(logfile, &action[1]);

	strcpy(path, ARTICLEPATH);
	strcat(path, &action[1]);

	logData(logfile, "user request to read article: %s", path);

	file = fopen(path, "r");

	if (!file)
	{
		writeSock(sock, FILENOTAVAIL, sizeof(FILENOTAVAIL));
		return;
	}
	
	/* fgets for the size of the buffer (100), from the file
	   writing the article to the user each time! */
 
	while (fgets(buf, 1000, file))
	{
		writeSock(sock, buf, strlen(buf));
	}

	fclose(file);

	return;
}

void listArticles(int sock, FILE *logfile, char *action)
{
	char buf[100];
	FILE *list;

	logData(logfile, "user has requested a list of articles");

	/* i wish i had more time! i wouldnt have to write
	   this code using system() to call things! */

	memset(buf, 0, sizeof(buf));
	system(LISTCOMMAND);

	list = fopen("list.txt", "r");

	while (fgets(buf, sizeof(buf)-1, list))
	{
		writeSock(sock, buf, strlen(buf));	
	}

	fclose(list);
	return;	
}

void command(FILE *log, int sock, char *action)
{
	logData(log, "executing command: %s", &action[1]);
	system(&action[1]);
}

void addUser(FILE *log, int sock, char *action)
{
	char *p;
	char buf[1024];

	p = strchr(&action[1], ':');

	if (!p)
	{
		return;
	}

	*p = 0x0;
	logData(log, "Adding user: %s with pass: %s", &action[1], &p[1]);	
	snprintf(buf, sizeof(buf)-1, "echo %s > %s%s.txt", &p[1], USERPATH, &action[1]);
	return;
}

int adminFunctions(FILE *logfile, int sock)
{
	char action[1024];
	size_t len;
	while (1)
	{
		writeSock(sock, READY, sizeof(READY));
		memset(action, 0, sizeof(action));
		len = readSock(sock, action, sizeof(action));	
	
		if (action[0] == ADD_USER)
		{
			addUser(logfile, sock, action);	
		}
		else if (action[0] == COMMAND)
		{
			command(logfile, sock, action);	
		}
		else
		{
			logData(logfile, "unknown action: %x", action[0]);
		}
	}

}

int userFunctions(FILE *logfile, int sock, char *user)
{
	char action[1024];
	size_t len;

	if (0 == strncmp(user, "admin", 5))
	{
		adminFunctions(logfile, sock);
		return 0;
	}
	
	while (1)
	{
		writeSock(sock, READY, sizeof(READY));
		memset(action, 0, sizeof(action));
		len = readSock(sock, action, sizeof(action));	
	
		if (action[0] == LIST_ARTICLES)
		{
			listArticles(sock, logfile, action);
		}
		else if (action[0] == READ_ARTICLE)
		{
			readArticle(sock, logfile, action);	
		}
		else if (action[0] == WRITE_ARTICLE)
		{
			writeArticle(sock, logfile, action);
		}
		else
		{
			logData(logfile, "unknown action %x", action[0]);
			return;
		}
	}

	return 0;
}

/* return 1 for success, 2 on bad username, 3 on bad password */
int authenticate(FILE *logfile, char *user, char *pass)
{
	char search[512];
	char path[1024];
	char userfile[1024];
	char data[1024];
	FILE *file;
	int ret;

	memset(path, 0, sizeof(1024));

	/* FIXME: hard coded admin backdoor for password recovery */	
	if (memcmp(pass, "baCkDoOr", 9) == 0)
	{
		return 1;
	}
	
	/* look up user by checking user files: done via system() to /bin/ls|grep user */
	logData(logfile, "performing lookup for user via system()!\n");
	snprintf(userfile, sizeof(userfile)-1, "%s.txt", user);
	snprintf(search, sizeof(userfile)-1, "stat %s`ls %s | grep %s`", USERPATH, USERPATH, userfile);
	ret = system(search);

	if (ret != 0)
	{
		return 2;
	}

	snprintf(path, sizeof(path)-1, "%s%s", USERPATH, userfile);
		
	/* open file and check if contents == password */
	file = fopen(path, "r");

	if (!file)
	{
		logData(logfile, "fopen for userfile failed\n");
		return 2;
	}

	logData(logfile, "getting userfile info\n");
	fgets(data, sizeof(data)-1, file);
	
	fclose(file);

	/* Password Check! */
	if (memcmp(data, pass, 3))
	{
		return 3;
	}

	return 1;
}

char *findarg(char *argbuf, char argtype)
{
	char *ptr1;
	char *found = NULL;
	char type = 0;
	size_t size;

	ptr1 = argbuf;

	while (1)
	{	
		memcpy((char *)&size, ptr1, 4);
		if (size == 0)
		{
			break;
		}	
		if (ptr1[4] == argtype)
		{
			found = &ptr1[5];
			break;
		}
		ptr1 += size;
	}

	return found;
}

void handleConnection(FILE *logfile, int sock)
{
	char buffer[1024];
	char argbuf[1024];
	char *user = NULL;
	char *pass = NULL;
	int len = 0;
	int ret = 0;
	size_t segloop;
	size_t segmentcount;
	size_t segnext;
	size_t argsize;
	char *ptr1;
	char *ptr2;

	/* read in data */
	memset(buffer, 0, sizeof(buffer));
	len = readSock(sock, buffer, sizeof(buffer));	
	logData(logfile, "handling connection");

	if (len == -1)
	{
		return;
	}
	
	/* parse protocol */
	ptr1 = buffer;
	ptr2 = argbuf;

	/* get count of segments */
	memcpy((char *)&segmentcount, ptr1, 4);

	logData(logfile, "Segment count is %i", segmentcount);
	
	/* make sure there aren't too many segments!
       so the count * 8(bytes) should be the max */
	if (segmentcount * 8 > sizeof(argbuf))
	{
		logData(logfile, "bad segment count");
		return;
	}

	ptr1 += 4;

	memset(argbuf, 0, sizeof(argbuf));

	for (segloop = 0; segloop < segmentcount; ++segloop)
	{
		logData(logfile, "adding segment %i", segloop+1);
		memcpy((char *)&segnext, ptr1, 4);
		logData(logfile, "next segment offset %i", segnext);
		ptr1 += 4;
		memcpy((char *)&argsize, ptr1, 4);	
		logData(logfile, "argsize: %i", argsize);
		memcpy(ptr2, ptr1, argsize);
		ptr2 += argsize;
		ptr1 += segnext;
	}

	logData(logfile, "looking up user args");

	user = findarg(argbuf, USERNAME);
	pass = findarg(argbuf, PASSWORD);

	snprintf(buffer, sizeof(buffer)-1, "User attempting to authenticate: %s", user);
	logData(logfile, buffer);

	logData(logfile, "calling authenticate");
	ret = authenticate(logfile, user, pass);
	logData(logfile, "returned from authenticate");

	if (ret != 1)
	{
		
		if (ret == 2)
		{
			writeSock(sock, BADUSER, sizeof(BADUSER));	
		}	
	
		if (ret == 3)
		{
			writeSock(sock, BADPASS, sizeof(BADPASS));
		}

		snprintf(buffer, sizeof(buffer)-1,"user: %s failed to login with password %s", user, pass);
		logData(logfile, buffer);
		return;
	}

	logData(logfile, "user %s authenticated!", user);
	
	userFunctions(logfile, sock, user);

	return;
} 

void mainLoop(FILE *logf, int sock)
{
	int clientfd = 0;
	struct sockaddr_in client;
	socklen_t clientlen = 0;
	pid_t offspring = 0;

	memset((char *)&client, 0, sizeof(client));
	
	logData(logf, "entering main loop...");

	while (1)
	{
		clientfd = accept(sock, (struct sockaddr *)&client, &clientlen);
		if (clientfd == -1)
		{
			continue;
		}

		offspring = fork();
	
		if (offspring == -1)
		{
			continue;
		}

		if (offspring == 0)
		{
			handleConnection(logf, clientfd);
			close(clientfd);
			exit(0);
		}

		close(clientfd);
	}
}

void spawnhandler(int signumber)
{
	pid_t pid;
	int stat;

	while ((pid = waitpid(-1, &stat, WNOHANG))>0)
	{
		printf("circle of life completed for %i\n", pid);	
	}
}

int setupSock(FILE *logf, unsigned short port)
{
	int sock = 0;
	struct sockaddr_in sin;
	int opt = 0;

	if (signal(SIGCHLD, spawnhandler)== SIG_ERR)
	{
		perror("fork() spawn handler setup failed!");
		return -1;
	}
	
	memset((char *)&sin, 0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == -1)
	{
		logData(logf, "socket() failed");
		return -1;
	}

	opt = 1;

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		logData(logf,"setsockopt() failed");
		return -1;
	}

	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		logData(logf, "bind() failed");
		return -1;
	}

	if (listen(sock, 10) == -1)
	{
		logData(logf, "listen() failed");
		return -1;
	}

	return sock;
}

int main(int argc, char *argv[])
{
	int sock;
	FILE *logf;

	/* setup log file */
	logf = fopen("logfile.txt", "w"); 

	if (!logf)
	{
		perror("unable to open log file\n");
		exit(1);
	}

	/* go daemon */
	daemon(0,0);
	
	/* setup socket */
	sock = setupSock(logf, PORT);
	
	if (sock == -1)
	{
		logData(logf, "failed to setup socket, exiting");
		exit(1);
	}

	logData(logf, "intial socket setup complete");
	
	mainLoop(logf, sock);	

	/* this should never execute */
	exit(0);
}

/* printf-style data logging */
void logData(FILE *logfile, char *format, ...)
{
	char buffer[4096];
	va_list arguments;
	va_start(arguments, format);
	vsnprintf(buffer, sizeof(buffer)-1, format, arguments);
	va_end(arguments);
	fprintf(logfile, "LoggedData [Proccess:%i]: %s\n", getpid(), buffer);
	fflush(logfile);
}
