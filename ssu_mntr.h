#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//getcwd()
#include<fcntl.h>
#include<sys/stat.h>//검색된결과값상태저장하는 구조체 stat
#include<sys/types.h>
#include<sys/time.h>
#include<pthread.h>
#include<string.h>
#include<dirent.h>//stddir()
#include<time.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>

//pthread_mutex_t mutex;

#define BUFFER_SIZE 1024
#define PATH_SIZE 256
#define TM_SIZE 32
#define OPT_SIZE 8 

//char *logfname="log.txt";
char onlyfname[PATH_SIZE];

//파일정보를 링크드리스트(연결리스트)에 전역변수로 저장.
typedef struct fNode{//연결리스트의 노드 구조체
    struct fNode *next;//다음 노드의 주소를 저장할 포인터
    char fname[BUFFER_SIZE];//데이터를 저장할 멤버
    struct fNode *prev;
}fNode;
//이러한 연결노드들을 갖고있는 리스트.head는 리스트의 시작노드, tail는 노드의 끝부분 가리키는 포인터.
typedef struct fList{//연결리스트의 노드 구조체
struct fList *head;//데이터는 없는 head노드 
    struct fList *next;//다음 노드의 주소를 저장할 포인터
    char data;//데이터를 저장할 멤버
    struct fList *prev;
}fList;

static int indent=0;//깊이를 체크하는 변수
char cdir[256];//파일경로 저장

void forlogtxt(char *file);
void Scandir(char *fname, fNode *node);

void ssu_mntr_play(void);
int check_opt(const char *str);
int get_deleteOpt(char *str);//DELETE [FILENAME] [ENDTIME] [OPTION]
void deloptR_alarm(int k);
int do_deleteOpt(void);//DELETE [FILENAME] [ENDTIME] [OPTION]
int do_sizeOpt(char *str);//SIZE [FILENAME] [OPTION]
int do_recoverOpt(char *str);
int do_treeOpt(char *str);
int do_helpOpt(char *str);
