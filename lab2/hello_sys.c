#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

void hello() {
	syscall(SYS_write, 1, "Hello world!\n", 13);
}

int main() {
	hello();
	printf("%d\n", SYS_write);
}
