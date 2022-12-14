/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#include "securec.h"
#include "accel_bmi160.h"
#include "osal_time.h"
#include "sensor_accel_driver.h"
#include "sensor_device_common.h"
#include "sensor_device_manager.h"

/* IO config for int-pin and I2C-pin */
#define SENSOR_I2C6_DATA_REG_ADDR 0x114f004c
#define SENSOR_I2C6_CLK_REG_ADDR  0x114f0048
#define SENSOR_I2C_REG_CFG        0x403

static int32_t ReadBmi160RawData(struct SensorCfgData *data, struct AccelData *rawData, int64_t *timestamp)
{
    uint8_t status = 0;
    uint8_t reg[ACCEL_AXIS_BUTT];
    OsalTimespec time;

    (void)memset_s(&time, sizeof(time), 0, sizeof(time));
    (void)memset_s(&reg, sizeof(reg), 0, sizeof(reg));


    CHECK_NULL_PTR_RETURN_VALUE(data, HDF_ERR_INVALID_PARAM);
    if (OsalGetTime(&time) != HDF_SUCCESS) {
        HDF_LOGE("%s: Get accel system time failed", __func__);
        return HDF_FAILURE;
    }
    *timestamp = time.sec * SENSOR_SECOND_CONVERT_NANOSECOND + time.usec * SENSOR_CONVERT_UNIT; /* unit nanosecond */

    int32_t ret = ReadSensor(&data->busCfg, BMI160_STATUS_ADDR, &status, sizeof(uint8_t));
    if (!(status & BMI160_ACCEL_DATA_READY_MASK) || (ret != HDF_SUCCESS)) {
        return HDF_FAILURE;
    }

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_X_LSB_ADDR, &reg[ACCEL_X_AXIS_LSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_X_MSB_ADDR, &reg[ACCEL_X_AXIS_MSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_Y_LSB_ADDR, &reg[ACCEL_Y_AXIS_LSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_Y_MSB_ADDR, &reg[ACCEL_Y_AXIS_MSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_Z_LSB_ADDR, &reg[ACCEL_Z_AXIS_LSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    ret = ReadSensor(&data->busCfg, BMI160_ACCEL_Z_MSB_ADDR, &reg[ACCEL_Z_AXIS_MSB], sizeof(uint8_t));
    CHECK_PARSER_RESULT_RETURN_VALUE(ret, "read data");

    rawData->x = (int16_t)(SENSOR_DATA_SHIFT_LEFT(reg[ACCEL_X_AXIS_MSB], SENSOR_DATA_WIDTH_8_BIT) |
        reg[ACCEL_X_AXIS_LSB]);
    rawData->y = (int16_t)(SENSOR_DATA_SHIFT_LEFT(reg[ACCEL_Y_AXIS_MSB], SENSOR_DATA_WIDTH_8_BIT) |
        reg[ACCEL_Y_AXIS_LSB]);
    rawData->z = (int16_t)(SENSOR_DATA_SHIFT_LEFT(reg[ACCEL_Z_AXIS_MSB], SENSOR_DATA_WIDTH_8_BIT) |
        reg[ACCEL_Z_AXIS_LSB]);

    return ret;
}

int32_t ReadBmi160Data(struct SensorCfgData *data)
{
    int32_t ret;
    struct AccelData rawData = { 0, 0, 0 };
    int32_t tmp[ACCEL_AXIS_NUM];
    struct SensorReportEvent event;

    (void)memset_s(&event, sizeof(event), 0, sizeof(event));

    ret = ReadBmi160RawData(data, &rawData, &event.timestamp);
    if (ret != HDF_SUCCESS) {
        return HDF_FAILURE;
    }

    event.sensorId = SENSOR_TAG_ACCELEROMETER;
    event.option = 0;
    event.mode = SENSOR_WORK_MODE_REALTIME;

    rawData.x = rawData.x * BMI160_ACC_SENSITIVITY_2G;
    rawData.y = rawData.y * BMI160_ACC_SENSITIVITY_2G;
    rawData.z = rawData.z * BMI160_ACC_SENSITIVITY_2G;

    tmp[ACCEL_X_AXIS] = (rawData.x * SENSOR_1K_UNIT) / SENSOR_CONVERT_UNIT;
    tmp[ACCEL_Y_AXIS] = (rawData.y * SENSOR_1K_UNIT) / SENSOR_CONVERT_UNIT;
    tmp[ACCEL_Z_AXIS] = (rawData.z * SENSOR_1K_UNIT) / SENSOR_CONVERT_UNIT;

    event.dataLen = sizeof(tmp);
    event.data = (uint8_t *)&tmp;
    ret = ReportSensorEvent(&event);
    return ret;
}

static int32_t InitBmi160(struct SensorCfgData *data)
{
    int32_t ret;

    CHECK_NULL_PTR_RETURN_VALUE(data, HDF_ERR_INVALID_PARAM);
    ret = SetSensorRegCfgArray(&data->busCfg, data->regCfgGroup[SENSOR_INIT_GROUP]);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: BMI160 sensor init config failed", __func__);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t InitAccelPreConfig(void)
{
    if (SetSensorPinMux(SENSOR_I2C6_DATA_REG_ADDR, SENSOR_ADDR_WIDTH_4_BYTE, SENSOR_I2C_REG_CFG) != HDF_SUCCESS) {
        HDF_LOGE("%s: Data write mux pin failed", __func__);
        return HDF_FAILURE;
    }
    if (SetSensorPinMux(SENSOR_I2C6_CLK_REG_ADDR, SENSOR_ADDR_WIDTH_4_BYTE, SENSOR_I2C_REG_CFG) != HDF_SUCCESS) {
        HDF_LOGE("%s: ClK write mux pin failed", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

int32_t DetectAccelBim160Chip(struct SensorCfgData *data)
{
    int32_t ret;
    struct AccelOpsCall ops;
    CHECK_NULL_PTR_RETURN_VALUE(data, HDF_ERR_INVALID_PARAM);

    if (strcmp(ACCEL_CHIP_NAME_BMI160, data->sensorAttr.chipName) != 0) {
        return HDF_SUCCESS;
    }
    ret = InitAccelPreConfig();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: init  BMI160 bus mux config", __func__);
        return HDF_FAILURE;
    }
    if (DetectSensorDevice(data) != HDF_SUCCESS) {
        return HDF_FAILURE;
    }
    ops.Init = InitBmi160;
    ops.ReadData = ReadBmi160Data;
    ret = RegisterAccelChipOps(&ops);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: register BMI160 accel failed", __func__);
        (void)ReleaseSensorBusHandle(&data->busCfg);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}
