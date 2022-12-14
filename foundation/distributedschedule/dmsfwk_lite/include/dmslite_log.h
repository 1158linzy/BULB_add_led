/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_DISTRIBUTEDSCHEDULE_DMSLITELOG_H
#define OHOS_DISTRIBUTEDSCHEDULE_DMSLITELOG_H

#include "log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DMSLITE_LABEL "[DMSLITE]"
#define NEW_LINE "\n"

#ifdef APP_PLATFORM_WATCHGT
#define DMSLITE_LOG(LEVEL, fmt, ...)
#else
#define DMSLITE_LOG(LEVEL, fmt, ...) \
    HILOG_##LEVEL(HILOG_MODULE_DMS, DMSLITE_LABEL "[%s:%d]" fmt NEW_LINE, __func__, __LINE__, ##__VA_ARGS__)
#endif

#if HILOG_COMPILE_LEVEL <= HILOG_LV_DEBUG
#define HILOGD(fmt, ...) DMSLITE_LOG(DEBUG, fmt, ##__VA_ARGS__)
#else
#define HILOGD(fmt, ...)
#endif

#if HILOG_COMPILE_LEVEL <= HILOG_LV_INFO
#define HILOGI(fmt, ...) DMSLITE_LOG(INFO, fmt, ##__VA_ARGS__)
#else
#define HILOGI(fmt, ...)
#endif

#if HILOG_COMPILE_LEVEL <= HILOG_LV_WARN
#define HILOGW(fmt, ...) DMSLITE_LOG(WARN, fmt, ##__VA_ARGS__)
#else
#define HILOGW(fmt, ...)
#endif

#if HILOG_COMPILE_LEVEL <= HILOG_LV_ERROR
#define HILOGE(fmt, ...) DMSLITE_LOG(ERROR, fmt, ##__VA_ARGS__)
#else
#define HILOGE(fmt, ...)
#endif

#if HILOG_COMPILE_LEVEL <= HILOG_LV_FATAL
#define HILOGF(fmt, ...) DMSLITE_LOG(FATAL, fmt, ##__VA_ARGS__)
#else
#define HILOGF(fmt, ...)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // OHOS_DISTRIBUTEDSCHEDULE_DMSLITELOG_H