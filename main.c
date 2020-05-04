#include"./ssu_mntr.h"

int main(int argc, char *argv[]){
    int fd;
    char logpath[PATH_SIZE];//log.txt path name
    char wdir[PATH_SIZE];//current dir
    char mondir[PATH_SIZE];//monitoring dir(sub dir)
    /*//현재 작업 디렉토리 경로 받음.
    strcpy(wdir,getcwd(NULL,0));

    //current dir로 이동.***사실 필요없는 작업 같음.
    if(chdir(wdir)<0){//returns 0 if success
	fprintf(stderr,"DIR:%s can't be found.\n",wdir);
	perror("chdir");
	exit(1);
    }*/
    if(getcwd(mondir,PATH_SIZE)==0){
	fprintf(stderr,"getcwd error to %s\n",mondir);
	exit(1);
    }
    //백업디렉토리(서브디렉토리) 이름을 지정해서 생성.
    sprintf(mondir,"%s/check",mondir);
    mkdir(mondir,0744);

    sprintf(logpath,"%s/log.txt",mondir);
    if((fd=open(logpath,O_RDWR|O_CREAT|O_TRUNC,0644))<0){
	fprintf(stderr,"open error for %s\n",logpath);
	exit(1);
    }
    close(fd);
    ssu_mntr_play();//옵션입력으로 넘어감.
    exit(0);
}

