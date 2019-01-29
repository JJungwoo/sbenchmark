#include <unistd.h>

int main()
{
	int buf;
	while(1)
	{
		read(0, (void *)&buf, sizeof(buf));
		buf++;
		write(0,(void *)&buf, sizeof(buf));
	}
}
