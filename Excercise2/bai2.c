#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

pthread_t tid;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define PHONE_LEN	10

typedef enum
{
    VNPT,
    FPT,
    VIETTEL,
    MOBI
} net_t;


typedef struct network
{
    net_t net;
    uint8_t sodau[3];
} network_t;

network_t phone_valid[] = {
    VNPT,{1,2,3},
    VNPT,{1,4,5},
    FPT,{5,3,2},
    MOBI,{6,2,1},
	VIETTEL,{0,3,8}
};

typedef struct UserInfo{
    char name[20];
    uint8_t age;
    char phoneNumber[11];
    char fileName[50];
} UserInfo_t;

UserInfo_t usr_info;
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
        int result = memcmp(sample,phone_valid[i].sodau,3);
        if(result == 0)
		{
            return true;
		}
    }
    return false;
}

void* thread1(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond, &lock);
		bool is_valid;
        if(is_phone_number_valid(usr_info.phoneNumber) /*&& is_phone_len_valid(strlen(receive_mess.info.phoneNumber))*/)
        {
            printf("valid infomation input\n");
			printf("name is %s\n age %d\n phonenumber %s\n filename %s\n",usr_info.name, usr_info.age, usr_info.phoneNumber, usr_info.fileName);
            int fd =  open(usr_info.fileName, O_RDONLY);
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
        pthread_mutex_unlock(&lock);
    }
}

void main()
{
    int status;
    status = pthread_create(&tid, NULL, &thread1, NULL);
    if (status == 0)
        printf("\n Thread created successfully\n");
    else
        printf("\ncan't create thread :[%s]", strerror(status));
    while(1)
    {
        pthread_mutex_lock(&lock);
        bool is_continue = true;
        printf("xin moi nhap thong tin ca nhan \n");
        printf("nhap ten \n");
        scanf("%s", usr_info.name);
        printf("nhap tuoi \n");
        scanf("%hhu", &usr_info.age);
        printf("nhap so dien thoai \n");
        scanf("%s", usr_info.phoneNumber);
        printf("nhap file \n");
        scanf("%s", usr_info.fileName);
        // printf("data0 %s , %d, %s, %s \n",data[user].info.name,data[user].info.age,data[user].info.phoneNumber,data[user].info.fileName );
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
        printf("ban muon nhap nua khong \n");
        scanf("%d", &is_continue);
        if(!is_continue)
            break;
        // user++;
    }
}