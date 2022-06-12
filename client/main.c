#include <err.h>
#include "print_page.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
		err(1, "2 arguments must be passed as parameter");

	print_page(argv[1], argv[2]);

	return 0;
}
