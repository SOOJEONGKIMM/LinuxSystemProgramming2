#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
//int getc(FILE*fp);
//한번에 하나의 문자를 읽는 라이브러리 함수. 주어진 파일에서 한문자씩 읽음.
//파일 스트림이 가리키는 바로 다음 문자를 읽어서 원래 데이터 타입인 unsigned char를 int로 변환한 다음 리턴값으로 돌려줌. 
//만약 파일끝에 도달했거나 에러발생 경우 EOF리턴. 
int main(void){
    int character;
    //getc()호출해 표준입력으로 입력 받음. 
    while((character=getc(stdin))!=EOF){
	//파일끝 도달하면 EOF리턴. 
	//입력받은 문자열을 putc()호출해 표준출력으로 출력. 
	if(putc(character,stdout)==EOF){
	    fprintf(stderr,"standard output error\n");
	    exit(1);
	}
    }
    //만약 에러가 있다면 에러문장 출력하고 프로그램종료.
    if(ferror(stdin)){
	fprintf(stderr,"standard input error\n");
	exit(1);
    }
    exit(0);
}
