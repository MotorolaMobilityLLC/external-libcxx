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
test_makefile := external/libcxx/test/utilities/tuple/tuple.tuple/tuple.creation/Android.mk

test_name := utilities/tuple/tuple.tuple/tuple.creation/forward_as_tuple
test_src := forward_as_tuple.pass.cpp
include external/libcxx/test/Android.build.mk

test_name := utilities/tuple/tuple.tuple/tuple.creation/tie
test_src := tie.pass.cpp
include external/libcxx/test/Android.build.mk

test_name := utilities/tuple/tuple.tuple/tuple.creation/tuple_cat
test_src := tuple_cat.pass.cpp
include external/libcxx/test/Android.build.mk

test_name := utilities/tuple/tuple.tuple/tuple.creation/make_tuple
test_src := make_tuple.pass.cpp
include external/libcxx/test/Android.build.mk

include $(call all-makefiles-under,$(LOCAL_PATH))