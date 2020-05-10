#include"./ssu_mntr.h"


//void scanmondir(char *searchdir,int depth,int sizeoptflag,int indentinit,char *delcurdir);
int update;

int main(void){
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    int idx=0;
    //    char logpath[PATH_SIZE];//log.txt path name
    //    pid_t pid;

    memset(wdir,0,PATH_SIZE);
    memset(mondir,0,PATH_SIZE);

    getcwd(wdir,PATH_SIZE);//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    strcpy(basedir,wdir);
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
    scanmondirBASE(mondir,1,binfo[BUFFER_SIZE],idx);
    //chdir(wdir);
    startdemon(wdir,mondir);

    //옵션입력으로 넘어감.
    return 0;
}
void startdemon(char *curdir,char *checkdir){
    pid_t pid;
    char wdir[PATH_SIZE];
    char mondir[PATH_SIZE];
    memset(wdir,0,PATH_SIZE);
    memset(mondir,0,PATH_SIZE);
    getcwd(wdir,PATH_SIZE);//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    sprintf(mondir,"%s/check",wdir);
    //Neww neww;
    //se base;
    struct Base binfo[BUFFER_SIZE];
    struct Neww ninfo[BUFFER_SIZE];
    /*    int fd, maxfd;

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
    if(setsid()==-1)
    exit(0);

    fd=open("/dev/null",O_RDWR);
    dup(0);
    dup(0);
    */
    /*  switch(pid=fork()){
	case 0:
	printf("I'm child. My PID is %d\n",getpid());*/
    while(1){
	//sleep(1);
	printf("++++++++++++++++++++++++++++++++++whileloop++++++++++++++++++++\n");
	printf("check:%s\n",mondir);
	//	chdir(ckdir);
	idx;
	scanmondirNEW(mondir,1,ninfo[BUFFER_SIZE],idx);//1초 주기로 new스캔+base&new비교
	//	chdir(curdir);
	//forlogtxt();

	if(update==1){//if update, scan base again.
	    update=0;//init
	    idx=0;
	    printf("update:%d\n",update);
    scanmondirBASE(mondir,1,binfo[BUFFER_SIZE],idx);
	}

	printf("ENDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD");

    }
    //	default:*/
    ssu_mntr_play();
    //    }

}
/*void forlogtxt(void){//cmp base&new 

  printf("-------------------------COMPARE STARTS----------------------\n");

//MNode *crenode=(MNode*)malloc(sizeof(MNode));
//memset(crenode,0,sizeof(crenode));
int newfile,deleted,modified; 
/*    MNode *crenode;
crenode=mhead;
MnNode *Ncrenode;
Ncrenode=mnhead;



modified=0;
if(update!=1){
struct MNode *result=NULL;
MNode *modnode;
modnode=mhead;
MnNode *Nmodnode;
Nmodnode=mnhead;

while(modnode){
push(&result, modnode->mtime);
modnode=modnode->next;
}

while(Nmodnode!=NULL){

//printf("mfname:n %s b %s\n",Ncrenode->listfname,crenode->listfname);
//printf("mfpath:n %s b %s\n",crenode->listfpath,crenode->listfpath);
//	printf("modnode->inum:%d Nmodnode->inum %d\n",modnode->inum,Nmodnode->inum);

if(isPresent(result,Nmodnode->mtime)){//but mtime change
write_logtxt(modnode->listfname,"modify",NULL);
update=1;
modified=1;
printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
break;
}
/*   if(strcmp(modnode->listfpath,Nmodnode->listfpath)){//but path change
write_logtxt(modnode->listfname,"modify",NULL);
update=1;
modified=1;
printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
break;
}*/
/*  if(strcmp(modnode->listfname,Nmodnode->listfname)){//fname change
    write_logtxt(modnode->listfname,"modify",NULL);
    update=1;
    modified=1;
    printf("MODIFY LOG!!!!!!!!!!!!!!!!\n");
    break;
    }*/

/*Nmodnode=Nmodnode->next;

  }
//free(modnode);
//free(Nmodnode);*/
/*  }
    newfile=1;//CREATE LOG: new 기준으로 
    while(Ncrenode){
    printf("what\n");
    while(crenode){//create time fprint
    printf("is wrong?\n");

    if(Ncrenode->inum==crenode->inum){//existing file in base
    printf("CrE Ninum:%d   inum: %d\n",Ncrenode->inum,crenode->inum);
    printf("fname:n %s b %s\n",Ncrenode->listfname,crenode->listfname); 
    newfile=0;
    }
    crenode=crenode->next;
    printf("working create...\n");

    if(newfile==1){//not existing file in base
    write_logtxt(Ncrenode->listfname,"create",NULL);
    update=1;
    printf("CREATE LOG!!!!!!!!!!!!!!!!\n");
    }
    printf("next Nnode scan working create...\n");
    }
    Ncrenode=Ncrenode->next;
    printf("Pfname:n %s b %s\n",Ncrenode->listfname,crenode->listfname); 
    }
//free(crenode);
//free(Ncrenode);
if(update!=1&&modified!=1){
MNode *delnode;
delnode=mhead;
MnNode *Ndelnode;
Ndelnode=mnhead;

deleted=1;//DELETE LOG: base 기준으로 
while(delnode){
while(Ndelnode){
if(Ndelnode->inum==delnode->inum){//existing file in new&base  
printf("dell Nnum:%d   inum: %d\n",Ndelnode->inum,delnode->inum);
deleted=0;
}
Ndelnode=Ndelnode->next;


if(deleted==1){//not existing file in new 
write_logtxt(delnode->listfname,"delete",NULL);
update=1;
printf("DELETE LOG!!!!!!!!!!!!!!!!\n");
break;
}
}
delnode=delnode->next;
}
//free(delnode);
// free(Ndelnode);
}


printf("-------------------------COMPARE ENDS----------------------\n");
}*/

int scanmondirBASE(char *searchdir,int inityes,binfo[BUFFER_SIZE],int idx){
    printf("-------------------------scanning dir BASE STARTS\n");
    if(inityes==1)
	mnhead=NULL;////init first!   

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
	//	MNode *node=(MNode*)malloc(sizeof(MNode));
	//	memset(node,0,sizeof(node));
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

	//memset(node->listfname,0,PATH_SIZE);
	strcpy(base[*idx].listfname,flist[i]->d_name);
	printf("node->listfname:%s\n",base[*indx].listfname);

	//	if(S_ISREG(tempstat.st_mode)){
	//memset(node->listfpath,0,PATH_SIZE);
	//strcpy(node->dirpath,searchdir);
	strcpy(base[*idx].listfpath,searchdir);

	fsize=0;
	fsize=stat(base[*idx].listfpath,&buf);//SIZE 


	//node->fsize=0;
	base[*idx].fsize=buf.st_size;//SIZE

	//memset(node->mtime,0,TM_SIZE);
	strcpy(base[*idx].mtime,ctime(&buf.st_mtime));
	printf("mtime:%s\n",base[*idx].mtime);

	base[*idx].inum=buf.st_ino;//inode num


	printf("fize(buf.st_size):%d\n",base[*idx].fsize);
	printf("listfpath:%s\n",base[*idx].listfpath);

	*idx+=1;
	//	Mlist_insert(node);
	//	}

	if((buf.st_mode&S_IFDIR)==S_IFDIR){
	    //  sprintf(recurdirpath,"%s/%s",searchdir,flist[i]->d_name);

	    //indent++;
	    //strcpy(node->listfpath,temppath);
	    //	    printf("listfpath:%sis directory\n",node->listfpath);

	    printf("~~~~~~~~~~~~~SCANDIR RECURSIVE FOR BASE~~~~~~~~~~~~~\n");

	    scanmondirBASE(searchdir,0,idx);

	}

	i++;
    }
    /*for(int j=0;j<countdirp;j++){
      free(flist[j]);
      }
      free(flist[i]);*/

    // indent--;
    // chdir("..");
    dirptr[-1]=0;
    printf("-------------------------scanning dir BASE ENDS\n");
    return 0;
}
int scanmondirNEW(char *searchdir,int inityes,ninfo[BUFFER_SIZE],int idx){
    printf("-------------------------scanning dir BASE STARTS\n");
    if(inityes==1)
	mnhead=NULL;////init first!   

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
	//	MNode *node=(MNode*)malloc(sizeof(MNode));
	//	memset(node,0,sizeof(node));
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

	//memset(node->listfname,0,PATH_SIZE);
	strcpy(neww[*idx].listfname,flist[i]->d_name);
	printf("node->listfname:%s\n",neww[*indx].listfname);

	//	if(S_ISREG(tempstat.st_mode)){
	//memset(node->listfpath,0,PATH_SIZE);
	//strcpy(node->dirpath,searchdir);
	strcpy(neww[*idx].listfpath,searchdir);

	fsize=0;
	fsize=stat(neww[*idx].listfpath,&buf);//SIZE 


	//node->fsize=0;
	neww[*idx].fsize=buf.st_size;//SIZE

	//memset(node->mtime,0,TM_SIZE);
	strcpy(neww[*idx].mtime,ctime(&buf.st_mtime));
	printf("mtime:%s\n",neww[*idx].mtime);

	neww[*idx].inum=buf.st_ino;//inode num


	printf("fize(buf.st_size):%d\n",neww[*idx].fsize);
	printf("listfpath:%s\n",neww[*idx].listfpath);

	*idx+=1;
	//	Mlist_insert(node);
	//	}

	if((buf.st_mode&S_IFDIR)==S_IFDIR){
	    //  sprintf(recurdirpath,"%s/%s",searchdir,flist[i]->d_name);

	    //indent++;
	    //strcpy(node->listfpath,temppath);
	    //	    printf("listfpath:%sis directory\n",node->listfpath);

	    printf("~~~~~~~~~~~~~SCANDIR RECURSIVE FOR BASE~~~~~~~~~~~~~\n");

	    scanmondirNEW(searchdir,0,idx);

	}

	i++;
    }
    /*for(int j=0;j<countdirp;j++){
      free(flist[j]);
      }
      free(flist[i]);*/

    // indent--;
    // chdir("..");
    dirptr[-1]=0;
    printf("-------------------------scanning dir BASE ENDS\n");
    return 0;
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
    if(!strcmp(status,"modify"))
	strcpy(timestr,mtimeifmod);
    else
	get_time(timestr,status);//current time

    fseek(fp,0,SEEK_END);
    fprintf(fp,"%s [%s _%s]\n",timestr,fname,status);
    fclose(fp);
}
