#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//getcwd()
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/time.h>
#include<pthread.h>
#include<string.h>
#include<dirent.h>//scandir()
#include<time.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>

#define SECOND_TO_MICRO 1000000
#define BUFFER_SIZE 1024
#define PATH_SIZE 256
#define FILE_SIZE 128
#define TM_SIZE 64
#define OPT_SIZE 8 

#define ALL 0
#define ONLYDIR 1
#define DEPTHEXIST 2
#define TREE 3

struct timeval begin_t, end_t;

char onlyfname[PATH_SIZE];

char curdir[PATH_SIZE];
char checkdir[PATH_SIZE];
char trashdir[PATH_SIZE];
char filesdir[PATH_SIZE];//backup deleting file
char infodir[PATH_SIZE];//backup deleting file

typedef struct _list{//trash dir의 파일리스트
    int size;
    struct _list *next;
}List;

typedef struct _node{//recover l option 
    char listfpath[PATH_SIZE];
    char listfname[FILE_SIZE];
    char dtime[TM_SIZE];
    char mtime[TM_SIZE];
    int dupindex;
    char dupped[TM_SIZE];//info정보에 그다음내용이 있다면 중복파일이라는 걸 확인 위해
    int optldt;
    char dtimestr[TM_SIZE];
    struct tm delt;
    int fsize;
    List *head;
    struct _node *next;
}Node;
Node * head;

typedef struct _clist{
    int size;
    struct _clist *next;
}CList;

typedef struct _cnode{//SIZE TREE 
    char listfpath[PATH_SIZE];
    char listdpath[PATH_SIZE];//size opt와 분별위해 delete에서 디렉토리 처리 위해 따로 만듦.
    char listfname[PATH_SIZE]; 
    char listdname[PATH_SIZE]; 
    int fsize;
    char relP[PATH_SIZE];
    CList *chead;
    struct _cnode *next;
}CNode;
CNode * chead;
//모니터링 파일정보를 링크드리스트(연결리스트)에 전역변수로 저장.
typedef struct MNode{//연결리스트의 노드 구조체
    struct MNode *next;//다음 노드의 주소를 저장할 포인터
    char listfname[FILE_SIZE];
    char listfpath[PATH_SIZE];
    int fsize;
    int inum;
    struct stat buf;  // time of last modification
    char mtime[TM_SIZE];
    char ctime[TM_SIZE];
    struct MNode *prev;
}MNode;
MNode * mhead;
typedef struct MnNode{//연결리스트의 노드 구조체
    struct MnNode *next;//다음 노드의 주소를 저장할 포인터
    char listfname[FILE_SIZE];
    char listfpath[PATH_SIZE];
    int fsize;
    int inum;
    struct stat buf;  // time of last modification
    char mtime[TM_SIZE];
    char ctime[TM_SIZE];
    struct MnNode *prev;
}MnNode;
MnNode * mnhead;
//이러한 연결노드들을 갖고있는 리스트.head는 리스트의 시작노드, tail는 노드의 끝부분 가리키는 포인터.
typedef struct fList{//연결리스트의 노드 구조체
    struct fList *head;//데이터는 없는 head노드 
    struct fList *next;//다음 노드의 주소를 저장할 포인터
    char data;//데이터를 저장할 멤버
    struct fList *prev;
}fList;

char cdir[256];//파일경로 저장

void ssu_runtime(struct timeval *begin_t, struct timeval *end_t);//프로그램실행시간 
void startdemon(char *curdir,char *checkdir);//데몬프로그램 시작 
void forlogtxt(void);//base스캔과 new스캔을 상태비교해서 로그파일입출력.
int scanmondirBASE(char *searchdir,int inityes);//base스캔 처음에 하고, 업데이트있을때만 호출
int scanmondirNEW(char *searchdir,int inityes);//new스캔 1초마다 forlogtxt()와 함께 호출됨.
void write_logtxt(char *fname, char *status,char *mtimeifmod);//파일입출력 
void get_time(char *str,char *status);//시간 계산 
int list_search(char *cmpfname,int searchisbase);//연결리스트에 존재하는지(create,delete)
int is_modified(char *cmpmtime,int cmpinum,char *cmpfname);//mtime과 파일이름비교(modify)
void list_print1(int bnodeyes);
void free_list(MNode* mhead);//연결리스트 초기화 

int list_search_CNode(char *cmpfname);//checkdir에 존재하는 파일인지 확인.
int path_search_CNode(char *cmpfpath);//checkdir에 존재하는 path인지 확인.
void list_insert(Node *newNode);//trashdir list에 node추가
void Clist_insert(CNode *newNode);//checkdir list에 node추가
void Mlist_insert(MNode *newNode);//모니터링 base스캔 list에 node추가
void Mnlist_insert(MnNode *newNode);//모니터링 new스캔 list에 node추가 
void list_sort(int (*cmp)());//trashdir 오름차순 정렬 
void list_sortC(int (*cmp)());//checkdir 오름차순 정렬 
void list_print();//recover -l option 삭제시간 오름차순으로 표준출력 
void swap_node_data(Node *, Node *);
int optldt_cmp(Node *a, Node *b);//삭제시간오름차순정렬 
int str_cmp(CNode *, CNode *);
int str_cmp_dtimestr(Node *a,Node *b);

void ssu_mntr_play(void);//프롬프트 입력 
int check_opt(const char *str);//프롬프트 명령어 확인 
int get_deleteOpt(char *str);//DELETE [FILENAME] [ENDTIME] [OPTION]
void deloptR_alarm(int k);//삭제시간에 시그널호출하는 시그널핸들러함수 
int do_deleteOpt(void);//DELETE [FILENAME] [ENDTIME] [OPTION]
int info_sizing(void);//info사이즈 2KB넘는지 확인 
void relPtoFile(char *onlyfname,char *relPFile,char *ch);//특정문자열 파싱하는 함수 
int do_sizeOpt(char *str);//SIZE [FILENAME] [OPTION]
int delEchar(char *buf, char *a);//문자열에서 특정문자열 제거.(뒤에서부터)
void makeRelativeP(char *absolutepath, char *relativepath, char *curdir);//상대경로로
int do_recoverOpt(char *str);//RECOVER [FILENAME] [OPTION]
int do_treeOpt(char *str);//TREE
int do_helpOpt(char *str);//HELP
void scanningTdir(char *scanningdir);//trashdir스캔 
//checkdir(지정디렉토리) 스캔 
void scanningCdir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir);

