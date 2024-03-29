# Linux System Programming
# Project 2

○ ssu_mntr 프로그램 기본 사항
- ssu_mntr 프로그램은 지정한 특정 디렉토리의 파일의 변경 상태 모니터링
ü 지정한 디렉토리는 제출할 소스코드 디렉토리 밑에 서브디렉토리로 생성
ü 모니터링 프로그램은 별도 생성한 디몬 프로세스가 담당
ü 파일이 생성, 삭제, 수정될 경우, 학번 서브디렉토리 밑에 있는 “log.txt”파일에 변경 사항을
추가

ü log.txt 파일 구조
- [생성 or 삭제 or 수정 시간] [수행내용_파일이름] 형태로 작성
- 새로운 파일 변경 사항은 “log.txt”파일 밑에 추가
- 파일 이름에는 한글 지원하지 않아도 됨
ü 모니터링이 종료될 경우, 종료 메시지(학생이 원하는 종료 메시지 마음대로)를 출력한 후 종료.

○ 설계 및 구현
- 가) ssu_mntr 실행 후 프롬프트 출력
ü 프롬프트 모양 : “학번>” 문자 출력. ex) 20201234>
ü 프롬프트에서는 delete, size, recover, tree, exit, help 명령어만 수행
ü 이외의 명령어 수행 시 자동으로 help를 실행시킨 것과 동일하게 표준 출력 후 프롬프트 출력
ü 엔터만 입력 시 프롬프트 재출력

- 나) DELETE [FILENAME] [END_TIME] [OPTION]
ü 지정한 삭제 시간에 자동으로 파일을 삭제해주는 명령어
ü 삭제한 파일은 제출할 소스코드 디렉토리 밑에 있는 'trash' 디렉토리로 이동
ü “trash” 디렉토리가 없을 경우 생성해야 함.
ü trash 디렉토리
- trash 디렉토리 밑에 두 개의 서브디렉토리, files, info 디렉토리를 생성
Ÿ “files” 디렉토리 : DELETE 명령어로 지운 파일 자체 저장 (rename으로 경로 변경)
Ÿ “info” 디렉토리 : DELETE 명령어로 지운 파일의 정보(절대경로, 삭제 시간, 최종 수정시간
(mtime))를 저장. 단 저장될 파일 이름은 절대경로를 제외하고 최종 파일 이름만 포함됨.
- info 디렉토리의 크기가 정해진 크기(2KB)를 초과할 경우 오래된 파일부터 files 디렉토리의
원본 파일과 함께 info 디렉토리 파일 정보까지 삭제
ü FILENAME
- 파일의 경로(절대경로와 상대경로 모두 가능해야함)
- FILENAME 입력이 없거나 존재하지 않을 경우, 에러처리 후 프롬프트로 출력
- 삭제할 파일은 지정한 특정 디렉토리 안에 있는 파일들로 한정함
ü END_TIME
- 삭제할 파일의 삭제 예정 시간 ex) 20201234> DELETE FILELNAME 2020-05-05 10:00 (5월 5
일 10시 삭제)
- 단, 제출 실행 예제는 테스트가 쉽게 가능하면 3분 이내 시간을 사용하기 바람.
- END_TIME을 주어주지 않을 경우 바로 삭제함
ü OPTION
- -i : 삭제 시 'trash' 디렉토리로 삭제 파일과 정보를 이동시키지 않고 파일 삭제
- -r : 지정한 시간에 삭제 시 삭제 여부 재확인

- 다) SIZE [FILENAME] [OPTION]
ü 파일경로(상대경로), 파일 크기 출력하는 명령어
ü 기본 크기는 byte단위임
ü 출력하는 파일 및 디렉토리는 문자열 오름차순으로 출력해야함
ü FILENAME
- 디렉토리와 파일이 모두 가능하며 FILENAME으로 입력 시 크기가 모두 표시되어야함
- 파일경로는 상대경로로 출력해야함
예. SIZE의 출력 결과
ü OPTION
- -d NUMBER : NUMBER 단계만큼의 하위 디렉토리까지 출력
Ÿ NUMBER의 수보다 하위 디렉토리의 수가 적을 경우, 최대 하위 디렉토리까지 출력

- 라) RECOVER [FILENAME] [OPTION]
ü “trash”디렉토리 안에 있는 파일을 원래 경로로 복구하는 명령어
ü 동일한 이름의 파일이 “trash”디렉토리에 여러 개 있을 경우 이를 표준 출력으로 파일 이름,
삭제 시간, 수정 시간을 보여줌
ü 복구 시 이름이 중복된다면 파일의 처음에 “숫자_”를 추가
ü FILENAME
- 해당 파일이 없거나 파일의 복구 경로가 없다면 에러처리 후 프롬프트 전환
ü OPTION
- -l : ‘trash’디렉토리 밑에 있는 파일과 삭제 시간들을 삭제 시간이 오래된 순으로 출력 후,
명령어 진행

- 마) TREE
ü “check”디렉토리의 구조를 tree 형태로 보여주는 명령어
ü 꼭 이 형태를 따르지 않아도 되나 지정한 디렉토리의 구조를 그림으로 보여주어야 함
ü 아래 예시에서는 ~lsp/test/20162495/check 디렉토리가 파일의 변경 상태를 모니터링을 하기 위
해 지정한 특정 디렉토리임

- 바) EXIT
ü 프로그램 종료시키는 명령어
ü EXIT 명령어 실행 시 명령어를 실행시키는 프로세스와 지정한 디렉토리를 모니터링하는 프로
세스 모두 종료되어야함.

- 사) HELP
ü 명령어 사용법을 출력하는 명령어
