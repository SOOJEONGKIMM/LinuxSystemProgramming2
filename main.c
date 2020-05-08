#include"ssu_mntr.h"

int main(void){
    char curdir[PATH_SIZE];
    char trashdir[PATH_SIZE];
    char checkdir[PATH_SIZE];
    char filesdir[PATH_SIZE];//backup deleting file
    char infodir[PATH_SIZE];//deleting file info dir
    int fd;
    char logpath[PATH_SIZE];//log.txt path name
    pid_t pid;

    memset(curdir,0,PATH_SIZE);
    memset(trashdir,0,PATH_SIZE);
    memset(checkdir,0,PATH_SIZE);
    memset(filesdir,0,PATH_SIZE);
    memset(infodir,0,PATH_SIZE);
    memset(logpath,0,PATH_SIZE);

    strcpy(curdir,getcwd(NULL,0));//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    printf("curdir:%s\n",curdir);
    sprintf(checkdir,"%s/check",curdir);
    sprintf(trashdir,"%s/trash/files",curdir);
    sprintf(filesdir,"%s/trash/files",curdir);
    sprintf(infodir,"%s/trash/info",curdir);

    if(chdir(curdir)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",curdir);
	exit(1);
    }
    //백업디렉토리(서브디렉토리) 이름을 지정해서 생성.
    sprintf(checkdir,"%s/check",curdir);
    mkdir(checkdir,0744);

    /* sprintf(logpath,"%s/log.txt",checkdir);
       if((fd=open(logpath,O_RDWR|O_CREAT|O_TRUNC,0644))<0){
       fprintf(stderr,"open error for %s\n",logpath);
       exit(1);
       }*/
	    scandirlog(checkdir,forlogtxt);
    ssu_mntr_play();//옵션입력으로 넘어감.
    close(fd);
    return 0;
}
void forlogtxt(char *file){
    char *fname="log.txt";
    struct stat buf;//파일정보 구조체 
    struct tm *t;//시간값 표현하기 위한 구조체
    FILE *fp;
    int fd, maxfd;

    pid_t pid;

    //printf("OO");
    //printf(file);//debug
    char debugdir[PATH_SIZE];
    strcpy(debugdir,getcwd(NULL,0));//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    printf("debugdir:%s",debugdir);

    //fork()로 자식 프로세스를 생성한다. 
    //이 함수는 한 번 호출되나 두 개의 리턴값을 리턴하는 함수다.
    //자식에게 리턴하는값은 0, 부모에게는 새 자식프로세스의 ID. 
    if((pid=fork())<0)//프로세스 생성 실패시 -1리턴.
	exit(0);
    //부모프로세스를 종료시킴.
    else if(pid!=0)
	exit(0);
    pid=getpid();
    //터미널 종료시 signal의 영향을 받지 않는다.
    signal(SIGHUP, SIG_IGN);
    close(0);//STDIN_FILENO
    close(1);//STDOUT_FILENO
    close(2);//STDERR_FILENO
    maxfd=getdtablesize();

    for(fd=0;fd<maxfd;fd++)
	close(fd);

    /* if(chdir("/")==-1){
       printf("chdir error\n");
       exit(1);
       }*/

    umask(0);
    //setsid로 새로운 세션만들고,
    //현재프로세스(자식)의 세션의 PID가 제어권을 가지도록 한다.
    if(setsid()==-1)
	exit(0);

    fd=open("/dev/null",O_RDWR);
    dup(0);
    dup(0);

    while(1){
	//여기부터 프로그램 본체
	if((fp=fopen(fname,"a"))<0){
	    fprintf(stderr,"fopen %s error",fname);
	    exit(1);
	}
	int inum;

	if(stat(file,&buf)<0){
	    fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][delete_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,file);
	}
	else{	
	    inum=buf.st_ino;
	    printf("%d",inum);
	    //파일 생성시각
	    t=localtime(&buf.st_ctime);
	    fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][create_%s]\n",t->tm_year + 1900, t->tm_mon+1,t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,file);

	    //파일 최종수정시각
	    t=localtime(&buf.st_mtime);
	    fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][LASTmodify_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,file);

	    //파일 최종접근시각--***log.txt에선 필요없음.  
	    t=localtime(&buf.st_atime);
	    fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][LASTaccess_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,file);
	}
    }
}
void scandirlog(char *scanningdir,void(*func)(char*)){
    char listfname[FILE_SIZE];
    char temppath[PATH_SIZE];
    char listfpath[PATH_SIZE];
    int fsize;
    struct stat buf;//fsize
    char mtime[TM_SIZE];
    char dtime[TM_SIZE];
    struct stat tempstat;
    int i;

    int countdirp=0;
    struct dirent **flist;
    printf("---------------------------------------_");

    if(chdir(scanningdir)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",scanningdir);
	perror("chdir");
    }
    if((countdirp=scandir(scanningdir,&flist,0,alphasort))<0){
	fprintf(stderr,"scandir error for %s\n",scanningdir);
	exit(1);
    }
    i=0;

    while(i<countdirp){
	MNode *node=(MNode*)malloc(sizeof(MNode));
	memset(node,0,sizeof(node));

	memset(temppath,0,PATH_SIZE);


	//현재디렉토리, 이전디렉토리는 무시.
	if((!strcmp(flist[i]->d_name,"."))||(!strcmp(flist[i]->d_name,".."))){
	    continue;
	}
	memset(temppath,0,PATH_SIZE);
	if(realpath(flist[i]->d_name,temppath)==0){
	    fprintf(stderr,"real path error for %s\n",flist[i]->d_name);
	    //  exit(1);
	}
	if(lstat(temppath,&tempstat)<0){
	    fprintf(stderr,"lstat error for %s\n",temppath);
	    // exit(1);
	}
	if(!strcmp(flist[i]->d_name,".")||!strcmp(flist[i]->d_name,"..")){
	    i++;
	    continue;
	}
	memset(listfpath,0,PATH_SIZE);
	if(realpath(flist[i]->d_name,listfpath)==0){
	    fprintf(stderr,"real path error for %s\n",flist[i]->d_name);
	    //  exit(1);
	}
	if(lstat(temppath,&tempstat)<0){
	    fprintf(stderr,"lstat error for %s\n",temppath);
	    // exit(1);
	}

	if(S_ISREG(tempstat.st_mode)){
	    memset(node->listfname,0,PATH_SIZE);//fname
	    strcpy(node->listfname,flist[i]->d_name);

	    memset(node->listfpath,0,PATH_SIZE);//path
	    strcpy(node->listfpath,temppath);

	    fsize=0;
	    fsize=stat(node->listfpath,&buf);//size
	    node->fsize=0;
	    node->fsize=buf.st_size;

	    memset(node->mtime,0,TM_SIZE);//mtime dtime
	    memset(node->dtime,0,TM_SIZE);
	    strcpy(node->mtime,mtime);
	    strcpy(node->dtime,dtime);

	    Mlist_insert(node);

	}
	if((tempstat.st_mode&S_IFDIR)==S_IFDIR){

	    /*strcpy(node->listfpath,temppath);
	      fsize=0;
	      fsize=stat(node->listfpath,&buf);
	      node->fsize=0;
	      node->fsize=buf.st_size;
	      Clist_insert(node);*/
	    scandirlog(flist[i]->d_name,func);


	}
	i++;
    }
    chdir("..");

}
void Mlist_insert(MNode *newNode){//list에 node추가
    newNode->next=NULL;
    if(mhead==NULL)
	mhead=newNode;
    else{
	MNode *listF;
	listF=mhead;
	while(listF->next!=NULL)
	    listF=listF->next;
	listF->next=newNode;
    }
}

