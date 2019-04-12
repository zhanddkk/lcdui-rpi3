#include <stdio.h>
#include <unistd.h>
#include "hal/gpio/gpio.h"
#include "hal/lcd/lcd.h"
static int system_init()
{
	int retVal = -1;
	Lcd lcd(1, 0x3f, 24, 25);
	if (lcd.init()) {
		lcd.sync();
		retVal = 0;
	}
	return retVal;
}

int main(int argc, const char *argv[])
{
	int ret = system_init();
	if (ret == 0) {
		while (true) {
			// do some tasks
			char ch = getchar();
			if (ch == 'q') {
				break;
			}
		}
	} else {
		printf("system init failed\n");
	}

	return ret;
}
