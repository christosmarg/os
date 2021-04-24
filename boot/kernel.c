#include "extern.h"

void
kernel_main(void) 
{
	tty_init();
	tty_write("Nothing to see here yet. At least it booted.\n");
}
