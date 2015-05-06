#include "main.h"
#include "threads.h"
#include "threadInterface.h"

void threadInterface_func(cyg_addrword_t data) {

	cmd_ini(0, NULL);
	monitor();
}
