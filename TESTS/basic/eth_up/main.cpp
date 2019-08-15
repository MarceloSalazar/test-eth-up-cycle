/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

#if MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == ETHERNET
  #define TEST_NETWORK_TYPE "Ethernet"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == WIFI
  #define TEST_NETWORK_TYPE "WiFi"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == MESH
  #define TEST_NETWORK_TYPE "Mesh"
#elif MBED_CONF_TARGET_NETWORK_DEFAULT_INTERFACE_TYPE == CELLULAR
  #define TEST_NETWORK_TYPE "Cellular"
#else
  #error [NOT_SUPPORTED] Either WiFi, Ethernet or Cellular network interface need to be enabled
#endif

#if !defined(MBED_CONF_APP_NO_LED)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
void led_thread() {
    led1 = !led1;
    led2 = !led1;
}
#endif

RawSerial pc(USBTX, USBRX);

void wait_nb(uint16_t ms) {
    wait_ms(ms);
}

void logger(const char* message, const char* decor) {
    wait_nb(10);
    pc.printf(message, decor);
    wait_nb(10);
}
void logger(const char* message) {
    wait_nb(10);
    pc.printf(message);
    wait_nb(10);
}
void test_failed() {
    greentea_send_kv("test_failed", 1);
}
void test_case_start(const char *name, size_t index) {
    wait_nb(10);
    pc.printf("\r\n>>> Running case #%u: '%s'...\n", index, name);
    GREENTEA_TESTCASE_START(name);
}
void test_case_finish(const char *name, size_t passed, size_t failed) {
    GREENTEA_TESTCASE_FINISH(name, passed, failed);
    wait_nb(10);
    pc.printf(">>> '%s': %u passed, %u failed\r\n", name, passed, failed);
}

void eth_up_testsuite(void) {
    int i = 0;
    int iteration = 0;
    char _key[20] = { };
    char _value[128] = { };
    char snum[30];

    greentea_send_kv("device_ready", true);
    while (1) {
        greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

        if (strcmp(_key, "iteration") == 0) {
            iteration = atoi(_value);
            break;
        }
    }

    // provide manifest to greentea so it can correct show skipped and failed tests
    if (iteration == 0) {
        greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_COUNT, 10);
        for (int i=0; i<10; i++){
            sprintf(snum, "Connect %3d", i);
            greentea_send_kv(GREENTEA_TEST_ENV_TESTCASE_NAME, snum);
        }
    }

    // Start network connection test.
    sprintf(snum, "Connect %3d", iteration);
    test_case_start(snum, 1);
        
    logger("[INFO] Attempting to connect to network.\r\n");

    // Connection definition.
    NetworkInterface *net = NetworkInterface::get_default_instance();
    nsapi_error_t net_status = -1;
    net_status = net->connect();
    // Report status to console.
    if (net_status != 0) {
        logger("[ERROR] Device failed to connect to Network.\r\n");
        test_failed();
    } else {
        logger("[INFO] Connected to network successfully. IP address: %s\n", net->get_ip_address());
    }

    sprintf(snum, "Connect %3d", iteration);
    test_case_finish(snum, (net_status == 0), (net_status != 0));

    if (iteration < 10) {
        logger("[INFO] Resetting device.\r\n");
        greentea_send_kv("test_advance", 0);
        while (1) {
            greentea_parse_kv(_key, _value, sizeof(_key), sizeof(_value));

            if (strcmp(_key, "reset") == 0) {
                wait(1);
                system_reset();
                break;
            }
        }
    }

    GREENTEA_TESTSUITE_RESULT(net_status);

    while (1) {
        wait(100);
    }
}

int main(void) {
    //Create a thread to blink an LED and signal that the device is alive
#if !defined(MBED_CONF_APP_NO_LED)
    Ticker t;
    t.attach(led_thread, 0.5);
#endif

    greentea_send_kv("device_booted", 1);

    GREENTEA_SETUP(240, "eth_host_tests");
    eth_up_testsuite();

    return 0;
}