#include <stdio.h>
#include <stdlib.h>

int main()
{
	enum week {Sun, Mon, Tue, Wed, Thur, Fri, Sat}; 
	enum week dow = Mon; 
	
	printf("%d\n", dow);


	enum status {OK = 1, Warning, Error, Fatal}; 
	enum status s = Error;
	
	printf("%d\n", s);
	return 0;
}
