#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
// 메시지 버퍼로 사용할 구조체를 문법에 따라 정의한다.
// 메시지를 저장할 배열의 크기는 송수신할 메시지의 크기에 따라 적절하게 조절하면 된다.
typedef struct mymsgbuf {
    long mtype;
    char mtext[80];
}MyMsgBuf;
 
int main(void) {
    key_t key;
    int msgid;
    MyMsgBuf mesg;
 
    key = ftok("keyfile", 1);               // 키를 정의하고
    msgid = msgget(key, IPC_CREAT|0644);    // 메시지 식별자를 생성한다.
    if(msgid == -1) {
        perror("msgget");
        exit(1);
    }
 
    // 전송할 메시지 버퍼를 설정한다.
    mesg.mtype = 1; // 메시지 유형을 1로 정의
    strcpy(mesg.mtext, "Message Q Test\n"); // 메시지 버퍼의 배열에 문자열을 복사.
 
    if(msgsnd(msgid, (void *)&mesg, 80, IPC_NOWAIT) == -1) { // msgsnd 함수를 사용해 메시지를 전송한다.
        perror("msgsnd");
        exit(1);
    }
    return 0;
}

