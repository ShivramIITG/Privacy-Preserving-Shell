#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <linux/types.h>
#include <stdlib.h>
#include <string.h>

char* substr(const char *src, int m, int len)
{
	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	strncpy(dest, (src + m), len);

	return dest;
}

int makesyscall(char *list_info, int *list_sizes)
{
	int err;	
	__asm(
		"MOV x8, #553;" //x8 holds syscall no
		"SVC #0;"      // supervisor call
		"MOV %[result], x0" : [result] "=r"(err) // copy return code to err variable
	);
	return err; 
}

int main()
{
	char *list_vals;
	list_vals = (char *) malloc(100);

	int num_services = 2, cur = 0;

	int *list_sizes;
	list_sizes = (int *) malloc(num_services * 2 * sizeof(int));

	int ret = makesyscall(list_vals, list_sizes);

	printf("ppslist syscall: %d\n", ret);

	printf("List of Services Available:\n");
	for (int i = 0; i < (num_services*2); i+=2)
	{
		char *name = substr(list_vals, cur, list_sizes[i]);
		cur += list_sizes[i];
		char *desc = substr(list_vals, cur, list_sizes[i+1]);
		cur += list_sizes[i+1];
		printf("%s\t- %s\n", name, desc);
	}

	return 0;
}

