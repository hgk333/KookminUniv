#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

char quit[] = "exit";

void error(char *msg)
{
	fprintf(stderr,"%s: %s\n",msg, strerror(errno));
	exit(1);
}

int open_listener_socket(){

	int s = socket(PF_INET,SOCK_STREAM,0);	//server socket

	if(s == -1)
		error("can not open server socket");

	return s;
}

void bind_to_port(int socket, int port){
	struct sockaddr_in name; //server port config

	name.sin_family = AF_INET;
	name.sin_port = (in_port_t)htons(30000);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int reuse = 1;

	if(setsockopt(socket, SOL_SOCKET,SO_REUSEADDR,(char *) &reuse, sizeof(int)) == -1)
		error("can not use socket\n");


	int c = bind(socket, (struct sockaddr *) &name, sizeof(name)); // binding = select server port

	if(c == -1)
		error("can not binding"); 

}
int say(int socket, char *msg){ // send msg

	write(socket,msg,strlen(msg));
}


int read_in(int socket, char *buf, int len){

	int str_len =0;
	while((str_len = read(socket,buf,len)) !=0){ // read server msg
		if(strncmp(buf, quit, 4) == 0) { // quit == buf
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
		if(read_in(socket,buf,255)) // read msg  return value = 1 -> buf = quit
			break;
	}

}
void* say_thread(void* d_sock){

	int socket = (int)d_sock;
	char buf[256];
	while(socket != -1){
		if(fgets(buf,sizeof(buf),stdin)!="s") //send msg
			say(socket,buf);
		if(strncmp(buf, quit, 4) == 0) {   //quit == buf
			printf("bye!");
			break;
		}
	}
	
}

void handle_shutdown(int socket){

	if(socket)
		close(socket);

	fprintf(stderr,"byebye!!\n");
	exit(0);

}

int main(void){
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);
	int listener_d = open_listener_socket();	//server socket
	char buf[256];
	int connect_d;
	void* result;
	pthread_t r_thread;
	pthread_t w_thread;

	bind_to_port(listener_d,30000);

	if(listen(listener_d, 10) == -1)  // queue size 10
		error("can not open listener");

	connect_d = accept(listener_d,(struct sockaddr *)&client_addr,&address_size); // server wait client

	if(connect_d == -1)
		error("can not open client socket");
	if(pthread_create(&r_thread,NULL,read_thread,(void*) connect_d) ==-1)
		error("can not create read thread");
	if(pthread_create(&w_thread,NULL,say_thread,(void*) connect_d) ==-1)
		error("can not create say thread");

	pthread_join(r_thread,&result);
	pthread_join(w_thread,&result);

	close(connect_d);

	handle_shutdown(listener_d);

	return 0;
}
