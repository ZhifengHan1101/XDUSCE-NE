#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <libgen.h>		//basename():从路径中获取文件名及后缀
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include "md5.h"

#define MAXDATASIZE 1024
#define PORT 3000
#define BACKLOG 5

#define HOST "localhost"
#define USERNAME "myQQ_info"
#define PASSWORD "string_password"
#define DATABASE "myQQ_users"

int FLAG=0;
int flag0=0;
char temp1[MAXDATASIZE];
//temp1登陆后要么在delete里要么在logout里，run一次c2 只能用一次
//我求求他成功

unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                         
void MD5Init(MD5_CTX *context)
{
     context->count[0] = 0;
     context->count[1] = 0;
     context->state[0] = 0x67452301;
     context->state[1] = 0xEFCDAB89;
     context->state[2] = 0x98BADCFE;
     context->state[3] = 0x10325476;
}
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)
{
    unsigned int i = 0,index = 0,partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = 64 - index;
    context->count[0] += inputlen << 3;
    if(context->count[0] < (inputlen << 3))
       context->count[1]++;
    context->count[1] += inputlen >> 29;
    
    if(inputlen >= partlen)
    {
       memcpy(&context->buffer[index],input,partlen);
       MD5Transform(context->state,context->buffer);
       for(i = partlen;i+64 <= inputlen;i+=64)
           MD5Transform(context->state,&input[i]);
       index = 0;        
    }  
    else
    {
        i = 0;
    }
    memcpy(&context->buffer[index],&input[i],inputlen-i);
}
void MD5Final(MD5_CTX *context,unsigned char digest[16])
{
    unsigned int index = 0,padlen = 0;
    unsigned char bits[8];
    index = (context->count[0] >> 3) & 0x3F;
    padlen = (index < 56)?(56-index):(120-index);
    MD5Encode(bits,context->count,8);
    MD5Update(context,PADDING,padlen);
    MD5Update(context,bits,8);
    MD5Encode(digest,context->state,16);
}
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)
{
    unsigned int i = 0,j = 0;
    while(j < len)
    {
         output[j] = input[i] & 0xFF;  
         output[j+1] = (input[i] >> 8) & 0xFF;
         output[j+2] = (input[i] >> 16) & 0xFF;
         output[j+3] = (input[i] >> 24) & 0xFF;
         i++;
         j+=4;
    }
}
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)
{
     unsigned int i = 0,j = 0;
     while(j < len)
     {
           output[i] = (input[j]) |
                       (input[j+1] << 8) |
                       (input[j+2] << 16) |
                       (input[j+3] << 24);
           i++;
           j+=4; 
     }
}
void MD5Transform(unsigned int state[4],unsigned char block[64])
{
     unsigned int a = state[0];
     unsigned int b = state[1];
     unsigned int c = state[2];
     unsigned int d = state[3];
     unsigned int x[64];
     MD5Decode(x,block,64);
     FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
 FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
 FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
 FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
 FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
 FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
 FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
 FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
 FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
 FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
 FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
 FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
 FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
 FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
 FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
 FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
 
 /* Round 2 */
 GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
 GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
 GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
 GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
 GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
 GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
 GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
 GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
 GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
 GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
 GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
 GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
 GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
 GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
 GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
 GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
 
 /* Round 3 */
 HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
 HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
 HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
 HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
 HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
 HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
 HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
 HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
 HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
 HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
 HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
 HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
 HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
 HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
 HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
 HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
 
 /* Round 4 */
 II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
 II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
 II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
 II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
 II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
 II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
 II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
 II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
 II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
 II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
 II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
 II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
 II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
 II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
 II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
 II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
     state[0] += a;
     state[1] += b;
     state[2] += c;
     state[3] += d;
}

// MD5加密函数
void MMD(char encrypt[128],char end[16])
{
	int i;
	//unsigned char encrypt[] ="admin";
	unsigned char decrypt[16];    
	char s[32]={0};
	
	MD5_CTX md5;
	MD5Init(&md5);         		
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt); 
	
	for(i=0;i<16;i++)
	{
		sprintf(s,"%02x",decrypt[i]);
		strcat(end,s);
	}
	
}

// 计算文件大小的函数
int size(char file_path[256]){
	FILE *fp =NULL;
	int n = 0;
	if ((fp = fopen(file_path,"r")) == NULL)
	{
	printf("Failed\n");
	exit(0);
	}
	fseek(fp,0,2);  		// 指针：移动到文件尾部
	n = (int) ftell(fp);	// 返回指针偏离文件头的位置（即文件中字符个数）
	return n;
}

// 字符串转int型函数
int stoi(char size[256]){
	int len=strlen(size);
	int i;
	int num=0;
	for(i=0;i<len;i++){
		num=num*10;
		num=num+size[i]-48;
	}
	return num;
}

// 不能只接收服务器转发的消息，在这里就接收login后服务器发送的所有消息
void* rcv(int sockfd)
{
	int nbytes;
	char buf[MAXDATASIZE];
	char cmd[MAXDATASIZE]={0};
	char file_rname[MAXDATASIZE]={0};
	int r;
	while(FLAG==0){
		memset(buf,0,sizeof(buf));
		
		read(sockfd,buf,MAXDATASIZE);
		if(buf!=NULL)
			printf("recv message from server:%s\n",buf);
		if(flag0==1)
			strcpy(temp1,buf);

		// 这个判定只限于作为文件收方时进入
		if(!strncmp(buf,"\nfile to be received",20))
		{
			//进入到文件接收模式
			//作为收方正式的接收文件
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			printf("recv file from:%s\n",buf);
			
			//收方首先收到文件名字
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			printf("recv filename from server:%s\n",buf);
			strcpy(file_rname,"recv_");
			strcat(file_rname,buf);
			
			char file_size[256]={0};
			//收方收到文件size
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			printf("recv filesize from server:%s\n",buf);
			strcpy(file_size,buf);
			
			
			//(str)file_size->int
			int ssize=stoi(file_size);
			
			
			int write_len = 0;
			
			//创建新文件
			//O_RDWR 读、写打开;O_CREAT 若此文件不存在则创建它;O_TRUNC 如果此文件存在，而且为只读或只写成功打开，则将其长度截短为0
			int fd = open(file_rname, O_RDWR | O_CREAT | O_TRUNC,0666);
			r = read(sockfd, buf, sizeof(buf));
			write(fd, buf, r);
			write_len += r;//记录写入的字节数
			while(1){
				bzero(buf, 1024);
				
				if( r <= 0|| write_len==ssize)
				{
					printf("\n [recv-%s] receive file done\n", file_rname);
					break;					
					}
				r = read(sockfd, buf, sizeof(buf));
				//将数据写入文件	
				write(fd, buf, r);
				write_len += r;//记录写入的字节数
				//printf("%d",write_len);
			}
			
			close(fd);
			printf("file over\n");
			printf("2:Query the online list;\n3:Chat;\n4:delete;\n5:logout;\n6:send file\n");
		}
	}
	pthread_exit (NULL);
}


int main(int argc,char **argv)
{
	int sockfd,nbytes;
	int i;
	char buf[MAXDATASIZE];
	struct sockaddr_in srvaddr;
	char cmd[MAXDATASIZE];
	char temp_cmd[MAXDATASIZE];
	char pw[MAXDATASIZE];
	char temp[MAXDATASIZE];
	char temp2[MAXDATASIZE];
	char online[80]={0};
	int rc;
	
	char name[20];
	
	pthread_t tid;
	
	const char s[2] = "#";
	char*token;
	// 创建套接字（socket）
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("can't create socket\n");
		exit(1);
	}
	
	// 分配套接字地址
	bzero(&srvaddr,sizeof(srvaddr));
	srvaddr.sin_family=AF_INET;
	srvaddr.sin_port=htons(PORT);
	if(inet_aton("127.0.0.1",&srvaddr.sin_addr)==0){
		printf("addr convert error\n");
		close(sockfd);
		exit(1);
	}

	// 连接客户端（connect）
	if(connect(sockfd,(struct sockaddr *)&srvaddr,sizeof(struct sockaddr))==-1){
		printf("connect error\n");
		close(sockfd);
		exit(1);
	}
	
	// 客户端接收响应（read）
	nbytes=read(sockfd,buf,MAXDATASIZE);
	buf[nbytes]='\0';
	printf("recv message from server:%s\n",buf);
	printf("Please input your command:\n");
	printf("0:register;1:log in;2:exit\n");
	gets(cmd);
	bzero(buf,sizeof(buf));
	sprintf(buf,cmd);
	write(sockfd,buf,strlen(buf));
	printf("send message to server:%s\n", buf);
	strcpy(temp,cmd);
	if(!strcmp(temp,"0"))
	{
		printf("please input your username and password\n");
		printf("usage: username#password");
		gets(cmd);
		
		strcpy(temp1,cmd);
		token = strtok(temp1, s);strcpy(name,token);
		token = strtok(NULL, s);strcpy(pw,token);
		
		strcpy(temp_cmd,name);
		strcat(temp_cmd,"#");
		
		char EEND[16]={0};

		MMD(pw,EEND);

		strcat(temp_cmd,EEND);

		bzero(buf,sizeof(buf));
		sprintf(buf,temp_cmd);
		write(sockfd,buf,strlen(buf));
		printf("send message to server:%s\n", buf);
		
		// 接收到要么成功要么失败，登录这里失败就退出吧
		// <应该写在服务器的>成功：You have registered successfully!
		// 失败：1、wrong:格式错误；2、wrong：用户名已存在
		// 比对接受来的前5个字符是不是wrong
		// 传过来的直接输出
		
		nbytes=read(sockfd,buf,MAXDATASIZE);
		buf[nbytes]='\0';
		strcpy(temp2,buf);

		//对str1和str2中的前count个字符按字典顺序比较 
		if(!strncmp(temp2,"Wrong",5)){//输入的有误了
			printf("recv message from server:%s\n",buf);
			sleep(1);
			exit(1);
		}	
		//合法的注册
		//为了后续方便，在这里获取一下name
		
		printf("recv message from server:%s\n",buf);
	}
	else if(!strcmp(temp,"1"))
	{
		for(i=0;i<3;i++)
		{
			printf("please input your 'username#password'\n");
			gets(cmd);
			
			strcpy(temp1,cmd);
			
			token = strtok(temp1, s);strcpy(name,token);
			token = strtok(NULL, s);strcpy(pw,token);
			
			strcpy(temp_cmd,name);
			strcat(temp_cmd,"#");
			
			char EEND[16]={0};
			MMD(pw,EEND);
			strcat(temp_cmd,EEND);
			
			bzero(buf,sizeof(buf));
			sprintf(buf,temp_cmd);
			write(sockfd,buf,strlen(buf));	// 账号密码发送到服务器端了
			printf("send message to server:%s\n", buf);
		
			//为了后续方便，在这里获取一下name
			
			sleep(1);
			
			//收一个服务器的回复，成功了为right，失败为wrong
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			
			strcpy(temp1,buf);
			//判断这个temp1是right（跳出循环）还是wrong
			if(!strcmp(temp1,"right"))
				break;
			else if(!strcmp(temp1,"w_input"))
				i--;
		}
		if(i==3)
		{
			printf("The password input error accumulates three times, the program automatically exits\n");
			exit(1);
		}
		
	
	}
	
	else if(!strcmp(temp,"2"))
	{
		sleep(2);
		FLAG=1;
		exit(1);
	}
	else
	{
		sleep(1);
		nbytes=read(sockfd,buf,MAXDATASIZE);
		buf[nbytes]='\0';
		printf("recv message from server:%s\n",buf);
		exit(1);
	}		
	
	printf("USER NAME：");
	printf("%s\n",name);
	
	// 创建收消息线程
	rc=pthread_create(&tid,NULL,rcv,(void*)sockfd);	// rcv是线程要执行的代码
	if (rc) 
	{
		printf("ERORR; return code from pthread_create() is %d\n", rc);
		exit(EXIT_FAILURE);
	}
			
	sleep(2);
	// 登录完成
	//////////////////////////////////////////
	//			以下为功能部分代码
	//				登录后显示
	//			从用户端获取操作指令
	//			2 : 查询在线人数
	//			3 : 聊天
	//			4 : 注销账号
	//			5 : 退出登录
	//			6 : 文件传输
	/////////////////////////////////////////
	while(1)
	{
		bzero(buf,sizeof(buf));
		printf("\n");
		printf("Please input your command:\n");
		printf("2:Query the online list;\n3:Chat;\n4:delete;\n5:logout;\n6:send file\n");
		gets(cmd);
		bzero(buf,sizeof(buf));
		sprintf(buf,cmd);
		write(sockfd,buf,strlen(buf));
		printf("send message to server:%s\n", buf);
		strcpy(temp,cmd);
		
		/*///////////////////////////////////////////////////////////
		//	2. 查询在线人数
		///////////////////////////////////////////////////////////*/
		if(!strcmp(temp,"2"))
		{
			flag0=1;
			//此时的buf是x#a#b#c，在这里要分割字符串再输出
			sleep(1);
			strcpy(online,temp1);
			
			token = strtok(online, s);
			printf( "There are %s people online \n", token );
			while(1) 
			{
				token = strtok(NULL, s);
				if(token==NULL) break;
				printf( "%s is online \n", token );
			}			
		}
		
		/*///////////////////////////////////////////////////////////
		//	3. 聊天（文字内容发送）
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"3"))
		{
			while(1)
			{
				printf("Who do you want to chat with or quit chatting(input'###')?\n");
				gets(cmd);
				bzero(buf,sizeof(buf));
				sprintf(buf,cmd);
				write(sockfd,buf,strlen(buf));
				printf("send message to server:%s\n", buf);
				
				if(!strcmp(cmd,"###"))	break;
				printf("your msg:");
				gets(cmd);
				bzero(buf,sizeof(buf));
				sprintf(buf,cmd);
				write(sockfd,buf,strlen(buf));
				printf("send message to server:%s\n", buf);
				sleep(1);
			}
		}
		
		/*///////////////////////////////////////////////////////////
		//	4. 注销账号
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"4"))
		{
			flag0 = 1;
			// 输入name 和 pw
			// 先把数据传出去，然后接收结果，删了的话直接退出（服务端关闭线程），没删继续输入指令
			printf("please input your 'password'\n");
			gets(cmd);
			bzero(buf,sizeof(buf));
			
			strcpy(temp2,name);
			strcat(temp2,"#");
			
			char EEND[16]={0};
			MMD(cmd,EEND);
			strcat(temp2,EEND);
			
			sprintf(buf,temp2);
			write(sockfd,buf,strlen(buf));	// 账号密码发送到服务器端了
			printf("send message to server:%s\n", buf);
			
			// 失败的话字符串长度是15，如果传回来的字符串>15，就是删除成功了
			sleep(1);
			// stpcpy(temp1,buf);
			if(strlen(temp1)>15){
				FLAG=1;
				exit(1);
			}
			
		}

		/*///////////////////////////////////////////////////////////
		//	5. 退出登录
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"5"))// 把name传回去，直接exit
		{
			flag0=1;
			//name传回去
			sleep(1);
			bzero(buf,sizeof(buf));
			sprintf(buf,name);
			write(sockfd,buf,strlen(buf));
			printf("send message to server:%s\n", buf);
			
			//收到删除成功的提醒
			sleep(1);
			if(strlen(temp1)==29)
			{
				sleep(3);
				FLAG=1;
				exit(1);
			}
		}
		
		/*///////////////////////////////////////////////////////////
		//	6. 文件传输
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"6"))
		{
			printf("Who do you want to send file to ?\n");
			gets(cmd);
			bzero(buf,sizeof(buf));
			sprintf(buf,cmd);
			write(sockfd,buf,strlen(buf));// 把B的name发给服务器判断是否在线
			
			flag0=1;
			
			sleep(2);
			if(!strcmp(temp1,"ON")){
				// 作为发方正式的发送文件
				char file_path[128] = {0};//文件路径
				printf("Please enter file path: ");
				gets(file_path);
				// 从文件路径中获取文件名，如"test/a.txt" ==> "a.txt"
				char file_name[128] = {0};
				strncpy(file_name, basename(file_path), sizeof(file_name));
				
				int file_ss=size(file_path);
				char file_size[128]={0};
				// sprintf(s, "%d", 123);//把整数123打印成一个字符串保存在s中
				sprintf(file_size,"%d",file_ss);//file size----str
				
				// 打开文件
				int fd = open(file_path, O_RDWR);
				if (fd == -1)
				{
					printf("open [%s]  failed", file_path);
					return -1;
				}
				
				// 将需要上传的文件名告诉对方
				bzero(buf,sizeof(buf));
				sprintf(buf,file_name);
				write(sockfd,buf,strlen(buf));
				sleep(1);
				
				// 将需要上传的文件size告诉对方
				bzero(buf,sizeof(buf));
				sprintf(buf,file_size);
				write(sockfd,buf,strlen(buf));
				sleep(1);
				
				int ret;
				while(1){
					bzero(buf, sizeof(buf));
					ret = read(fd, buf, sizeof(buf));
					if (ret <= 0)
					{
						printf("send file[%s] succeed!!!!\n", file_name);
						break;
					}
					// 发送数据
					write(sockfd, buf, ret);
				}
				close(fd);
				
			}
			else
			{
				printf("The receiver is not online.");
				continue;
			}
			flag0=0;
			
		}
		
		else
		{
			printf("Invalid inputs\n");
		}
	}
	// 关闭套接字（close）
	close(sockfd);
	return 0;
}

