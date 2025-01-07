#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<time.h>

// 用于创建一个唯一的key
#define MSG_FILE "/etc/passwd"

// 消息结构
struct msg_form {
    long mtype;
    char mtext[256];
};

int main()
{
    int msqid;
    key_t key;
    struct msg_form msg;
  
   // 获取key值
   if((key = ftok(MSG_FILE,'z')) < 0)
   {
       perror("ftok error");
       exit(1);
    }

  // 打印key值
    printf("Message Queue - Server key is: %d.\n", key);

   // 创建消息队列
    if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
    {
       perror("msgget error");
       exit(1);
    }
 
    // 打印消息队列ID及进程ID
    printf("My msqid is: %d.\n", msqid);
    printf("My pid is: %d.\n", getpid());

    // 循环读取消息
    for(;;) 
    {
        msgrcv(msqid, &msg, 256, 888, 0);// 返回类型为888的第一个消息
        printf("Server: receive msg.mtext is: %s.\n", msg.mtext);
        printf("Server: receive msg.mtype is: %d.\n", msg.mtype);

        msg.mtype = 999; // 客户端接收的消息类型
        sprintf(msg.mtext, "hello, I'm server %d", getpid());
        msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
   }
   return 0;
 }