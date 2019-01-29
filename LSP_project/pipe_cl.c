#include <unistd.h>

int main()
{
	int buf;
	while(1)
	{
		//read(0, (void *)&buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		printf("read %d\n", buf);
	}
}
