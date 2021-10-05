#ifndef __SYLAR_UTIL_H_
#define __SYLAR_UTIL_H_

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <string>

namespace sylar
{
	pid_t GetThreadId();
	uint32_t GetFiberId();
    void Backtrace(std::vector<std::string>& bt, int size, int skip=1);
    std::string BacktraceToString(int size, int skip=2, const std::string& prefix = "");
}



#endif