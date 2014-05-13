#
# Copyright (C) 2014 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
test_makefile := external/libcxx/test/input.output/iostreams.base/std.ios.manip/error.reporting/Android.mk

test_name := input.output/iostreams.base/std.ios.manip/error.reporting/make_error_condition
test_src := make_error_condition.pass.cpp
include external/libcxx/test/Android.build.mk

test_name := input.output/iostreams.base/std.ios.manip/error.reporting/iostream_category
test_src := iostream_category.pass.cpp
include external/libcxx/test/Android.build.mk

test_name := input.output/iostreams.base/std.ios.manip/error.reporting/make_error_code
test_src := make_error_code.pass.cpp
include external/libcxx/test/Android.build.mk

include $(call all-makefiles-under,$(LOCAL_PATH))