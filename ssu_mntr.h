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
#define FILE_SIZE 128
#define TM_SIZE 64
#define OPT_SIZE 8 

#define ALL 0
#define ONLYDIR 1
#define DEPTHEXIST 2
#define TREE 3


//char *logfname="log.txt";
char onlyfname[PATH_SIZE];
//int dupindex;

char curdir[PATH_SIZE];
char checkdir[PATH_SIZE];
char trashdir[PATH_SIZE];
char filesdir[PATH_SIZE];//backup deleting file
char infodir[PATH_SIZE];//backup deleting file

typedef struct _list{//trash dir의 파일리스트
    // char listfname[PATH_SIZE];
    int size;
    struct _list *next;
}List;

typedef struct _node{//recover l option 
    //char filesdir[PATH_SIZE];
    char listfpath[PATH_SIZE];
    char listfname[FILE_SIZE];
    char dtime[TM_SIZE];
    char mtime[TM_SIZE];
    int dupindex;
    char dupped[TM_SIZE];//info정보에 그다음내용이 있다면 중복파일이라는 걸 확인 위해
    int optldt;

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
    char listfname[PATH_SIZE]; 
    int fsize;
    char relP[PATH_SIZE];
    CList *chead;
    struct _cnode *next;
}CNode;
CNode * chead;
/////////////////////////////////////////////////////////////////////////////////////////
//파일정보를 링크드리스트(연결리스트)에 전역변수로 저장.
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


//static int indent=1;//깊이를 체크하는 변수
char cdir[256];//파일경로 저장

//void scandirlog(char *scanningdir);
void startdemon(char *curdir,char *checkdir);
void forlogtxt(void);
int scanmondirBASE(char *searchdir,int inityes);
int scanmondirNEW(char *searchdir,int inityes);
void write_logtxt(char *fname, char *status,char *mtimeifmod);
void get_time(char *str,char *status);
int list_search(char *cmpfname,int searchisbase);
int is_modified(char *cmpmtime,int cmpinum,char *cmpfname);
void list_print1(int bnodeyes);

void list_insert(Node *newNode);//list에 node추가
void Clist_insert(CNode *newNode);//list에 node추가
void Mlist_insert(MNode *newNode);
void Mnlist_insert(MnNode *newNode);
void list_sort(int (*cmp)());
void list_sortC(int (*cmp)());
void list_print();
void swap_node_data(Node *, Node *);
int optldt_cmp(Node *a, Node *b);
int str_cmp(CNode *, CNode *);


void ssu_mntr_play(void);
int check_opt(const char *str);
int get_deleteOpt(char *str);//DELETE [FILENAME] [ENDTIME] [OPTION]
void deloptR_alarm(int k);
int do_deleteOpt(void);//DELETE [FILENAME] [ENDTIME] [OPTION]
int do_sizeOpt(char *str);//SIZE [FILENAME] [OPTION]
void makeRelativeP(char *absolutepath, char *relativepath, char *curdir);
void do_sizeOptDIR(char *dirname);
int do_recoverOpt(char *str);
int do_treeOpt(char *str);
int do_helpOpt(char *str);
void scanningTdir(char *scanningdir);
void scanningCdir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir);//,static int indent);//,int indent);



