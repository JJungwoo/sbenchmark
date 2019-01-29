#include <sys/types.h> 
#include <sys/socket.h> 

int main() 
{ 
   int sv[2]; 
   int pid; 
   int mynum = 1; 

   // ���Ͻ��� �����Ѵ�. 
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

   // exec�� �̿��ؼ� �ڽ����μ����� �����Ѵ�. 
   if( pid == 0) 
   { 
       // ���Ͻ��� �ϳ��� ǥ���Է����� �����Ѵ�. 
       dup2(sv[0], 0); 
       // ������ �ʴ� ������ �ݴ´�. 
       close(sv[1]); 
       close(sv[0]); 
	   execl("/home/jungwoo/jjo/LSP_project/pipe_cl_socket", "pipe_cl_socket", 0); 
   } 
   else if (pid > 0) 
   { 
       // ���Ͻ��� ������� �ʴ� ������ �ݴ´�.  
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