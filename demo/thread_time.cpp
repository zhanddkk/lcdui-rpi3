/*
 * thread_time.cpp
 *
 *  Created on: Apr 10, 2019
 *      Author: zhanlei
 */
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <sys/time.h>

static sem_t sem;
char ch = '\0';

static int change_time(void)
{
	struct timeval tv;
	int ret = gettimeofday(&tv, NULL);
	if (ret != 0) {
		printf("get system time failed");
		return ret;
	}

	tv.tv_sec -= 100;
	ret = settimeofday(&tv , NULL);
	if (ret == 0) {
		printf("set system time back to 100s");
	} else {
		printf("set system time failed");
		return ret;
	}

	return ret;
}

static void *thread_task(void *data)
{
    timespec rts, mts;
    int is_exit = false;
    int ticks = 0;

	while(!is_exit) {
		clock_gettime(CLOCK_MONOTONIC, &mts);
		printf("[%04d] %lds - %ldns : ", ticks++, mts.tv_sec, mts.tv_nsec);

		clock_gettime(CLOCK_REALTIME, &rts);
		rts.tv_sec += 5;
		int ret = sem_timedwait(&sem, &rts);

		clock_gettime(CLOCK_MONOTONIC, &mts);
		printf("%lds - %ldns ", mts.tv_sec, mts.tv_nsec);
		if(ret == 0) {
			switch (ch) {
			case 'q':
				printf("<exit>");
				is_exit = true;
				break;
			case 'w':
				printf("<change time: ");
				ret = change_time();
				printf(" ret = %d>", ret);
				break;
			default:
				printf("<message: %c>", ch);
				break;
			}
			ch = '\0';
		} else {
			printf("<timeout>");
		}
		printf("\n");
	}

	return nullptr;
}

int demo_thread_time_main(int argc, const char **argv)
{
	pthread_t thread;
	int ret = sem_init(&sem, 0, 0);
	if (ret != 0) {
		printf("init semaphore failed\n");
		return ret;
	}
	ret = pthread_create(&thread, nullptr, thread_task, nullptr);
	if (ret != 0) {
		printf("create thread failed\n");
		return ret;
	}

	while (true) {
		ch = getchar();
		if ((ch >= 0x20) && (ch <= 0x7e)) {
			sem_post(&sem);
			while (ch != '\0') {
				usleep(50000);
			}
		}
		if (ch == 'q') {
			break;
		}
	}
	pthread_cancel(thread);
	sem_destroy(&sem);
	return 0;
}



