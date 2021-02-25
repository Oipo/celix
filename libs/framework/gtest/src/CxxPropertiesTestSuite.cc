/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <gtest/gtest.h>

#include "celix/Properties.h"

using ::testing::MatchesRegex;

class CxxPropertiesTestSuite : public ::testing::Test {
public:
};

TEST_F(CxxPropertiesTestSuite, CreateDestroy) {
    celix::Properties props{};
    EXPECT_EQ(0, props.size());
}

TEST_F(CxxPropertiesTestSuite, FillAndLoop) {
    celix::Properties props{};
    EXPECT_EQ(0, props.size());

    props["key1"] = "value1";
    props.set("key2", "value2");
    props.set("key3", 3.3);
    props.set("key4", 4);
    props.set("key5", true);
    EXPECT_EQ(5, props.size());

    EXPECT_EQ(props.get("key1"), "value1");
    EXPECT_EQ(props.get("key2"), "value2");
    //TODO ASSERT_THAT(props.get("key3"), MatchesRegex("3.3.*"));
    EXPECT_EQ(props.getAsDouble("key3", 0), 3.3);
    EXPECT_EQ(props.get("key4"), "4");
    EXPECT_EQ(props.getAsLong("key4", -1), 4);
    EXPECT_EQ(props.get("key5"), "true");
    EXPECT_EQ(props.getAsBool("key5", false), true);

    int count = 0;
    for (const auto& pair : props) {
        EXPECT_NE(pair.first, "");
        count++;
    }
    EXPECT_EQ(5, count);
}

TEST_F(CxxPropertiesTestSuite, CopyTest) {
    celix::Properties props{};

    props["key1"] = "value1";
    props["key2"] = "value2";

    celix::Properties copy = props;
    copy["key1"] = "value1_new";

    std::string v1 = props["key1"];
    std::string v2 =  copy["key1"];
    EXPECT_EQ(v1, "value1");
    EXPECT_EQ(v2, "value1_new");
}

TEST_F(CxxPropertiesTestSuite, WrapTest) {
    auto *props = celix_properties_create();
    celix_properties_set(props, "test", "test");

    EXPECT_EQ(1, celix_properties_size(props));
    {
        auto cxxProps = celix::Properties::wrap(props);
        EXPECT_EQ(1, cxxProps->size());
        EXPECT_EQ(props, cxxProps->getCProperties());
    } //NOTE cxxProps out of scope, but will not destroy celix_properties
    EXPECT_EQ(1, celix_properties_size(props));

    celix_properties_destroy(props);
}
