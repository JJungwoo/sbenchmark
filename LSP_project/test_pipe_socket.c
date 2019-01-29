#include <sys/types.h> 
#include <sys/socket.h> 

int main() 
{ 
   int sv[2]; 
   int pid; 
   int mynum = 1; 

   // 소켓쌍을 생성한다. 
   if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) 
   { 
       perror("socketpair error"); 
       exit(0); 
   } 

   pid = fork(); 
   if (pid < 0) 
   { 
       perror("fork error"); 
       exit(0); 
   } 

   // exec를 이용해서 자식프로세스를 생성한다. 
   if( pid == 0) 
   { 
       // 소켓쌍중 하나를 표준입력으로 복사한다. 
       dup2(sv[0], 0); 
       // 사용되지 않는 소켓을 닫는다. 
       close(sv[1]); 
       close(sv[0]); 
	   execl("/home/jungwoo/jjo/LSP_project/pipe_cl_socket", "pipe_cl_socket", 0); 
   } 
   else if (pid > 0) 
   { 
       // 쏘켓쌍중 사용하지 않는 소켓은 닫는다.  
       close(sv[0]); 
       while(1) 
       { 
           write(sv[1], (void *)&mynum, sizeof(mynum)); 
           sleep(1); 
           read(sv[1], (void *)&mynum, sizeof(mynum)); 
           printf("num is %d\n", mynum); 
       } 
   } 
} 