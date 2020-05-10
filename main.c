#include"./ssu_mntr.h"


//void scanmondir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir);
int update;
int bfcnt, nfcnt;

int main(void){
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    //    char logpath[PATH_SIZE];//log.txt path name
        pid_t pid;

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
    //chdir(mondir);
    //    mkdir(checkdir,0744);
    printf("checkdir:%s\n",mondir);
    scanmondirBASE(mondir,1);
    //chdir(wdir);
    switch(pid=vfork()){
	case 0:
	    printf("I'm child. My PID is %d\n",getpid());
	    startdemon(wdir,mondir);
	default:
	    ssu_mntr_play();
    }

    //옵션입력으로 넘어감.
    return 0;
}
void startdemon(char *curdir,char *checkdir){
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    memset(wdir,0,PATH_SIZE);
    memset(mondir,0,PATH_SIZE);
    getcwd(wdir,PATH_SIZE);//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    sprintf(mondir,"%s/check",wdir);
    int bfcnt,nfcnt;
    int fd, maxfd;

    pid_t pid;

    printf("forlogtxt");
    //printf(file);//debug

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

    if(chdir("/")==-1){
	printf("chdir error\n");
	exit(1);
    }

    umask(0);
    //setsid로 새로운 세션만들고,
    //현재프로세스(자식)의 세션의 PID가 제어권을 가지도록 한다.
    if(setsid()==-1){
	fprintf(stderr,"set sid error\n");
	exit(0);
    }

    fd=open("/dev/null",O_RDWR);
    dup(0);
    dup(0);

	    while(1){
		sleep(1);
		printf("++++++++++++++++++++++++++++++++++whileloop++++++++++++++++++++\n");
		printf("check:%s\n",mondir);
		//	chdir(ckdir);
		scanmondirNEW(mondir,1);//1초 주기로 new스캔+base&new비교
		forlogtxt();
		//	chdir(curdir);

		if(update==1){//if update, scan base again.
		    update=0;//init
		    printf("update:%d\n",update);
		    scanmondirBASE(mondir,1);
		}

		printf("ENDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");

	    }

}
void forlogtxt(void){//cmp base&new 

    printf("-------------------------COMPARE STARTS----------------------\n");

    //MNode *crenode=(MNode*)malloc(sizeof(MNode));
    //memset(crenode,0,sizeof(crenode));
    int newfile,deleted,modified; 

    if(bfcnt==nfcnt){
	MNode *modnode;
	modnode=mhead;
	MnNode *Nmodnode;
	Nmodnode=mnhead;
	while(modnode!=NULL && Nmodnode!=NULL){
	    printf("mfname:n %s b %s\n",modnode->listfname,modnode->listfname);
	    printf("modnode->inum:%d Nmodnode->inum %d\n",modnode->inum,Nmodnode->inum);
	    printf("modnode->modtime:%s Nmodnode->modtime %s\n",modnode->mtime,Nmodnode->mtime);
	    printf("modnode->pathe:%s Nmodnode->mpppe %s\n",modnode->listfpath,Nmodnode->listfpath);
	    Nmodnode=Nmodnode->next;
	    modnode=modnode->next;
	}


	modified=0;
	if(update!=1){
	    modnode=mhead;
	    Nmodnode=mnhead;

	    while(modnode!=NULL && Nmodnode!=NULL){
		printf("mfname:n %s b %s\n",modnode->listfname,modnode->listfname);
		printf("modnode->inum:%d Nmodnode->inum %d\n",modnode->inum,Nmodnode->inum);
		//if(modnode->inum==Nmodnode->inum){//same file  
		if(strcmp(modnode->mtime,Nmodnode->mtime)){//but mtime change
		    write_logtxt(modnode->listfname,"modify",NULL);
		    update=1;
		    modified=1;
		    printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
		    break;
		}
		/* if(strcmp(modnode->listfpath,Nmodnode->listfpath)){//but path change
		   write_logtxt(modnode->listfname,"modify",NULL);
		   update=1;
		   modified=1;
		   printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
		   break;
		   }
		   if(strcmp(modnode->listfname,Nmodnode->listfname)){//fname change
		   write_logtxt(modnode->listfname,"modify",NULL);
		   update=1;
		   modified=1;
		   printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
		   break;
		   }*/
		//}
		Nmodnode=Nmodnode->next;
		modnode=modnode->next;
	    }
	    //free(modnode);
	    //free(Nmodnode);
	}
    }
    printf("lllllllllllllllllllllllllllllllllllllllllllllllllllli\n");

    if(bfcnt!=nfcnt){
	MNode *crenode;
	crenode=mhead;
	MnNode *Ncrenode;
	Ncrenode=mnhead;

	newfile=1;//CREATE LOG: new 기준으로 
	while(Ncrenode!=NULL){
	    while(crenode!=NULL){//create time fprint
		printf("Pfname:n %s b %s\n",Ncrenode->listfname,crenode->listfname); 
		if(Ncrenode->listfname!=crenode->listfname){//existing file in base
		    printf("CrE Ninum:%d   inum: %d\n",Ncrenode->inum,crenode->inum);
		    printf("fname:n %s b %s\n",Ncrenode->listfname,crenode->listfname); 
		    newfile=1;
		    write_logtxt(Ncrenode->listfname,"create",NULL);
		    update=1;
		    printf("CREATE LOG!!!!!!!!!!!!!!!!\n");
		    break;
		}
		crenode=crenode->next;
		printf("working create...\n");

		//if(newfile==1){//not existing file in base
	    }
	    Ncrenode=Ncrenode->next;
	    printf("next Nnode scan working create...\n");
	    // }
	}
	//free(crenode);
	//free(Ncrenode);
	if(update!=1&&modified!=1){
	    MNode *delnode;
	    delnode=mhead;
	    MnNode *Ndelnode;
	    Ndelnode=mnhead;

	    //deleted=1;//DELETE LOG: base 기준으로 
	    while(delnode){
		while(Ndelnode){
		    if(Ndelnode->listfname!=delnode->listfname){//existing file in new&base  
			printf("dell Nnum:%d   inum: %d\n",Ndelnode->inum,delnode->inum);
			//deleted=0;
			write_logtxt(delnode->listfname,"delete",NULL);
			update=1;
			printf("DELETE LOG!!!!!!!!!!!!!!!!\n");
			break;
		    }
		    Ndelnode=Ndelnode->next;


		    //  if(deleted==1){//not existing file in new 
		    // }
		    delnode=delnode->next;
		}
	    }
	    //free(delnode);
	    // free(Ndelnode);
	}
    }


    printf("-------------------------COMPARE ENDS----------------------\n");
}


int scanmondirBASE(char *searchdir,int inityes){
    printf("-------------------------scanning dir BASE STARTS\n");
    if(inityes==1){
	mnhead=NULL;////init first!   
	bfcnt=0;
    }

    char *dirptr;
    dirptr=searchdir+strlen(searchdir);
    *dirptr++='/';
    *dirptr='\0';
    struct tm *t;//시간값 표현하기 위한 구조체

    char dirpath[PATH_SIZE];
    char recurdirpath[PATH_SIZE];

    struct stat tempstat;
    // struct timeval *renamet;    
    int i;   
    char listfname[FILE_SIZE];
    int fsize;//SIZE
    int fcnt=0;

    struct stat buf;//SIZE   

    int countdirp=0;
    struct dirent **flist;

    if((countdirp=scandir(searchdir,&flist,0,alphasort))<0){
	fprintf(stderr,"scandir error for %s\n",searchdir);
	exit(1);
    }
    i=0;

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
	if(!strcmp(flist[i]->d_name,"log.txt")){
	    i++;
	    continue;
	}
	strcpy(dirptr,flist[i]->d_name);
	bfcnt+=1;

	if(!S_ISDIR(tempstat.st_mode)){
	    memset(node->listfname,0,PATH_SIZE);
	    strcpy(node->listfname,flist[i]->d_name);
	    printf("node->listfname:%s\n",node->listfname);

	    memset(node->listfpath,0,PATH_SIZE);
	    //strcpy(node->dirpath,searchdir);
	    strcpy(node->listfpath,searchdir);

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
	}

	if((buf.st_mode&S_IFDIR)==S_IFDIR){
	    //  sprintf(recurdirpath,"%s/%s",searchdir,flist[i]->d_name);

	    //indent++;
	    //strcpy(node->listfpath,temppath);
	    printf("listfpath:%sis directory\n",node->listfpath);

	    printf("~~~~~~~~~~~~~SCANDIR RECURSIVE FOR BASE~~~~~~~~~~~~~\n");

	    scanmondirBASE(searchdir,0);

	}

	i++;
    }
    // indent--;
    // chdir("..");
    dirptr[-1]=0;
    printf("-------------------------scanning dir BASE ENDS\n");
    return 0;
}
int scanmondirNEW(char *searchdir,int inityes){
    if(inityes==1){
	mnhead=NULL;////init first!    
	nfcnt=0;
    }

    printf("-------------------------scanning dir NEW STARTS\n");
    char *dirptr;
    dirptr=searchdir+strlen(searchdir);
    *dirptr++='/';
    *dirptr='\0';
    struct tm *t;//시간값 표현하기 위한 구조체

    char dirpath[PATH_SIZE];
    char recurdirpath[PATH_SIZE];

    struct stat tempstat;
    // struct timeval *renamet;    
    int i;   
    char listfname[FILE_SIZE];
    int fsize;//SIZE
    int fcnt=0;

    struct stat buf;//SIZE   

    int countdirp=0;
    struct dirent **flist;

    if((countdirp=scandir(searchdir,&flist,NULL,alphasort))<0){
	fprintf(stderr,"scandir error for %s\n",searchdir);
	//exit(1);
    }
    i=0;
    while(i<countdirp){
	MnNode *node=(MnNode*)malloc(sizeof(MnNode));
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
	if(!strcmp(flist[i]->d_name,"log.txt")){
	    i++;
	    continue;
	}
	strcpy(dirptr,flist[i]->d_name);
	nfcnt+=1;

	if(!S_ISDIR(tempstat.st_mode)){
	    memset(node->listfname,0,PATH_SIZE);
	    strcpy(node->listfname,flist[i]->d_name);
	    printf("node->listfname:%s\n",node->listfname);

	    memset(node->listfpath,0,PATH_SIZE);
	    //strcpy(node->dirpath,searchdir);
	    strcpy(node->listfpath,searchdir);

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

	    Mnlist_insert(node);
	}

	if((buf.st_mode&S_IFDIR)==S_IFDIR){
	    //  sprintf(recurdirpath,"%s/%s",searchdir,flist[i]->d_name);

	    //indent++;
	    //strcpy(node->listfpath,temppath);
	    printf("listfpath:%sis directory\n",node->listfpath);

	    printf("~~~~~~~~~~~~~SCANDIR RECURSIVE FOR NEW~~~~~~~~~~~~~\n");

	    scanmondirNEW(searchdir,0);

	}

	i++;
    }
    // indent--;
    // chdir("..");
    dirptr[-1]=0;
    printf("-------------------------scanning dir NEW ENDS\n");
    return 0;
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
void Mnlist_insert(MnNode *newNode){//list에 node추가
    newNode->next=NULL;
    if(mnhead==NULL)
	mnhead=newNode;
    else{
	MnNode *listF;
	listF=mnhead;
	while(listF->next!=NULL)
	    listF=listF->next;
	listF->next=newNode;
    }
}
void get_time(char *str,char *status){
    char timestr[TM_SIZE];
    time_t timer=time(NULL);
    struct tm *t=localtime(&timer);

    memset(timestr,0,TM_SIZE);
    memset(str,0,TM_SIZE);

    sprintf(timestr,"[%04d-%02d-%02d %02d:%02d:%02d]",t->tm_year + 1900, t->tm_mon+1,t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    /*if(!strcmp(status,"create")){
      t=localtime(&tempstat.st_ctime);
      sprintf("[%04d-%02d-%02d %02d:%02d:%02d]",t->tm_year + 1900, t->tm_mon+1,t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
      }
      if(!strcmp(status,"modify")){
      t=localtime(&buf.st_mtime);
      fprintf("[%04d-%02d-%02d %02d:%02d:%02d]", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
      }*/

    strcpy(str,timestr);
}
void write_logtxt(char *fname, char *status,char *mtimeifmod){
    //함수호출전에 반드시 curdir로, 함수호출후에 원래workingdir으로 반드시 바꿔놓을것.
    char timestr[TM_SIZE];
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    memset(wdir,0,PATH_SIZE);
    memset(mondir,0,PATH_SIZE);
    getcwd(wdir,PATH_SIZE);
    sprintf(mondir,"%s/check",wdir);
    chdir(mondir);
    FILE *fp;
    char *logfname="log.txt";
    if((fp=fopen(logfname,"r+w"))<0){
	fprintf(stderr,"fopen %s error",fname);
	exit(1);
    }
    //if(!strcmp(status,"modify"))
    //	strcpy(timestr,mtimeifmod);
    //  else
    get_time(timestr,status);//current time

    fseek(fp,0,SEEK_END);
    fprintf(fp,"%s [%s _%s]\n",timestr,fname,status);
    fclose(fp);
}
