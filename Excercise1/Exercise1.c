#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/msg.h>
#include <sys/ipc.h>
#include<stdbool.h>
#include <fcntl.h>
pthread_t tid;
key_t key;
int msgid;
#define PHONE_LEN	10
typedef enum
{
    VNPT,
    FPT,
    VIETTEL,
    MOBI
} net_t;

#define max 10000
typedef struct network
{
    net_t net;
    uint8_t three_firstNumber[3];
} network_t;
network_t phone_valid[] = {
    VNPT,{1,2,3},
    VNPT,{1,4,5},
    FPT,{5,3,2},
    MOBI,{6,2,1},
	VIETTEL,{0,3,8}
};

bool is_phone_len_valid(const uint8_t value)
{
	bool a = (value == PHONE_LEN) ? true : false;
	return a;
}

bool is_phone_number_valid(uint8_t* value)
{
    uint8_t sample[] = {value[0]-48,value[1]-48, value[2]-48};          // convert from ASCII to uint8_t
    for(uint8_t i = 0; i < sizeof(phone_valid)/sizeof(network_t); i++)
	{
        int result = memcmp(sample,phone_valid[i].three_firstNumber,3);
        if(result == 0)
		{
            return true;
		}
    }
    return false;
}

typedef struct UserInfo{
    char name[20];
    uint8_t age;
    char phoneNumber[11];
    char fileName[50];
} UserInfo_t;

typedef struct Message {
    long mtype;
    UserInfo_t info;
} Message_t;

void* thread1(void *arg)
{
    msgid = msgget(key, 0666 | IPC_CREAT);
    while(1)
    {
		bool is_valid;
        Message_t receive_mess;
        memset(&receive_mess,0,sizeof(Message_t));
        msgrcv(msgid, &receive_mess, sizeof(receive_mess), 1, 0);
        if(is_phone_number_valid(&receive_mess.info.phoneNumber) /*&& is_phone_len_valid(strlen(receive_mess.info.phoneNumber))*/)
        {
            printf("valid infomation input\n");
			printf("name is %s\n age %d\n phonenumber %s\n filename %s\n",receive_mess.info.name, receive_mess.info.age,receive_mess.info.phoneNumber,receive_mess.info.fileName);
            int fd =  open(receive_mess.info.fileName, O_RDONLY);
            if(fd > 0)
            {
                printf("openfile successfully\n");
                uint8_t buffer[100];
                memset(buffer, 0, sizeof(buffer)/sizeof(uint8_t));
                ssize_t size = read(fd,buffer,16);
                printf("buffer is %s\n", buffer);
            }
            else
            {
                printf("image file is not existed\n");
            }
        }
        else
        {
            printf("unvalid");
        }
    }
}

int main(void)
{
    Message_t data[100];
    int status;
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    status = pthread_create(&tid, NULL, &thread1, NULL);

    if (status == 0)
        printf("\n Thread created successfully\n");
    else
        printf("\ncan't create thread :[%s]", strerror(status));

    while(1)
    {   
        uint8_t user = 0;
        bool is_continue = true;
        printf("xin moi nhap thong tin ca nhan \n");
        printf("nhap ten \n");
        scanf("%s", data[user].info.name);
        printf("nhap tuoi \n");
        scanf("%u", &data[user].info.age);
        printf("nhap so dien thoai \n");
        scanf("%s", data[user].info.phoneNumber);
        printf("nhap file \n");
        scanf("%s", data[user].info.fileName);
        data[user].mtype = 1;
        msgsnd(msgid, &data[user], sizeof(data[user]), 0);
        printf("data0 %s , %d, %s, %s \n",data[user].info.name,data[user].info.age,data[user].info.phoneNumber,data[user].info.fileName );
        sleep(1);
        printf("ban muon nhap nua khong \n");
        scanf("%d", &is_continue);
        if(!is_continue)
            break;
        // user++;
    }
    
    sleep(1);
    return 0;
}

