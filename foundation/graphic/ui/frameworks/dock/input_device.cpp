/*
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
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

#include "dock/input_device.h"

namespace OHOS {
void InputDevice::ProcessEvent()
{
    DeviceData data;
    bool moreToRead = false;
    do {
        moreToRead = Read(data);
        DispatchEvent(data);
    } while (moreToRead);
}
}  // namespace OHOS
