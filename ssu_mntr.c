#include "./ssu_mntr.h"
int deleteOpt=0;
int sizeOpt=0;
int recoverOpt=0;
int treeOpt=0;
int exitOpt=0;
int helpOpt=0;
int invalidOpt=0;

int deloptI=0;
int deloptR=0;

void ssu_mntr_play(void){
    char cmdbuf[BUFFER_SIZE];
    int cmd;


    while(1){
	memset((char*)cmdbuf,0,BUFFER_SIZE);

	//printf("d:%d,s:%d,r:%d,t:%d,e:%d,h:%d,inv:%d",deleteOpt,sizeOpt,recoverOpt,treeOpt,exitOpt,helpOpt,invalidOpt);//debug
	printf("20162969>");
	fgets(cmdbuf,BUFFER_SIZE,stdin);

	if(cmdbuf[0]=='\n'){//엔터만 입력시 프롬프트 재출력
	    printf("entered");
	    continue;
	}
	cmdbuf[strlen(cmdbuf)-1]=0;//개행문자제거 
	check_opt(cmdbuf);
	if(deleteOpt&!sizeOpt&!recoverOpt&!treeOpt&!exitOpt&!helpOpt&!invalidOpt){
	    deleteOpt=0;
	    get_deleteOpt(cmdbuf);
	}
	else if(!deleteOpt&sizeOpt&!recoverOpt&!treeOpt&!exitOpt&!helpOpt&!invalidOpt){
	    sizeOpt=0;
	    do_sizeOpt(cmdbuf);
	}
	else if(!deleteOpt&!sizeOpt&recoverOpt&!treeOpt&!exitOpt&!helpOpt&!invalidOpt){
	    recoverOpt=0;
	    do_recoverOpt(cmdbuf);
	}
	else if(!deleteOpt&!sizeOpt&!recoverOpt&treeOpt&!exitOpt&!helpOpt&!invalidOpt){
	    treeOpt=0;
	    do_treeOpt(cmdbuf);
	}
	else if(!deleteOpt&!sizeOpt&!recoverOpt&!treeOpt&exitOpt&!helpOpt&!invalidOpt){
	    exit(0);
	}
	else if(!deleteOpt&!sizeOpt&!recoverOpt&!treeOpt&!exitOpt&helpOpt&!invalidOpt){
	    helpOpt=0;
	    do_helpOpt(cmdbuf);
	}
	else if(!deleteOpt&!sizeOpt&!recoverOpt&!treeOpt&!exitOpt&!helpOpt&invalidOpt){
	    invalidOpt=0;
	    do_helpOpt(cmdbuf);
	    continue;
	}
    }
}
int check_opt(const char *str){
    char trim[31];
    char cmdbuf[BUFFER_SIZE];
    memset((char*)trim,0,sizeof((char*)trim));
    for(int i=0;i<strlen(str)&&str[i]!=' ';i++){
	trim[i]=str[i];
    }
    while(1){
	if(!strcmp(trim,"delete")){
	    deleteOpt=1;
	    break;
	}
	if(!strcmp(trim,"size")){
	    sizeOpt=1;
	    break;
	}
	if(!strcmp(trim,"recover")){
	    recoverOpt=1;
	    break;
	}
	if(!strcmp(trim,"tree")){
	    treeOpt=1;
	    break;
	}
	if(!strcmp(trim,"exit")){
	    exitOpt=1;
	    break;
	}
	if(!strcmp(trim,"help")){
	    helpOpt=1;
	    break;
	}
	else{
	    invalidOpt=1;
	    return -1;
	}
    }
    return 1;

}

int get_deleteOpt(char *str){//DELETE [FILENAME] [ENDTIME] [OPTION]
    char trashdir[PATH_SIZE];
    char filesdir[PATH_SIZE];//backup deleting file
    char infodir[PATH_SIZE];//deleting file info dir
    char infotxt[PATH_SIZE];//onlyfname을 info dir 경로로 
    char curdir[PATH_SIZE];
    char checkdir[PATH_SIZE];
    char fnamepath[PATH_SIZE];//deleting file name path
    //    char onlyfname[PATH_SIZE];//deleting file name
    char newdir[PATH_SIZE];//newdir path buf for 'files'dir
    char endtimestr[TM_SIZE];
    char enddatestr[TM_SIZE];
    char delopt[OPT_SIZE];
    struct stat statbuf;
    struct stat infobuf;
    // struct timeval *renamet;
    struct tm delt;
    struct tm *t;
    int i;
    int endtimeExist=1;
    deloptI=deloptR=0;//init delopt
    FILE *fp;

    int len=strlen(str);
    //trim option cmd part
    for(i=0;i<strlen(str);i++){
	if(str[i]==' ')
	    break;  
    }
    while(i<len && str[i]==' ')
	i++;
    memset((char*)onlyfname,0,PATH_SIZE);
    //인자로 얻은 str에서 [FILENAME]만 추출
    int a=0;
    int j;
    for(j=i;j<len && str[j]!=' ';j++){
	onlyfname[a]=str[j];
	a++;
	i++;
    }
    printf("onlyfname:%s",onlyfname);
    i=j;
    //[ENDTIME] 추출 
    while(i<len && str[i]==' ')
	i++;
    if(i>=len){//[ENDTIME]주어지지않은경우
	endtimeExist=0;
	printf("\nendTE:%d\n",endtimeExist);
    }
    else{
	endtimeExist=1;
	a=0;
	for(j=i;j<len && str[j]!=' ';j++){//2020-05-05
	    if(str[j]=='-'&&str[j+1]=='i'){//[ENDTIME]없이 바로 -i만 들어온 경우
		endtimeExist=0;
		printf("came here\n");
		break;
	    }
	    enddatestr[a]=str[j];
	    //   printf("enddate:%c\n",enddatestr[a]);
	    a++;
	    i++;
	}
	if(endtimeExist==1){
	    i=j;
	    while(i<len && str[i]==' ')
		i++;
	    a=0;
	    for(j=i;j<len && str[j]!=' ';j++){//10:00
		endtimestr[a]=str[j];
		a++;
		i++;
	    }
	    i=j;
	    printf("enddatestr:%s\n",enddatestr);
	    printf("endtimestr:%s\n",endtimestr);
	}
    }
    //[OPTION] 추출 
    i=j;
    while(i<len && str[i]==' ')
	i++;
    a=0;
    memset(delopt,0,OPT_SIZE);
    for(j=i;j<len && str[j]!=' ';j++){//-r -i
	delopt[a]=str[j];
	a++;
	i++;
    }
    printf("delopt:%s\n",delopt);
    i=j;
    if(!strcmp(delopt,"-i")){
	deloptI=1;//전역변수로 선언하고, do_deleteOpt()에서 옵션수행할때 다시 플래그 0으로 초기화할것.
    }
    if(!strcmp(delopt,"-r")){
	deloptR=1;
    }

    printf("deloptR:%d\n",deloptR);
    printf("\nendTE:%d",endtimeExist);
    if(endtimeExist==0)
	do_deleteOpt();
    else{
	//	char alarmTmin[TM_SIZE];
	char endtimehour[TM_SIZE];
	char endtimemin[TM_SIZE];
	struct tm alarmsetT;
	int waitmin=0;
	printf("setting alarm\n");
	time_t alarmsett=time(NULL);
	alarmsetT=*localtime(&alarmsett);
	printf("alarmsetT:%d일 %d:%d:%d\n",alarmsetT.tm_mday,alarmsetT.tm_hour,alarmsetT.tm_min,alarmsetT.tm_sec);
	//sprintf(alarmTmin,"%d",alarmsetT.tm_min);
	j=0;
	int i=0;
	for(i=0;i<strlen(endtimestr)&&endtimestr[i]!=':';i++){//10:30 hour
	    endtimehour[i]=endtimestr[i];
	    j++;
	}
	printf("Pendtimehour:%s\n",endtimehour);
	while(i<strlen(endtimestr) && endtimestr[i]==':')
	    i++;
	//j=0;
	for(int i=0;i<strlen(endtimestr)&&endtimestr[i]!=' ';i++){//10:30 min
	    endtimemin[i]=endtimestr[j+1];
	    j++;
	}
	int endtimeminInt=atoi(endtimemin);
	waitmin=endtimeminInt-alarmsetT.tm_min;
	if(waitmin<0){
	    printf("[ENDTIME] is set wrong.\n");
	    ssu_mntr_play();
	}
	printf("waitSec:%d\n",waitmin*60);
	int k=3;
	signal(SIGALRM,deloptR_alarm);
	alarm(waitmin*60);//예약시간이 되면 alarm이 SIGALRM을 전송한다.


	printf("after alarm\n");
    }



    return 0;
}

//endtimestr[] ENDTIME예정시각에 
//do_deleteOpt()호출 
void deloptR_alarm(int k){
    pid_t pid;
    int status;
    char recheckbuf[OPT_SIZE];
    printf("alarm ringing\n");
    //vfork 생성 (자식/부모프로세스에서 삭제작업 진행)
    switch(pid=vfork()){
	case 0:
	    printf("I'm child. My PID is %d\n",getpid());
	    if(deloptR==1){//do_deleteOpt()호출 전 재확인 문구
		while(1){
		    printf("Delete [y/n]?");
		    memset(recheckbuf,0,OPT_SIZE);
		    fgets(recheckbuf,OPT_SIZE,stdin);
		    recheckbuf[strlen(recheckbuf)-1]=0;//개행문자제거 
		    if(!strcmp(recheckbuf,"y"))
			do_deleteOpt();
		    else if(!strcmp(recheckbuf,"n"))
			ssu_mntr_play();
		    else
			continue;
		}
	    }
	    else{
		printf("no -r option, working on delete in ENDTIME..\n");
		do_deleteOpt();
	    }
	    //case -1:
	    //	fprintf(stderr,"vfork error\n");
	default: //parent process
	    while(wait(&status)!=pid)
		continue;
	    break;
    }


    //after Ropt, kill vfork (부모가 죽지 않게 하기 위해서)
    // kill(getpid(),SIGKILL);
}


int do_deleteOpt(void){//DELETE [FILENAME] [ENDTIME] [OPTION]
    char trashdir[PATH_SIZE];
    char filesdir[PATH_SIZE];//backup deleting file
    char infodir[PATH_SIZE];//deleting file info dir
    char infotxt[PATH_SIZE];//onlyfname을 info dir 경로로 
    char curdir[PATH_SIZE];
    char checkdir[PATH_SIZE];
    char fnamepath[PATH_SIZE];//deleting file name path
    //    char onlyfname[PATH_SIZE];//deleting file name
    char newdir[PATH_SIZE];//newdir path buf for 'files'dir
    struct stat statbuf;
    struct stat infobuf;
    // struct timeval *renamet;
    struct tm delt;
    struct tm *t;
    int i;
    FILE *fp;
    strcpy(curdir,getcwd(NULL,0));//***절대경로 상대경로 입력시 모두 동작하도록 수정해야함
    sprintf(checkdir,"%s/check",curdir);
    //strcpy(checkdir,getcwd(NULL,0));
    printf("curdir:%s\n",curdir);
    printf("deloptI:%d\n",deloptI);
    if(chdir(checkdir)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",checkdir);
	perror("chdir");
	exit(1);
    }

    if(access(onlyfname,F_OK)!=0){
	printf("%s is not a existing file\n",onlyfname);
	//ssu_mntr_play();
    }
    //삭제파일의 절대경로 가져옴
    if(realpath(onlyfname,fnamepath)==NULL){
	fprintf(stderr,"realpath error for %s\n",onlyfname);
	//exit(1);
    }
    //입력받은 파일이 정상적으로 존재하는 파일인지 확인
    if(lstat(fnamepath,&statbuf)<0){
	fprintf(stderr,"lstat error for %s\n",fnamepath);
	//exit(1);
    }
    if(!S_ISREG(statbuf.st_mode)){
	fprintf(stderr,"stat error for %s, not a regular file\n",fnamepath);
	//exit(1);
    } 
    if(deloptI==1){
	int delI=remove(fnamepath);
	if(delI==0)
	    printf("%s deleted with i option.\n",fnamepath);
	else
	    printf("%s failed to delete with i option.\n",fnamepath);
    }
    else if(deloptI==0){
	printf("entered here");
	//trash dir 이름을 지정해서 생성.
	if(getcwd(trashdir,PATH_SIZE)==0){
	    fprintf(stderr,"getcwd error to %s\n",trashdir);
	    exit(1);
	}
	printf("trashdir:%s\n",trashdir);
	sprintf(trashdir,"%s/trash",curdir);
	mkdir(trashdir,0744);
	//trashdir로 이동
	if(chdir(trashdir)<0){//returns 0 if success
	    fprintf(stderr,"DIR:%s can't be found.\n",trashdir);
	    perror("chdir");
	    exit(1);
	}
	//files dir 서브디렉토리로 생성 
	if(getcwd(filesdir,PATH_SIZE)==0){
	    fprintf(stderr,"getcwd error to %s\n",filesdir);
	    exit(1);
	}
	sprintf(filesdir,"%s/files",filesdir);
	mkdir(filesdir,0744);
	sprintf(newdir,"%s/%s",filesdir,onlyfname);
	//files디렉토리로 DELETE명령어로 지운 파일 자체 저장
	if(rename(fnamepath,newdir)<0){
	    fprintf(stderr,"rename error for %s to %s\n",fnamepath,newdir);
	}
	//파일 삭제 시간 저장.(info dir파일정보 출력시 사용)
	if(stat(fnamepath,&infobuf)<0){
	    time_t t=time(NULL);
	    delt=*localtime(&t);
	}
	//files dir에 작업 마치고 다시 trashdir로 이동
	if(chdir(trashdir)<0){//returns 0 if success
	    fprintf(stderr,"DIR:%s can't be found.\n",trashdir);
	    perror("chdir");
	    exit(1);
	}
	//info dir 서브디렉토리로 생성  
	if(getcwd(infodir,PATH_SIZE)==0){
	    fprintf(stderr,"getcwd error to %s\n",infodir);
	    exit(1);
	}
	sprintf(infodir,"%s/info",infodir);
	mkdir(infodir,0744);
	//info dir에 DELETE명령어로 지운 파일정보를 담은 파일 저장
	//(이름은 절대경로 제외 파일이름으로)
	sprintf(infotxt,"%s/%s",infodir,onlyfname);
	if((fp=fopen(infotxt,"a"))<0){
	    fprintf(stderr,"fopen error for %s\n",infotxt);
	    exit(1);
	}
	if(stat(infotxt,&infobuf)<0){
	    fprintf(stderr,"lstat error for %s\n",infotxt);
	    exit(1);
	}
	fprintf(fp,"[Trash info]\n");
	fprintf(fp,"%s\n",fnamepath);//지운 파일 절대경로 
	//파일 삭제 시간
	fprintf(fp,"D : %04d-%02d-%02d %02d:%02d:%02d\n", delt.tm_year+1900,delt.tm_mon+1,delt.tm_mday,delt.tm_hour,delt.tm_min,delt.tm_sec);
	//파일 최종 수정시간
	t=localtime(&infobuf.st_mtime);
	fprintf(fp,"M : %04d-%02d-%02d %02d:%02d:%02d\n", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);

	fclose(fp);
    }



    return 0;
}

int do_sizeOpt(char *str){//SIZE [FILENAME] [OPTION]

    printf("sizeopt");
    return 0;
}

int do_recoverOpt(char *str){

    printf("recoveropt");

    return 0;
}

int do_treeOpt(char *str){
    printf("treeopt");

    return 0;
}

int do_helpOpt(char *str){

    printf("helpopt");
    return 0;
}
