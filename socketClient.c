#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

char quit[] = "exit";

void error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

int open_socket(char *host, int port)
{

	int d_sock = socket(PF_INET,SOCK_STREAM,0);	//client socket
	int c;
	struct sockaddr_in name; //server port config
	
	memset(&name, 0, sizeof(name));

	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = inet_addr(host);

	if(d_sock == -1)
		error("can not open socket");	

	c = connect(d_sock,(struct sockaddr *) &name,sizeof(name));

	if(c == -1)
		error("can not connect socket");
	return d_sock;
}

int say(int socket, char *msg){

	write(socket,msg,strlen(msg));
}


int read_in(int socket, char *buf, int len){

	int str_len =0;
	while((str_len = read(socket,buf,len)) !=0){
		if(strncmp(buf, quit, 4) == 0) {
			printf("bye!");
			return 1;
		}
		buf[str_len]='\0';
		printf("receive: %s", buf);
	}	
	return 0;
}

void* read_thread(void* d_sock){

	int socket = (int)d_sock;
	char buf[256];
	while(socket != -1){
		if(read_in(socket,buf,255))
			break;
	}

}
void* say_thread(void* d_sock){

	int socket = (int)d_sock;
	char buf[256];
	while(socket != -1){
		if(fgets(buf,sizeof(buf),stdin)!="s")
			say(socket,buf);
		if(strncmp(buf, quit, 4) == 0) {
			printf("bye!");
			break;
		}
	}
	
}


int main(int argc, char *argv[]){

	int d_sock;
	pthread_t r_thread;
	pthread_t w_thread;
//	d_sock = open_socket(argv[1],atoi(argv[0]));
	d_sock = open_socket("127.0.0.1",30000);
	void* result;
	if(pthread_create(&r_thread,NULL,read_thread,(void*)d_sock) ==-1)
		error("can not create read thread");
	if(pthread_create(&w_thread,NULL,say_thread,(void*)d_sock) ==-1)
		error("can not create say thread");
	// thread create & excute
	pthread_join(r_thread,&result);
	pthread_join(w_thread,&result);
	// thread wait to end

	return 0;

}
