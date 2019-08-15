## ----------------------------------------------------------------------------
## Copyright 2016-2018 ARM Ltd.
##
## SPDX-License-Identifier: Apache-2.0
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
## ----------------------------------------------------------------------------

from mbed_host_tests import BaseHostTest
from mbed_host_tests.host_tests_logger import HtrunLogger

import os
import time
import hashlib
import subprocess
import re
import signal
import pkg_resources


DEFAULT_CYCLE_PERIOD = 1.0

class SDKTests(BaseHostTest):
    __result = None
    iteration = 0
    boot_cycles = 0

    def send_safe(self, key, value):
        self.send_kv(key, value)
        self.send_kv(key, value)
        self.send_kv(key, value)
        self.send_kv(key, value)
        self.send_kv(key, value)

    def _callback_device_booted(self, key, value, timestamp): 
        # This is used to let the device boot normally
        self.send_safe('__sync', 0)

    def _callback_device_ready(self, key, value, timestamp):
        # Send device iteration number after a reset
        self.boot_cycles += 1
        self.send_safe('iteration', self.iteration)

    def _callback_test_advance(self, key, value, timestamp):
        # Advance test sequence
        self.iteration = self.iteration + 1
        self.send_safe('reset', 0)

    def _callback_test_failed(self, key, value, timestamp):
        # Test failed. End it.
        self.notify_complete(False)


    """
    Host setup routines
    """
    def setup(self):
        # Generic test routines
        self.register_callback('device_booted', self._callback_device_booted)
        self.register_callback('device_ready', self._callback_device_ready)
        self.register_callback('test_advance', self._callback_test_advance)
        self.register_callback('test_failed', self._callback_test_failed)

        self.iteration = 0
        self.boot_cycles = 0


    def result(self):
        return self.__result

    def teardown(self):
        pass

    def __init__(self):
        super(SDKTests, self).__init__()
        self.logger = HtrunLogger('TEST')