/*
 *
 * Adelphos, log utilities
 *
 * Copyright (c) 2025 Pasqualino Ferrentino (lino.ferrentino@gmail.com)
 *
 * License: GPL3 or above.
 *
 *
 */

#ifndef a_log_h
#define a_log_h

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

#include "dbg.h"
#include "colors.h"

#ifndef MODULE_LEV
#define MODULE_LEV 2
#endif

#define ML_TRIVIAL 0
#define ML_INFO    1
#define ML_WARNING 2

#define TRIVIAL_MARK "-"
#define INFO_MARK " "
#define WARN_MARK "!"
#define LIMIT_MARK "*"
#define _MODULE_LEN_FIX 15

#define AM_PRINT_LOG fprintf
#define ALOG_NO_LOG_CODE do {} while(0)

#define LOG_MODULE_IMP \
  static uint8_t _imp_module_initialized = 0; \
  static char _imp_MODULE[_MODULE_LEN_FIX + 1];

const char *ad_get_module(const char* _buf, 
		uint8_t *_imp_module_initialized, char *_imp_MODULE_);


#include <time.h>
#include <sys/time.h>
#define PRINT_ALOG(mark, fmt, ...) do {\
	struct timeval tv; gettimeofday(&tv, NULL); \
        AM_PRINT_LOG(stderr, "%.4d.%d [%s:%.4d]" mark fmt "\n", (int)(tv.tv_sec % 10000), (int)(tv.tv_usec), \
			ad_get_module(_LOG_MODULE, &_imp_module_initialized, _imp_MODULE), __LINE__, ##__VA_ARGS__);\
        }while(0)

#if MODULE_LEV == 0

 #define alogt(fmt, ...) PRINT_ALOG(TRIVIAL_MARK, fmt , ##__VA_ARGS__)
 #define dump_payload_t(_dmp_buf, _dmp_buf_sz)  \
        DUMP_PAYLOAD(_dmp_buf, _dmp_buf_sz)

 #define alog_tlim(ms, now, msg, ...) do { \
        static uint64_t _lst = 0; \
        int64_t df = now - _lst; \
        static int skipped = 0;\
        if ((df) > (ms*1000)) {\
                PRINT_ALOG(LIMIT_MARK, "/%jd-%d/ " msg, df, \
                                skipped, ##__VA_ARGS__); \
                _lst = now; skipped = 0; \
        } else ++skipped; }  while(0)


#else
 #define alogt(fmt, ...) ALOG_NO_LOG_CODE
 #define dump_payload_t(_dmp_buf, _dmp_buf_sz)  ULOG_NO_DUMP_CODE
 #define alog_tlim(ms, now, msg, ...) ALOG_NO_LOG_CODE
#endif


#if MODULE_LEV <= 1
 #define alogi(fmt, ...) PRINT_ALOG(INFO_MARK, fmt , ##__VA_ARGS__)
#else
 #define alogi(fmt, ...) ALOG_NO_LOG_CODE
#endif

#if MODULE_LEV <= 1
 #define alogw(fmt, ...) PRINT_ALOG(WARN_MARK, fmt , ##__VA_ARGS__)
#else
 #define alogw(fmt, ...) ALOG_NO_LOG_CODE
#endif

#define alogx(fmt, ...) ALOG_NO_LOG_CODE

#define adie(X, ...) do { fprintf(stderr, "%d Error: " X "\n", __LINE__, ##__VA_ARGS__); exit(-1); } while(0)


#endif

