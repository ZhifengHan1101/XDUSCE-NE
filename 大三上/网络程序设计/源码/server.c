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
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <mysql.h>


#define MAXDATASIZE 1024
#define PORT 3000
#define BACKLOG 5

#define HOST "localhost"
#define USERNAME "myQQ_info"
#define PASSWORD "string_password"
#define DATABASE "myQQ_users"

MYSQL mysql; 		//mysql连接
MYSQL_FIELD *fd;  	//字段列数组
char field[32][32]; //存字段名二维数组
MYSQL_RES *res; 	//这个结构代表返回行的一个查询结果集
MYSQL_ROW column; 	//一个行数据的类型安全(type-safe)的表示，表示数据行的列
char query[150]; 	//查询语句


// 用户注册函数
bool Register(char new_name[20],char new_pw[32])
{
	memset(query, 0, sizeof query);
	// 使用sql语言，insert对应信息表示注册
    strcpy(query, "insert into user values ('"); 
	strcat(query,new_name);
	strcat(query,"',");
	strcat(query,"'");
	strcat(query,new_pw);
	strcat(query,"'");
	strcat(query,",0,0)");

	// 执行sql语句
    if (mysql_query(&mysql, query))
    {
        printf("Query failed \n");
        return false;
    }
    else
    {
        printf("Register success\n");
        return true;
    }
}

// 比较用户输入密码与server端存储密码是否一致，判断密码是否正确
bool compare(char id[20],char pw[32])
{
    mysql_query(&mysql, "set names gbk");	// 使用gbk模式编码，防止中文乱码（非常重要！！）
	memset(query, 0, sizeof query);
	strcpy(query, "select pw from user where name='");
	strcat(query, id);
	strcat(query, "';");
    //返回0 查询成功，返回1查询失败  
    if (mysql_query(&mysql, query))        //执行SQL语句  
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("query success\n");
    }
    res = mysql_store_result(&mysql);
    
    int j = mysql_num_fields(res);  // 获取列数  

    column = mysql_fetch_row(res);//column[0]就是对应用户id的密码！！！！

    if(!strcmp(column[0],pw))	// 正确返回1，错误返回0
		return 1;
    else
		return 0;
		
}

// 用户登录函数（此处指修改数据库中的状态）
void Log(bool state,int sock,char name[20])
{

	char s1[10],s2[10];
	memset(s1, 0, sizeof s1);
	memset(s2, 0, sizeof s2);
	
	sprintf(s1,"%d",state);
	sprintf(s2,"%d",sock);
	
	// 拼接query
	memset(query, 0, sizeof query);
	strcat(query,"update user set state=");
	strcat(query,s1);
	strcat(query,",data_sock=");
	strcat(query,s2);
	strcat(query," where name='");
	strcat(query,name);
	strcat(query,"';");

    if (mysql_query(&mysql, query))        //执行SQL语句  
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));

    }
    else
    {
        printf("Query success\n");

    }
}

// 在线人数查询函数
void Online(char online[80])
{	// 参数online为空字符串，在函数内更新online，最终格式为：第一个值是在线人数x，后面是id，即online="x#a#b#c"
	// 数组传过来的时候已经置空了
	// 通过数据库查询state=1的行，来查询在线用户
    
    int i=0;
    mysql_query(&mysql, "set names gbk");

    // 返回0 查询成功，返回1查询失败  
    if (mysql_query(&mysql, "select name from user where state=1"))        // 执行SQL语句  
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
    }
    else
    {
        printf("query success\n");
    }
    res = mysql_store_result(&mysql);
    
    char s[10];
    memset(s, 0, sizeof s);

	sprintf(s,"%d",mysql_affected_rows(&mysql)); // s表示在线人数总个数
    strcpy(online,s);

    while (column = mysql_fetch_row(res))
    {
    	
		strcat(online,"#");
        strcat(online,column[0]);
    }
	
    
}

// 用户注销函数
bool Del(char new_name[20],char new_pw[32]){
	// 通过删除数据库中对应的行，来实现用户账号的注销

	memset(query, 0, sizeof query);
    strcpy(query, "delete from user where name='"); 
	strcat(query,new_name);
	strcat(query,"';");

    if (mysql_query(&mysql, query))        //执行SQL语句  
    {
        printf("Query failed \n");
        return false;
    }
    else
    {
        printf("Delete success\n");
        return true;
    }
}

// 根据用户名查询套接字，在线则返回对应套接字，否则返回0
int sel_socket(char name[20])
{
	// 数据库中，通过用户名查询对应的socket并返回（不在线socket默认为0）
    mysql_query(&mysql, "set names gbk");
   	memset(query, 0, sizeof query);
	strcpy(query, "select data_sock from user where name='");
	strcat(query, name);
	strcat(query, "';");
    if (mysql_query(&mysql, query))        //执行SQL语句  
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        return false;
    }
    else
    {
        printf("query success\n");
    }
    res = mysql_store_result(&mysql);
    column = mysql_fetch_row(res);

    if(atoi(column[0])!=0)	
		return atoi(column[0]);
    else	
		return 0;

}


// 根据套接字查询用户名
void find(int sock,char name[20])
{
	// 发送信息时，用户输入的是套接字，因此为了提示输出，需要查询用户名
	// 此处传入的字符串name为空字符串，返回套接字sock对应的用户名

    mysql_query(&mysql, "set names gbk");
    int u_id;
    char s[10];
    memset(s, 0, sizeof s);
    memset(query, 0, sizeof query);
    strcpy(query, "select name from user where data_sock=");
    sprintf(s, "%d", sock);
    strcat(query,s);
 
    if (mysql_query(&mysql,query))        // 执行SQL语句  
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        exit(1);
    }
    else
    {
        printf("query success\n");
    }
    res = mysql_store_result(&mysql);
    
    while (column = mysql_fetch_row(res))
    {
    	    strcpy(name,(column[0]));
    }
    
}

// 查询用户名是否存在，可用于注册时判读用户名是否重复
bool exist(char name[20])
{
	// 在数据库中select对应的name信息
	int num=0;
	mysql_query(&mysql, "set names gbk");
    memset(query, 0, sizeof query);
	strcpy(query, "select count(*) from user where name='");
	strcat(query,name);
	strcat(query,"';");
    if (mysql_query(&mysql,query))   // 执行失败
    {
        printf("Query failed (%s)\n", mysql_error(&mysql));
        exit(1);
    }
    else     
    {
        printf("query success1\n");
    }
	
	res = mysql_store_result(&mysql);
    
    column = mysql_fetch_row(res);
    num=atoi(column[0]);
    printf("column[0]=%d\n",num);
    if (num>0)	
		return true;
	else	
		return false;
}

// server端运行函数，用于开新线程
void* ser(int sockfd){
	char buf[MAXDATASIZE];
	int nbytes;
	int count=0;
	int on=0;
	char temp[MAXDATASIZE];
	const char s[2] = "#";
	char name[20]={0};
	char pw[32]={0};
	char who_on[80]={0};
	char temp1[MAXDATASIZE]={0};
	char temp2[MAXDATASIZE]={0};
	char temp3[MAXDATASIZE]={0};
	char temp_msg[MAXDATASIZE]={0};
	char temp_name[MAXDATASIZE]={0};
	char*token;
	char*loc;
	
	int i;
	int ssize=0;

	int res;
	MYSQL_RES *res_ptr;		// 指向查询结果
	mysql_init(&mysql); 
	
	if(!&mysql)  
	{  
		fprintf(stderr,"mysql_init failed\n");  
		return EXIT_FAILURE;  
	}  
  
	mysql_real_connect(&mysql,HOST,USERNAME,PASSWORD,DATABASE,0,NULL,0);  
  
	if(&mysql)  
		printf("Connection success\n");  
	else  
		printf("Connection failed\n");  
	
	// 一个客户端连接就会获得一个线程，这里是数据传输套接字直接进行操作即可
	bzero(buf,sizeof(buf));
	sprintf(buf,"welcome!");
	write(sockfd,buf,strlen(buf));
	printf("send response to client:%s\n", buf);
	sleep(2);

	printf("now_socket=%d\n",sockfd);
	
	// 从用户端获取操作指令：0为注册；1为登录
	nbytes=read(sockfd,buf,MAXDATASIZE);
	buf[nbytes]='\0';
	printf("recv message from client:%s\n",buf);
	strcpy(temp,buf);

	/*///////////////////////////////////////////////////////////
	//	0. 注册
	///////////////////////////////////////////////////////////*/
	if(!strcmp(temp,"0"))
	{
		// 提示输入用户名和8位密码，用#分隔
		// e.g. name00#12345678
		
		nbytes=read(sockfd,buf,MAXDATASIZE);
		buf[nbytes]='\0';
		printf("recv message from client:%s\n",buf);
		
		strcpy(temp1,buf);
		
		strcpy(temp3,temp1);

		// 找出str指向的字符串中第一次出现字符ch的位置
		loc=strchr(temp3,'#');

		if(loc)		// 有#则格式正确，分离账号密码
		{
			token = strtok(temp1, s);strcpy(name,token);
		}
		else		// 无#则格式错误，返回错误信息
		{
			bzero(buf,sizeof(buf));
			sprintf(buf,"Wrong:The format is wrong!");
			write(sockfd,buf,strlen(buf));
			printf("send response to client:%s\n", buf);
			sleep(1);
			pthread_exit (NULL);
		}
		if(exist(name))		// 判断用户名是否重复，重复则退出
		{
			bzero(buf,sizeof(buf));
			sprintf(buf,"Wrong:The user name is already registered");
			write(sockfd,buf,strlen(buf));
			printf("send response to client:%s\n", buf);
			sleep(1);
			pthread_exit (NULL);
		}

		// 如果名字不重复：正常
		token = strtok(NULL, s);strcpy(pw,token);
		
		if(Register(name,pw))
		{
			bzero(buf,sizeof(buf));
			sprintf(buf,"You have registered successfully!");
			write(sockfd,buf,strlen(buf));
			printf("send response to client:%s\n", buf);
			Log(1,sockfd,name);		// 注册后直接登录
		}
		else
		{
			printf("register error!");
		}
	}

	/*///////////////////////////////////////////////////////////
	//	1. 登录
	///////////////////////////////////////////////////////////*/
	else if(!strcmp(temp,"1"))
	{
		int flag=0;
		while(1){
			count++;
			nbytes=read(sockfd,buf,MAXDATASIZE);// 读入登录的账号密码
			buf[nbytes]='\0';
			printf("recv message from client:%s\n",buf);

			strcpy(temp1,buf);
			strcpy(temp3,temp1);

			// 找出str指向的字符串中第一次出现字符ch的位置
			loc=strchr(temp3,'#');

			if(loc){
				token = strtok(temp1, s);strcpy(name,token);
			}
			
			if(loc&&(exist(name)))	// 输入正确（有#）且用户存在，则判断密码是否正确
			{
				token = strtok(NULL, s);strcpy(pw,token);
				printf("token is ok!\n");
				
				// 把结果传回给客户端，成功为right，失败为wrong
				if(compare(name,pw))	// 用户密码正确，成功登录
				{
					bzero(buf,sizeof(buf));
					sprintf(buf,"right");
					write(sockfd,buf,strlen(buf));	// 传回判定结果
					printf("send response to client:%s\n", buf);
					flag=1;
				}
				else	// 用户密码错误，返回错误提示
				{
					bzero(buf,sizeof(buf));
					sprintf(buf,"wrong");
					write(sockfd,buf,strlen(buf));
					printf("send response to client:%s\n", buf);
				}
			}
			else	// 如果输入不正确（无#），或者用户名不存在，返回错误提示
			{
				count--;
				bzero(buf,sizeof(buf));
				sprintf(buf,"wrong input");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
			}

			// 登录成功后，更新数据库状态，并返回登录成功提示
			if(flag==1)
			{
				//把sockfd加到数据库里
				Log(1,sockfd,name);
				
				sleep(1);//很重要，避免同时发送两条消息
				bzero(buf,sizeof(buf));
				sprintf(buf,"You have login successfully!");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
				break;
			}

			// 防止线程过多，终止线程 
			if(count==3) {pthread_exit (NULL);	break;}
		}
	}


	/*///////////////////////////////////////////////////////////
	//	2. 退出
	///////////////////////////////////////////////////////////*/
	else if(!strcmp(temp,"2"))
	{
		bzero(buf,sizeof(buf));
		sprintf(buf,"You have exit successfully!");
		write(sockfd,buf,strlen(buf));
		printf("send response to client:%s\n", buf);
		pthread_exit (NULL);
	}


	/*///////////////////////////////////////////////////////////
	//	其他. 异常输入（输入不是1或2）
	///////////////////////////////////////////////////////////*/
	else
	{
		// 输入异常（服务器结束线程，客户端退出<用exit>）
		bzero(buf,sizeof(buf));
		sprintf(buf,"Input error");
		write(sockfd,buf,strlen(buf));
		printf("send response to client:%s\n", buf);
		pthread_exit (NULL);
	}
	

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

	while(1){
		nbytes=read(sockfd,buf,MAXDATASIZE);
		buf[nbytes]='\0';
		printf("recv message from client:%s\n",buf);
		strcpy(temp,buf);
		
		/*///////////////////////////////////////////////////////////
		//	2. 查询在线人数
		///////////////////////////////////////////////////////////*/
		if(!strcmp(temp,"2")){
			//查询成功
			Online(who_on);		// who_on数组中：在线个数#name1#name2    （至少有一个他自己）
			bzero(buf,sizeof(buf));
			sprintf(buf,who_on);
			write(sockfd,buf,strlen(buf));
			printf("send response to client:%s\n", buf);

			sleep(1);
		}
		

		/*///////////////////////////////////////////////////////////
		//	3. 聊天（文字内容发送）
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"3"))
		{
			while(1){
				nbytes=read(sockfd,buf,MAXDATASIZE);
				buf[nbytes]='\0';
				printf("recv message from client:%s\n",buf);
				strcpy(temp2,buf);
				if(!strcmp(temp2,"###"))	break;
				// 到这temp2里面是被聊天的对象B
				// 判断B在不在线,on为B当前的socket（sel_socket返回查询用户名的socket）
				on=sel_socket(temp2);
				// 读客户端发来的msg
				nbytes=read(sockfd,buf,MAXDATASIZE);
				buf[nbytes]='\0';
				printf("recv message from client:%s\n",buf);
				if(on){
					// 给收方B的
					strcpy(temp_msg,"recv message from ");
					// 根据套接字查找发方用户名
					find(sockfd,temp_name);
					strcat(temp_msg,temp_name);		// 发方用户名
					strcat(temp_msg," :");
					strcat(temp_msg,buf);
					
					bzero(buf,sizeof(buf));
					sprintf(buf,temp_msg);
					write(on,buf,strlen(buf));
					printf("send response to client:%s\n", buf);
				
					// 给发方A的！！！！！！
					bzero(buf,sizeof(buf));
					sprintf(buf,"The msg has already been sent");
					write(sockfd,buf,strlen(buf));
					printf("send response to client:%s\n", buf);
				}
				else	// B不在线
				{
					bzero(buf,sizeof(buf));
					sprintf(buf,"The receiver is not online");
					write(sockfd,buf,strlen(buf));
					printf("send response to client:%s\n", buf);//
				}
			
			}
		}
		

		/*///////////////////////////////////////////////////////////
		//	4. 注销账号
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"4"))
		{
			nbytes=read(sockfd,buf,MAXDATASIZE);	// 读入登录的账号密码
			buf[nbytes]='\0';
			printf("recv message from client:%s\n",buf);
			
			strcpy(temp1,buf);
			token = strtok(temp1, s);strcpy(name,token);
			token = strtok(NULL, s);strcpy(pw,token);

			// 先要核对用户名和密码，然后再删除
			if(compare(name,pw)&&Del(name,pw))	// 可以删除，并且成功
			{
				bzero(buf,sizeof(buf));
				sprintf(buf,"You have deleted successfully!");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
				pthread_exit (NULL);
			}
			else	// 删除失败
			{
				bzero(buf,sizeof(buf));
				sprintf(buf,"Delete failed!");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
			}
		}
		

		/*///////////////////////////////////////////////////////////
		//	5. 退出登录
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"5"))
		{
			// 更新数据库状态并关闭线程
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			printf("recv message from client:%s\n",buf);
			
			strcpy(name,buf);
			Log(0,0,name);
			
			bzero(buf,sizeof(buf));
			sprintf(buf,"You have logout successfully!");
			write(sockfd,buf,strlen(buf));
			printf("send response to client:%s\n", buf);
			pthread_exit (NULL);
		}
		

		/*///////////////////////////////////////////////////////////
		//	6. 文件传输
		///////////////////////////////////////////////////////////*/
		else if(!strcmp(temp,"6"))
		{
			// 收到username判断是否在线
			nbytes=read(sockfd,buf,MAXDATASIZE);
			buf[nbytes]='\0';
			printf("recv message from client:%s\n",buf);
			strcpy(temp2,buf);
			// 到这temp2里面是接收文件的对象B，查询套接字判断是否在线
			on=sel_socket(temp2);	// on是B的套接字
			
			if(on)	// B在线则开始传送文件
			{
				bzero(buf,sizeof(buf));
				sprintf(buf,"ON");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
				
				bzero(buf,sizeof(buf));
				find(sockfd,temp_name);
				strcpy(temp3,"\nfile to be received");
				sprintf(buf,temp3);
				write(on,buf,strlen(buf));	// 服务器告诉B收来自A的文件
				printf("send response to client:%s\n", buf);
				
				sleep(2);
				bzero(buf,sizeof(buf));
				strcpy(temp3,temp_name);
				sprintf(buf,temp3);
				write(on,buf,strlen(buf));	// 向接受方发送发送方名字
				printf("send response to client:%s\n", buf);
				
				// 开始正式的文件传输
				// 先收到文件名字
				nbytes=read(sockfd,buf,MAXDATASIZE);
				buf[nbytes]='\0';
				printf("recv filename from sender:%s\n",buf);
				
				// 把文件名字发给收方
				char file_name[256]={0};
				strcpy(file_name,buf);
				bzero(buf,sizeof(buf));
				sprintf(buf,file_name);
				write(on,buf,strlen(buf));
				
				// 收到文件size
				nbytes=read(sockfd,buf,MAXDATASIZE);
				buf[nbytes]='\0';
				printf("recv filesize from sender:%s\n",buf);
				// 把文件名size发给收方
				char file_size[256]={0};
				strcpy(file_size,buf);
				bzero(buf,sizeof(buf));
				sprintf(buf,file_size);
				write(on,buf,strlen(buf));
				printf("..................\n");
				
				puts(file_size);
				
				for(i=0;i<strlen(file_size);i++){
					ssize=ssize*10;
					ssize=ssize+file_size[i]-48;
				}
				
				int ret;
				sleep(1);
				bzero(buf,sizeof(buf));
				
				int write_len = 0;
				
				while(1)
				{
					printf("%d\n", ssize + 1);
					ret = read(sockfd, buf, sizeof(buf));
					write(on, buf, ret);
					bzero(buf,sizeof(buf));
					write_len += ret;	// 记录写入的字节数
					printf("len=%d",write_len);
					
					if( ret <= 0 || write_len==ssize)
					{
						printf("\n [recv-%s] receive file done!!!\n", file_name);
						break;
					}
				}
				ssize=0;
						
			}
			else{
				bzero(buf,sizeof(buf));
				sprintf(buf,"The receiver is not online");
				write(sockfd,buf,strlen(buf));
				printf("send response to client:%s\n", buf);
			}

		}
	
	}
	
}

int main(){
	int listen_sock;	// 监听套接字
	int num=0;
	int client_sock;
	int sin_size;
	int rc;
	struct sockaddr_in servaddr,clientaddr;

	pthread_t tids[20];
	
	// 创建套接字（socket）
	listen_sock=socket(AF_INET,SOCK_STREAM,0);
	if(listen_sock==-1){
		printf("can't create socket\n");
		exit(1);
	}
	int opt=1;
 	unsigned int len=sizeof(opt);
 	setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,len);
    setsockopt(listen_sock,SOL_SOCKET,SO_KEEPALIVE,&opt,len);

	
	// 分配套接字地址
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(PORT);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	// 绑定地址和端口（bind）
	if(bind(listen_sock,(struct sockaddr *)&servaddr,sizeof(struct sockaddr))==-1){
		printf("bind error\n");
		close(listen_sock);
		exit(1);
	}
	
	// 监听端口（listen）
	if(listen(listen_sock,BACKLOG)==-1){
		printf("listen error\n");
		close(listen_sock);
		exit(1);
	}
	
	// 循环接受客户端连接（accept）
	while(1){
		sin_size=sizeof(struct sockaddr_in);
		// 该客户端的数据传输套接字创建
		client_sock=accept(listen_sock,(struct sockaddr *)&clientaddr,&sin_size);

		if(client_sock==-1){
				printf("accept error\n");
				continue;
		}
		// 创建线程，调用ser函数完成交流
		rc = pthread_create(&tids[num],NULL,ser,(void*)client_sock);
		if (rc)	// 创建线程失败
		{
      
			printf("ERORR; return code from pthread_create() is %d\n", rc);
			exit(EXIT_FAILURE);
        }
		num++;
	}
	
	// 关闭套接字（close）
	close(listen_sock);
	// 关闭mysql
	mysql_close(&mysql); 

	return 0;
}



	
