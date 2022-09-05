#include "hilink_device.h"
#include "hilink_log.h"
#include <stdlib.h>

#include <unistd.h>

// #include "ohos_init.h"
#include "hi_cmsis_os2.h"
#include <hi_io.h>
#include "hi_gpio.h"
#include "hsi_gpio.h"

#include <app_demo_io_gpio.h>

#define M2M_NO_ERROR 0
#define M2M_SVC_RPT_CREATE_PAYLOAD_ERR -201

#ifndef INVALID_PACKET
#define INVALID_PACKET -87
#endif
//LED配置
#define LED_INTERVAL_TIME_US 300000
#define LED_TASK_STACK_SIZE 1024
#define LED_TASK_PRIO 25
//#define LED_TEST_GPIO 12 
#define LED_TEST_GPIO 2 
#define KEY_TEST_GPIO 7


typedef struct {
	unsigned int switch_on;
	unsigned int faltDetection_code;
	unsigned int faltDetection_status;
} t_device_info;

enum LedState {
    LED_ON = 0,
    LED_OFF,
    LED_SPARK,
};
enum LedSpeed{
	FAST = 0,
	MIDDLE,
	SLOW,
};

static t_device_info g_device_info = { 0 };

enum LedState g_ledState = LED_SPARK;
enum LedSpeed g_ledspeed = FAST;
hi_gpio_value gpio_val = HI_GPIO_VALUE1;

static void *Led_and_Key_Task(const char *arg)
{
    (void)arg;
    while (1) {
		//判断从DP 平台下发的命令解析后数据
		if(g_device_info.switch_on == 1)
		{//给高电平，灯亮
			    IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
                usleep(LED_INTERVAL_TIME_US);
		}
		if(g_device_info.switch_on == 0)
		{/*给低电平，灯灭*/
			    IoTGpioSetOutputVal(LED_TEST_GPIO, 0);
                usleep(LED_INTERVAL_TIME_US);
		}
		//轮询获取按键输入的电平，先读取一次
		hi_gpio_get_input_val(HI_GPIO_IDX_7, &gpio_val);
		//判断读取到的电平
		if(gpio_val == HI_GPIO_VALUE0)
		{
			hi_sleep(100);
			while (gpio_val==HI_GPIO_VALUE0)
			{//松手检测
				hi_gpio_get_input_val(HI_GPIO_IDX_7, &gpio_val);/* code */
			}
			printf("Key was pressed!\r\n");
			IoTGpioSetOutputVal(LED_TEST_GPIO, 1);
    		usleep(LED_INTERVAL_TIME_US);
		}
    }
    return NULL;
}

hi_void io_gpio_demo(hi_void)
{
    /* Take gpio 0 as an example */
    hi_u32 ret;
	IoTGpioInit(KEY_TEST_GPIO);
     //ret = hi_gpio_init();
    // if (ret != HI_ERR_SUCCESS) {
    //     printf("===== ERROR ===== gpio -> hi_gpio_init ret:%d\r\n", ret);
    //     return;
    // }
    // printf("----- gpio init success-----\r\n");

	//复用管脚功能
    ret = hi_io_set_func(HI_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_GPIO);
    if (ret != HI_ERR_SUCCESS) {
        printf("===== ERROR ===== gpio -> hi_io_set_func ret:%d\r\n", ret);
        return;
    }
    printf("----- io set func success-----\r\n");
	//设置上拉电阻
	hi_io_set_pull(HI_IO_NAME_GPIO_7,HI_IO_PULL_UP);
	hi_io_set_driver_strength(HI_IO_NAME_GPIO_7,HI_IO_DRIVER_STRENGTH_0);
	//设置管脚模式
    ret = hi_gpio_set_dir(HI_GPIO_IDX_7, HI_GPIO_DIR_IN);
    if (ret != HI_ERR_SUCCESS) {
        printf("===== ERROR ===== gpio -> hi_gpio_set_dir1 ret:%d\r\n", ret);
        return;
    }
    printf("----- gpio set dir success! -----\r\n");
}
void LedExampleEntry(void)
{
    osThreadAttr_t attr;
	//LED
    IoTGpioInit(LED_TEST_GPIO);
    IoTGpioSetDir(LED_TEST_GPIO, IOT_GPIO_DIR_OUT);
	//key
	io_gpio_demo();

    attr.name = "Led_and_Key_Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = LED_TASK_STACK_SIZE;
    attr.priority = LED_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)Led_and_Key_Task, NULL, &attr) == NULL) {
        printf("[LedExample] Falied to create Led_and_Key_Task!\n");
    }
}

SYS_RUN(LedExampleEntry);

int handle_put_switch(const char *svc_id, const char *payload, unsigned int len) {

	void *pJson = hilink_json_parse(payload);
	if (pJson == NULL) {
		printf("JSON parse failed in PUT cmd: ID-%s \r\n", svc_id);
		return INVALID_PACKET;
	}
	bool on;
	bool *on_p = NULL;
	if (hilink_json_get_number_value(pJson, "on", &on) == 0
			&& (on == 0 || on == 1)) {
		on_p = &on;
	}
	//全局变量，获取解析从DP平台下发的命令
	g_device_info.switch_on = *on_p;
	if (pJson != NULL) {
		hilink_json_delete(pJson);
	}
	printf("handle func:%s, sid:%s \r\n", __FUNCTION__, svc_id);

	return M2M_NO_ERROR;
}
int handle_get_switch(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	bool on = g_device_info.switch_on;
	*out_len = 20;
	*out = (char*) hilink_malloc(*out_len);
	if (NULL == *out) {
		printf("malloc failed in GET cmd: ser %s in GET cmd", svc_id);
		return INVALID_PACKET;
	}
	*out_len = sprintf(*out, "{\"on\":%d}", on);
	printf("hilink_device_ctr.c :%d %s svcId:%s, out:%s\r\n", __LINE__,
			__FUNCTION__, svc_id, *out);

	return M2M_NO_ERROR;
}

typedef int (*handle_put_func)(const char *svc_id, const char *payload, unsigned int len);
typedef int (*handle_get_func)(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len);

typedef struct {
	char *sid;
	handle_put_func putFunc;
	handle_get_func getFunc;
} HANDLE_SVC_INFO;


HANDLE_SVC_INFO g_device_profile[] =
{
	{ "switch", handle_put_switch, handle_get_switch },
};

int g_device_profile_count = sizeof(g_device_profile) / sizeof(HANDLE_SVC_INFO);

static HANDLE_SVC_INFO* find_handle(const char *svc_id) {

	for (int i = 0; i < g_device_profile_count; i++)
	{
		HANDLE_SVC_INFO handle = g_device_profile[i];

		if (strcmp(handle.sid, svc_id) == 0)
		{
			return &g_device_profile[i];
		}
	}
	return NULL;
}
int handle_put_cmd(const char *svc_id, const char *payload, unsigned int len) {

	HANDLE_SVC_INFO *handle = find_handle(svc_id);
	if (handle == NULL) {
		printf("no service to handle put cmd : %s \r\n", svc_id);
		return INVALID_PACKET;
	}
	handle_put_func function = handle->putFunc;
	if (function == NULL) {
		printf("put function is null for %s \r\n", svc_id);
		return INVALID_PACKET;
	}

	return function(svc_id, payload, len);
}
int handle_get_cmd(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	HANDLE_SVC_INFO *handle = find_handle(svc_id);
	if (handle == NULL) {
		printf("no service to handle get cmd : %s \r\n", svc_id);
		return INVALID_PACKET;
	}
	handle_get_func function = handle->getFunc;
	if (function == NULL) {
		printf("get function is null for %s \r\n", svc_id);
		return INVALID_PACKET;
	}

	return function(svc_id, in, in_len, out, out_len);
}
int hilink_put_char_state(const char *svc_id, const char *payload, unsigned int len) {

	int err = M2M_NO_ERROR;
	if (svc_id == NULL) {
		hilink_error("empty service ID in PUT cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	if (payload == NULL) {
		hilink_error("empty payload in PUT cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	

	hilink_debug("start handle PUT cmd: ID-%s", svc_id);
	err = handle_put_cmd(svc_id, payload, len);
	hilink_debug("handle PUT cmd end: ID-%s, ret-%d", svc_id, err);

	return err;
}
int hilink_get_char_state(const char *svc_id, const char *in, unsigned int in_len, char **out, unsigned int *out_len) {

	int err = M2M_NO_ERROR;
	if (svc_id == NULL) {
		hilink_error("empty service ID in GET cmd");
		return M2M_SVC_RPT_CREATE_PAYLOAD_ERR;
	}
	hilink_info("start process GET cmd: ID - %s", svc_id);
	err = handle_get_cmd(svc_id, in, in_len, out, out_len);
	hilink_debug("end process GET cmd: ID - %s, ret - %d", svc_id, err);

	return err;
}


