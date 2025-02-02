/*
 *
 * Copyright (C) 2024 KOO Inc.
 *
 * Driver for the Koomon
 *
 * Licensed under the GPL-2 or later.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include <stdlib.h>
#include <sys/statvfs.h>

int main(int argc, char** argv)
{

	struct statvfs stat;
	const char *path = "/mnt/sd/";

	if (statvfs(path, &stat) != 0) {
		perror("statvfs Error");
		return EXIT_FAILURE;
	}

	unsigned long long total = (unsigned long long)stat.f_blocks * stat.f_frsize;
	unsigned long long free = (unsigned long long)stat.f_bfree * stat.f_frsize;
	unsigned long long available = (unsigned long long)stat.f_bavail * stat.f_frsize;

	printf("경로: %s\n", path);
	printf("총 디스크 크기: %llu 바이트 (%.2f GB)\n", total, total / (1024.0 * 1024 * 1024));
	printf("사용 가능한 공간: %llu 바이트 (%.2f GB)\n", available, available / (1024.0 * 1024 * 1024));
	printf("남은 공간: %llu 바이트 (%.2f GB)\n", free, free / (1024.0 * 1024 * 1024));	

	printf("Koo\n");

	return 0;
}
