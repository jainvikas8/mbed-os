/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
 *
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
#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"

#include "inc/test_framework_integ_test.h"

using namespace utest::v1;

void tfm_ns_regression()
{
    tfm_non_secure_client_run_tests();
    /* TODO: Enable the code below when patchset
       https://review.trustedfirmware.org/c/trusted-firmware-m/+/3172 is
       merged.
    */
    //uint32_t retval = tfm_non_secure_client_run_tests();
    // TEST_ASSERT_EQUAL_UINT32(0, retval);
}

// Test setup
utest::v1::status_t test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

const Case cases[] = {
    Case("TF-M NS regression tests", tfm_ns_regression),
};

Specification specification(test_setup, cases);

int main()
{
    return !Harness::run(specification);
}
