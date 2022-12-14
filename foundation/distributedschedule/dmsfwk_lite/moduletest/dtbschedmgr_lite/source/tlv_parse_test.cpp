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

#include "gtest/gtest.h"

#include "dmslite_parser.h"
#include "dmslite_tlv_common.h"

using namespace testing::ext;

namespace OHOS {
namespace DistributedSchedule {
class TlvParseTest : public testing::Test {
protected:
    static void SetUpTestCase() { }
    static void TearDownTestCase() { }
    virtual void SetUp() { }
    virtual void TearDown() { }

    static void RunTest(const uint8_t *buffer, uint16_t bufferLen,
        const TlvParseCallback onTlvParseDone, const StartAbilityCallback onStartAbilityDone)
    {
        IDmsFeatureCallback dmsFeatureCallback = {
            .onTlvParseDone = onTlvParseDone,
            .onStartAbilityDone = onStartAbilityDone
        };

        CommuMessage commuMessage;
        commuMessage.payloadLength = bufferLen;
        commuMessage.payload = buffer;

        ProcessCommuMsg(&commuMessage, &dmsFeatureCallback);
    }
};

/**
 * @tc.name: NormalPackage_001
 * @tc.desc: normal package with small bundle name and ability name
 * @tc.type: FUNC
 * @tc.require: SR000ELTHO
 */
HWTEST_F(TlvParseTest, NormalPackage_001, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x01,
        0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e, 0x68, 0x75, 0x61, 0x77, 0x65, 0x69,
        0x2e, 0x6c, 0x61, 0x75, 0x6e, 0x63, 0x68, 0x65, 0x72, 0x00,
        0x03, 0x0c, 0x4d, 0x61, 0x69, 0x6e, 0x41, 0x62, 0x69, 0x6c, 0x69, 0x74,
        0x79, 0x00,
        0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B, 0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        const TlvNode *tlvHead = reinterpret_cast<const TlvNode *>(dmsMsg);
        EXPECT_EQ(errCode, DMS_TLV_SUCCESS);
        EXPECT_EQ(UnMarshallUint16(tlvHead, DMS_TLV_TYPE_COMMAND_ID), 1);
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_BUNDLE_NAME)), "com.huawei.launcher");
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_ABILITY_NAME)), "MainAbility");
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLER_SIGNATURE)), "publickey");
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}


/**
 * @tc.name: NormalPackageWithLongBundleName
 * @tc.desc: normal package with 255 bytes long(upper boundary) bundle name
 * @tc.type: FUNC
 * @tc.require: AR000ENCTK
 */

HWTEST_F(TlvParseTest, NormalPackage_002, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x01,
        0x02, 0x82, 0x00, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x62, 0x63, 0x64, 0x65,
        0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
        0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x41, 0x42, 0x43,
        0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61,
        0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
        0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
        0x7a, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
        0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
        0x76, 0x77, 0x78, 0x79, 0x7a, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
        0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65,
        0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71,
        0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x41, 0x42, 0x43,
        0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61,
        0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
        0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
        0x7a, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x00,
        0x03, 0x0c, 0x4d, 0x61, 0x69, 0x6e, 0x41, 0x62, 0x69, 0x6c, 0x69, 0x74,
        0x79, 0x00,
        0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B, 0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        const TlvNode *tlvHead = reinterpret_cast<const TlvNode *>(dmsMsg);
        EXPECT_EQ(errCode, DMS_TLV_SUCCESS);
        std::stringstream ss;
        ss << "com.";
        for (int8_t i = 0; i < 4; i++) {
            ss << "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
        ss << "abcdefghijklmnopqrstuvwxyzABCDEFGHIJ";
        ss << ".huawei";
        EXPECT_EQ(UnMarshallUint16(tlvHead, DMS_TLV_TYPE_COMMAND_ID), 1);
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_BUNDLE_NAME)), ss.str());
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_ABILITY_NAME)), "MainAbility");
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLER_SIGNATURE)), "publickey");
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageOutOfOrder_001
 * @tc.desc: abnormal package with node type sequence in disorder
 * @tc.type: FUNC
 * @tc.require: AR000ELTIG
 */
HWTEST_F(TlvParseTest, AbnormalPackageOutOfOrder_001, TestSize.Level0) {
    uint8_t buffer[] = {
        0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e, 0x68, 0x75, 0x61, 0x77,
        0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75, 0x6e, 0x63, 0x68, 0x65,
        0x72, 0x00, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_OUT_OF_ORDER);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadNodeNum_001
 * @tc.desc: abnormal package without node
 * @tc.type: FUNC
 * @tc.require: AR000E0DGE
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadNodeNum_001, TestSize.Level0) {
    uint8_t buffer[] = { };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadNodeNum_002
 * @tc.desc: abnormal package with only one mandatory node
 * @tc.type: FUNC
 * @tc.require: AR000E0DE5
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadNodeNum_002, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_BAD_NODE_NUM);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}


/**
 * @tc.name: AbnormalPackageBadNodeNum_005
 * @tc.desc: abnormal package with an additional node
 * @tc.type: FUNC
 * @tc.require: AR000DSCRB
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadNodeNum_005, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00, 0x05, 0x01, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_BAD_NODE_NUM);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_001
 * @tc.desc: abnormal package tlv node without value
 * @tc.type: FUNC
 * @tc.require: SR000E0DR9
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_001, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_002
 * @tc.desc: abnormal package tlv node with zero-size length
 * @tc.type: FUNC
 * @tc.require: AR000E0ECR
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_002, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_003
 * @tc.desc: abnormal package with mismatched buffer size
 * @tc.type: FUNC
 * @tc.require: AR000E0DE0
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_003, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, sizeof(buffer) + 1, onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_004
 * @tc.desc: abnormal package with mismatched buffer size
 * @tc.type: FUNC
 * @tc.require: AR000E0DE0
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_004, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, sizeof(buffer) - 1, onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_005
 * @tc.desc: abnormal package with mismatched buffer size
 * @tc.type: FUNC
 * @tc.require: AR000E0DE0
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_005, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        bool condition = (errCode == DMS_TLV_ERR_LEN
                          || errCode == DMS_TLV_ERR_BAD_NODE_NUM);
        EXPECT_EQ(true, condition);
    };

    RunTest(buffer, MAX_DMS_MSG_LENGTH, onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadLength_006
 * @tc.desc: abnormal package with mismatched buffer size
 * @tc.type: FUNC
 * @tc.require: AR000E0DE0
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadLength_006, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0x00, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0x00, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0x00
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        EXPECT_EQ(errCode, DMS_TLV_ERR_LEN);
    };

    RunTest(buffer, 0, onTlvParseDone, nullptr);
}

/**
 * @tc.name: AbnormalPackageBadSource_001
 * @tc.desc: abnormal package string field with no '\0' in the ending
 * @tc.type: FUNC
 * @tc.require: AR000E0DE0
 */
HWTEST_F(TlvParseTest, AbnormalPackageBadSource_001, TestSize.Level0) {
    uint8_t buffer[] = {
        0x01, 0x02, 0x00, 0x00, 0x02, 0x14, 0x63, 0x6f, 0x6d, 0x2e,
        0x68, 0x75, 0x61, 0x77, 0x65, 0x69, 0x2e, 0x6c, 0x61, 0x75,
        0x6e, 0x63, 0x68, 0x65, 0x72, 0xff, 0x03, 0x0d, 0x4d, 0x61,
        0x69, 0x6e, 0x41, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79,
        0xff, 0x04, 0x0a, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0x6B,
        0x65, 0x79, 0xff
    };

    auto onTlvParseDone = [] (int8_t errCode, const void *dmsMsg) {
        const TlvNode *tlvHead = reinterpret_cast<const TlvNode *>(dmsMsg);
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_BUNDLE_NAME)), "");
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLEE_ABILITY_NAME)), "");
        EXPECT_EQ(std::string(UnMarshallString(tlvHead, DMS_TLV_TYPE_CALLER_SIGNATURE)), "");
    };

    RunTest(buffer, sizeof(buffer), onTlvParseDone, nullptr);
}
}
}
