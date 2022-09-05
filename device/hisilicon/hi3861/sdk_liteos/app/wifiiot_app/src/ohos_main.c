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

#include "ohos_main.h"
#include <hi_at.h>
#include <hi_errno.h>

#if defined(CONFIG_AT_COMMAND) || defined(CONFIG_FACTORY_TEST_MODE)
int __attribute__((weak)) QuerySysparaCmd()
{
    return;
}
#ifdef CONFIG_HILINK
hi_u32 __attribute__((weak)) cmd_set_systoken(hi_s32 argc, const hi_char *argv[]){
	return;
}
hi_u32 __attribute__((weak)) cmd_get_systoken(hi_s32 argc, const hi_char *argv[]){
	return;
}
hi_u32 __attribute__((weak)) at_exe_rfactory_cmd(void){
	return;
}
#endif

static const  at_cmd_func G_OHOS_AT_FUNC_TBL[] = {
    {"+SYSPARA", 8, 0, 0, 0, (at_call_back_func)QuerySysparaCmd},
#ifdef CONFIG_HILINK
	{"+SYSTOKEN",9, 0, (at_call_back_func)cmd_get_systoken, (at_call_back_func)cmd_set_systoken, 0},
	{"+SYSFACTORY", 11, 0 ,0, 0, (at_call_back_func)at_exe_rfactory_cmd},
#endif
};
#define OHOS_AT_FUNC_NUM  (sizeof(G_OHOS_AT_FUNC_TBL) / sizeof(G_OHOS_AT_FUNC_TBL[0]))
#endif

void __attribute__((weak)) OHOS_SystemInit(void)
{
    return;
}
extern void LedExampleEntry(void);
void OHOS_Main()
{
#if defined(CONFIG_AT_COMMAND) || defined(CONFIG_FACTORY_TEST_MODE)
    hi_u32 ret;
    ret = hi_at_init();
    if (ret == HI_ERR_SUCCESS) {
        hi_u32 ret2 = hi_at_register_cmd(G_OHOS_AT_FUNC_TBL, OHOS_AT_FUNC_NUM);
        if (ret2 != HI_ERR_SUCCESS) {
            printf("Register ohos failed!\n");
        }
    }
#endif
    LedExampleEntry();
    OHOS_SystemInit();
}
