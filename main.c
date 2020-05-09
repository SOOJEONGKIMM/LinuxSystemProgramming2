#include"./ssu_mntr.h"


void scanmondir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir);

int main(void){
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    int fd;
    //    char logpath[PATH_SIZE];//log.txt path name
    //    pid_t pid;

    memset(wdir,0,PATH_SIZE);
    memset(mondir,0,PATH_SIZE);

    getcwd(wdir,PATH_SIZE);//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    printf("main\n");

    if(chdir(wdir)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",curdir);
	//exit(1);
    }
    //백업디렉토리(서브디렉토리) 이름을 지정해서 생성.
    sprintf(mondir,"%s/check",wdir);
    //    mkdir(checkdir,0744);
    printf("checkdir:%s\n",mondir);

    scanmondir(NULL,0,TREE,1,curdir);
    startdemon(mondir);

    chdir(wdir);
    ssu_mntr_play();//옵션입력으로 넘어감.
    close(fd);
    //return 0;
}
void startdemon(char *scanningdir){

    int fd, maxfd;

    //    pid_t pid;

    printf("forlogtxt");
    //printf(file);//debug
    char debugdir[PATH_SIZE];
    strcpy(debugdir,getcwd(NULL,0));//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    printf("debugdir:%s",debugdir);

    //fork()로 자식 프로세스를 생성한다. 
    //이 함수는 한 번 호출되나 두 개의 리턴값을 리턴하는 함수다.
    //자식에게 리턴하는값은 0, 부모에게는 새 자식프로세스의 ID. 
    /*    if((pid=fork())<0)//프로세스 생성 실패시 -1리턴.
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

    if(chdir("/")==-1){
    printf("chdir error\n");
    exit(1);
    }

    umask(0);
    //setsid로 새로운 세션만들고,
    //현재프로세스(자식)의 세션의 PID가 제어권을 가지도록 한다.
    if(setsid()==-1)
    exit(0);

    fd=open("/dev/null",O_RDWR);
    dup(0);
    dup(0);
    */
    // while(1){
    printf("++++++++++++++++++++++++++++++++++whileloop++++++++++++++++++++\n");
    forlogtxt(scanningdir);
    //}
}

void forlogtxt(char *mondir){
    printf("scanning update dir:%s\n",mondir);
    char *fname="log.txt";
    struct stat tempstat;//scanning current update stat
    struct stat recurstat;//scanning current update stat
    struct stat buf; //linkedlist stat
    struct stat crebuf; //linkedlist stat(create log)
    struct tm *t;//시간값 표현하기 위한 구조체
    FILE *fp;

    chdir(mondir);
    printf("Hi\n");
    //여기부터 프로그램 본체
    if((fp=fopen(fname,"a"))<0){
	fprintf(stderr,"fopen %s error",fname);
	exit(1);
    }
    int inum,i;
    int newfile;//create flag
    char temppath[PATH_SIZE];
    int countdirp=0;
    struct dirent **flist;
    if((countdirp=scandir(mondir,&flist,0,alphasort))<0){
	fprintf(stderr,"scandir error for %s\n",mondir);
	exit(1);
    }
    i=0;
    while(i<countdirp){
	if(!strcmp(flist[i]->d_name,".")||!strcmp(flist[i]->d_name,"..")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,".git")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,"Makefile")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,"a.out")){
	    i++;
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

	MNode *crenode=(MNode*)malloc(sizeof(MNode));
	memset(crenode,0,sizeof(crenode));
	crenode=mhead;

	newfile=1;
	while(crenode){//create time fprint
	    stat(crenode->listfpath, &crebuf);
	    if(!strcmp(crenode->listfname,flist[i]->d_name)){
		if(crebuf.st_ino==crenode->inum)
		    newfile=0;
	    }
	    crenode=crenode->next;
	}
	if(newfile==1){
	    t=localtime(&tempstat.st_ctime);
	    fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][create_%s]\n",t->tm_year + 1900, t->tm_mon+1,t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,flist[i]->d_name);
	}

	MNode *lognode=(MNode*)malloc(sizeof(MNode));
	memset(lognode,0,sizeof(lognode));
	lognode=mhead;

	stat(lognode->listfpath,&buf);

	while(lognode){
	    if(stat(lognode->listfpath,&buf)<0){
		fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][delete_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,lognode->listfname);
	    }
	    else{	
		inum=buf.st_ino;
		printf("node->listfname:%s   ",lognode->listfname);
		printf("nodeinum:%d   inum: %d\n",lognode->inum,inum);
		if(lognode->inum==inum){
		    if(strcmp(lognode->listfname,flist[i]->d_name)){//이름변경 
			t=localtime(&buf.st_mtime);
			fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][modify_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,lognode->listfname);
		    }
		    else{
			if(strcmp(lognode->mtime,ctime(&buf.st_mtime))){//수정시각변화 
			t=localtime(&buf.st_mtime);
			fprintf(fp,"[%04d-%02d-%02d %02d:%02d:%02d][modify_%s]\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,lognode->listfname);
			}
		    }

		}
	    }
	    lognode=lognode->next;
	}	  

	if(S_ISDIR(tempstat.st_mode)){

	printf("SCANNNG:%s\n",temppath);
	    printf("~~~~~~~~~~~~~~SCANDIR RECURSIVE UNTIL END~~~~~~~~~~~~~\n");
	    forlogtxt(temppath);
	 }


/*	if(lstat(temppath,&recurstat)<0){
	    printf("lstat error\n");
	}
	if((recurstat.st_mode&S_IFDIR)==S_IFDIR){

	    //	indent++;
	    //strcpy(node->listfpath,temppath);
	    printf("listfpath:%s\n",lognode->listfpath);



	}

*/
	i++;
    }
    //indent--;
    chdir("..");
    printf("scandir update end======================================\n");
}

void scanmondir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir){//SIZE
    ////init first!    
    char curdir[PATH_SIZE];
    char checkdir[PATH_SIZE];
    char searchdirbuf[PATH_SIZE];
    static int indent;
    static int depthcnt;//recursive num
    if(indentinit==1){
	indent=0;//처음으로 스캔시작할때만 초기화되도록 
	mhead=NULL;
    }
    struct tm *t;//시간값 표현하기 위한 구조체
    char temppath[PATH_SIZE];
    char treefname[BUFFER_SIZE];
    char relativepath[PATH_SIZE];
    //struct stat statbuf;    
    struct stat tempstat;
    // struct timeval *renamet;    
    int i;   
    char listfname[FILE_SIZE];
    int recoptl=1;//init
    int fsize;//SIZE
    int fcnt=0;

    struct stat buf;//SIZE
    struct stat checkemptybuf;//TREE 빈폴더인경우 -----표시를 하지 않는다. 
    int emptydir=0;
    memset(curdir,0,PATH_SIZE);
    memset(checkdir,0,PATH_SIZE);
    memset(temppath,0,PATH_SIZE); 
    strcpy(curdir,getcwd(NULL,0));
    sprintf(checkdir,"%s/check",curdir);
    printf("-------------------------scanning dir for CNode list\n");
    printf("curdir:%s\n",curdir);
    memset(searchdirbuf,0,PATH_SIZE);
    if(searchdir==NULL&&sizeoptflag!=TREE)
	strcpy(searchdirbuf,curdir);
    else if(searchdir==NULL&&sizeoptflag==TREE)////////////////////////////////////////////////
	strcpy(searchdirbuf,checkdir);
    else
	sprintf(searchdirbuf,"%s/%s",curdir,searchdir);

    int countdirp=0;
    struct dirent **flist;

    if(chdir(searchdirbuf)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",searchdirbuf);
	perror("chdir");
    }
    if((countdirp=scandir(searchdirbuf,&flist,0,alphasort))<0){
	fprintf(stderr,"scandir error for %s\n",searchdirbuf);
	exit(1);
    }
    i=0;
    //info dir의 파일이름, 삭제시간, 수정시간 각 노드 만들어주기
    while(i<countdirp){
	MNode *node=(MNode*)malloc(sizeof(MNode));
	memset(node,0,sizeof(node));
	//printf("counting file num in %s dir..\n",searchdirbuf);
	if(!strcmp(flist[i]->d_name,".")||!strcmp(flist[i]->d_name,"..")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,".git")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,"Makefile")){
	    i++;
	    continue;
	}
	if(!strcmp(flist[i]->d_name,"a.out")){
	    i++;
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
	memset(node->listfname,0,PATH_SIZE);
	strcpy(node->listfname,flist[i]->d_name);
	printf("node->listfname:%s\n",node->listfname);

	//		t=localtime(&tempstat.st_mtime);

	memset(node->listfname,0,PATH_SIZE);
	strcpy(node->listfname,flist[i]->d_name);
//	if(S_ISREG(tempstat.st_mode)){

	    strcpy(node->listfpath,temppath);
	    fsize=0;
	    fsize=stat(node->listfpath,&buf);//SIZE 


	    node->fsize=0;
	    node->fsize=buf.st_size;//SIZE

	    memset(node->mtime,0,TM_SIZE);
	    strcpy(node->mtime,ctime(&buf.st_mtime));
	    printf("mtime:%s\n",node->mtime);

	    node->inum=buf.st_ino;//inode num


	    printf("fize(buf.st_size):%d\n",node->fsize);
	    printf("listfpath:%s\n",node->listfpath);

	    Mlist_insert(node);
//	}
	if((tempstat.st_mode&S_IFDIR)==S_IFDIR){

	    indent++;
	    //strcpy(node->listfpath,temppath);
	    printf("listfpath:%s\n",node->listfpath);

	    printf("~~~~~~~~~~~~~SCANDIR RECURSIVE UNTIL END~~~~~~~~~~~~~\n");

	    if(sizeoptflag==TREE)
		scanmondir(flist[i]->d_name,depth,TREE,0,delcurdir);

	}

	i++;
    }
    indent--;
    chdir("..");
    printf("-------------------------scanning dir for CNode list ends\n");
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

