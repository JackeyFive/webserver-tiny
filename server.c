#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>  
#include<errno.h>  
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>

#define MAX 1024

void request_error();
void static_page();
void dy_page(int a,int b);
void readline(int fd,char* buf);
void distinguish();

int fd_n,newfd;
int pid;
char bufin[MAX];
char bufout[MAX];

void main(){

struct sockaddr_in ad_in;


char *addr="127.0.0.1";
char *client="client:";

memset(&ad_in,0,sizeof(ad_in));

if(inet_pton(AF_INET,(char *)addr,(void *)&(ad_in.sin_addr))<1){
printf("htn");exit(0);
}	
		
if((fd_n=socket(AF_INET,SOCK_STREAM,0))<0){
	printf("socket");exit(0);}

ad_in.sin_family = AF_INET;
//ad_in.sin_addr.s_addr = htonl(INADDR_ANY);
ad_in.sin_port=htons(8000);




if( bind(fd_n, (struct sockaddr*)&ad_in, sizeof(ad_in)) <0){
printf("bind");exit(0);
}

if( listen(fd_n, 10) == -1){
printf("listen");exit(0);
}


while(1)
{
	struct sockaddr_in conad_in;
	size_t len=sizeof(conad_in);
	memset(&conad_in,0,len);
	conad_in.sin_family = AF_INET;

//	biggest leession:if(newfd=accept(fd_n, (struct sockaddr*)NULL, NULL)<0)
if((newfd = accept(fd_n, (struct sockaddr*)&conad_in, (socklen_t*)&len))<0){
			printf("accept");exit(0);}

	if(!fork())
	{
	char http_buf[MAX];
//printf("hehe");
	readline(newfd,http_buf);

	if(strstr(http_buf,"GET")==NULL)
		request_error();
	else	if(strstr(http_buf,"haha")!=NULL)
		static_page();
	else  if(strstr(http_buf,"add")!=NULL)
		{
		strtok(http_buf,"?");
				
		char* arg[2];
		arg[0]=strtok(NULL,"&");
		arg[1]=strtok(NULL,"&");
//printf("%s\n%s",arg[0],arg[1]);	
			dy_page( atoi(arg[0]),atoi(arg[1]) );
		}
		
	else 	request_error();
	
	close(newfd);
	close(fd_n);
	exit(0);

	}
	sleep(1);
	close(newfd);
}

	close(fd_n);
	exit(0);
}

void dy_page(int one,int two){
	int result=one+two;
	int siz_temp;
	int siz_itoa;

	char itoa[4];
	
	itoa[0]=(char)(((result%10000)/1000)+48);
	itoa[1]=(char)(((result%1000)/100)+48);
	itoa[2]=(char)(((result%100)/10)+48);
	itoa[3]=(char)(result%10+48);
	
//printf("%s",itoa);
	char* temp="Result is : ";

	siz_temp=strlen(temp);
	siz_itoa=sizeof(itoa);
//printf("%d",siz_itoa);

	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");
	sprintf(buf,"%sContent-length: %d\r\n",buf,siz_temp+siz_itoa);
	strcat(buf,"Content-type: text/html\r\n\r\n");
	strncat(buf,temp,siz_temp);
	strncat(buf,itoa,siz_itoa);
	strncat(buf,"\0",1);
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}

}


void request_error(){
	char* temp="error";

	int siz_temp=strlen(temp);
	
	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");
	sprintf(buf,"%sContent-length: %d\r\n",buf,siz_temp);
	strcat(buf,"Content-type: text/html\r\n\r\n");
	strncat(buf,temp,siz_temp);
	strncat(buf,"\0",1);
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}

}

void static_page(){

	int fil;
	fil=open("/home/nice/桌面/web server tiny/haha.html",O_RDONLY,0);
	
	struct stat sta;
	fstat(fil,&sta);
	size_t siz=sta.st_size;
	
	char buffil[MAX];
	read(fil,buffil,siz);
//write(1,buffil,siz);
	
	char buf[MAX];

	strcpy(buf,"HTTP/1.0 200 OK\r\n");
	strcat(buf,"Connection: close\r\n");

	sprintf(buf,"%sContent-length: %d\r\n",buf,siz); //strcat(buf,"Content-length: 14\r\n");

	strcat(buf,"Content-type: text/html\r\n\r\n");
	strncat(buf,buffil,siz);
	strncat(buf,"\0",1);
	
	char *ch=buf;	
	while((*ch)!='\0')
	{
	write(newfd,ch,1);
	ch++;
	}

	close(fil);

}



void readline(int fd,char* buf)
{	
	char ch;
	read(fd,&ch,1);
	while((ch)!='\n')
	{
	memcpy(buf,&ch,1);
	buf++;
	read(fd,&ch,1);
	
	}
	strncat(buf,"\0",1);
}

