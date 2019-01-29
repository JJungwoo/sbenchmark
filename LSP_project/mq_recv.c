#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
 
typedef struct mymsgbuf {
    long mtype;
    char mtext[80];
}mymsgbuf;
 
int main(void) {
    mymsgbuf inmsg;
    key_t key;
    int msgid, len;
 
    // msgsnd 예제와 같은 경로명과 정수값을 사용해 키를 생성한다.
    key = ftok("keyfile", 1);
    // msgget 함수의 두번째 인자를 0으로 지정해 기존 메시지 큐의 식별자를 리턴하게 한다.
    if((msgid = msgget(key, 0)) < 0) {
        perror("msgget");
        exit(1);
    }
 
    // msgrcv 함수를 사용해 메시지를 읽어온다.
    // 버퍼의 크기는 80바이트로 지정하고, 큐가 비었을 경우 기다리도록 지정한다.
    len = msgrcv(msgid, &inmsg, 80, 0, 0);
    printf("Received Msg = %s, Len=%d\n", inmsg.mtext, len);
 
    return 0;
}

