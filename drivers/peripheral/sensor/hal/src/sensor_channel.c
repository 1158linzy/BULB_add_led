/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "securec.h"
#include "hdf_io_service_if.h"
#include "osal_mutex.h"
#include "osal_time.h"
#include "sensor_channel.h"
#include "sensor_common_type.h"
#include "sensor_manager.h"
#include "sensor_type.h"

#define HDF_LOG_TAG    sensor_channel_c

#define ACCEL_ACCURACY    (HDI_SENSOR_GRAVITY / HDI_SENSOR_ACCEL_LSB / HDI_SENSOR_UNITS)

static struct SensorCovertCoff g_sensorCovertCoff[] = {
    { SENSOR_TYPE_NONE, 0, DATA_X, { ACCEL_ACCURACY } },
    { SENSOR_TYPE_ACCELEROMETER, SENSOR_TYPE_MAX, DATA_XYZ, { ACCEL_ACCURACY, ACCEL_ACCURACY, ACCEL_ACCURACY } },
};

void SetSensorIdBySensorType(enum SensorTypeTag type, int32_t sensorId)
{
    uint32_t count = sizeof(g_sensorCovertCoff) / sizeof(g_sensorCovertCoff[0]);

    for (uint32_t i = 0; i < count; ++i) {
        if (g_sensorCovertCoff[i].sensorTypeId == (int32_t)type) {
            g_sensorCovertCoff[i].sensorId = sensorId;
            break;
        }
    }
}

static void ConvertSensorData(struct SensorEvents *event)
{
    uint32_t dataLen;
    int32_t axis;
    int32_t *data = NULL;
    float *value = NULL;

    data = (int32_t *)event->data;
    value = (float *)event->data;

    for (uint32_t i = 0; i < sizeof(g_sensorCovertCoff) / sizeof(g_sensorCovertCoff[0]); ++i) {
        if ((g_sensorCovertCoff[i].sensorId == event->sensorId) && (g_sensorCovertCoff[i].dim != 0)) {
            dataLen = event->dataLen / sizeof(int32_t);
            for (uint32_t j = 0; j < dataLen; ++j) {
                axis = j % g_sensorCovertCoff[i].dim;
                value[j] = (float)(data[j] * g_sensorCovertCoff[i].coff[axis]);
            }
        }
    }
}

static int OnSensorEventReceived(struct HdfDevEventlistener *listener,
    struct HdfIoService *service, uint32_t id, struct HdfSBuf *data)
{
    uint32_t len;
    struct SensorEvents *event = NULL;
    struct SensorDevManager *manager = GetSensorDevManager();
    (void)listener;
    (void)service;
    (void)id;

    CHECK_NULL_PTR_RETURN_VALUE(manager->recordDataCb, SENSOR_NULL_PTR);

    (void)OsalMutexLock(&manager->eventMutex);
    if (!HdfSbufReadBuffer(data, (const void **)&event, &len) || event == NULL) {
        HDF_LOGE("%s: Read sensor event fail!", __func__);
        (void)OsalMutexUnlock(&manager->eventMutex);
        return SENSOR_FAILURE;
    }

    uint8_t *buf = NULL;
    if (!HdfSbufReadBuffer(data, (const void **)&buf, &len) || buf == NULL) {
        (void)OsalMutexUnlock(&manager->eventMutex);
        HDF_LOGE("%s: Read sensor data fail!", __func__);
        return SENSOR_FAILURE;
    } else {
        event->data = buf;
        event->dataLen = len;
    }

    ConvertSensorData(event);
    manager->recordDataCb(event);
    (void)OsalMutexUnlock(&manager->eventMutex);

    return SENSOR_SUCCESS;
}

static struct HdfDevEventlistener g_listener = {
    .onReceive = OnSensorEventReceived,
    .priv = "hdi_sensor"
};

struct HdfDevEventlistener *GetSensorListener()
{
    return &g_listener;
}

static int32_t AddSensorDevServiceGroup(void)
{
    struct SensorManagerNode *pos = NULL;
    struct SensorDevManager *manager = GetSensorDevManager();

    (void)OsalMutexLock(&manager->mutex);
    manager->serviceGroup = HdfIoServiceGroupObtain();
    if (manager->serviceGroup == NULL) {
        (void)OsalMutexUnlock(&manager->mutex);
        return SENSOR_FAILURE;
    }

    DLIST_FOR_EACH_ENTRY(pos, &manager->managerHead, struct SensorManagerNode, node) {
        if ((pos->ioService != NULL) &&
            (HdfIoServiceGroupAddService(manager->serviceGroup, pos->ioService) != SENSOR_SUCCESS)) {
            HdfIoServiceGroupRecycle(manager->serviceGroup);
            (void)OsalMutexUnlock(&manager->mutex);
            HDF_LOGE("%s: Add service to group failed", __func__);
            return SENSOR_INVALID_SERVICE;
        }
    }
    (void)OsalMutexUnlock(&manager->mutex);

    int32_t ret = HdfIoServiceGroupRegisterListener(manager->serviceGroup, &g_listener);
    if (ret != SENSOR_SUCCESS) {
        HDF_LOGE("%s: Register listener to group failed", __func__);
        HdfIoServiceGroupRecycle(manager->serviceGroup);
        return ret;
    }

    return SENSOR_SUCCESS;
}

int32_t Register(RecordDataCallback cb)
{
    struct SensorDevManager *manager = NULL;

    CHECK_NULL_PTR_RETURN_VALUE(cb, SENSOR_NULL_PTR);
    manager = GetSensorDevManager();
    (void)OsalMutexLock(&manager->mutex);
    manager->recordDataCb = cb;
    (void)OsalMutexUnlock(&manager->mutex);

    return AddSensorDevServiceGroup();
}

int32_t Unregister(void)
{
    struct SensorDevManager *manager = GetSensorDevManager();
    CHECK_NULL_PTR_RETURN_VALUE(manager->serviceGroup, SENSOR_SUCCESS);

    int32_t ret = HdfIoServiceGroupUnregisterListener(manager->serviceGroup, &g_listener);
    if (ret != SENSOR_SUCCESS) {
        HDF_LOGE("%s: Sensor unregister listener failed", __func__);
        return ret;
    }

    (void)OsalMutexLock(&manager->mutex);
    manager->hasSensorListener = false;
    HdfIoServiceGroupRecycle(manager->serviceGroup);
    manager->serviceGroup = NULL;
        manager->recordDataCb = NULL;

    (void)OsalMutexUnlock(&manager->mutex);

    return SENSOR_SUCCESS;
}
