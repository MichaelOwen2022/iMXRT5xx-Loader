#
# Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

HARNESS_ENTRY=harness
HARNESS_FILE=MQTT_MatchTopic_harness
PROOF_UID=MQTT_MatchTopic

# The topic name/filter length are bounded, so that the loops in topic matching algorithmic
# functions called by MQTT_MatchTopic can be unwound to an expected
# amount that won't make the proof run too long.
MAX_TOPIC_NAME_FILTER_LENGTH=10

DEFINES += -DMAX_TOPIC_NAME_FILTER_LENGTH=$(MAX_TOPIC_NAME_FILTER_LENGTH)
INCLUDES +=

REMOVE_FUNCTION_BODY +=
UNWINDSET += __CPROVER_file_local_core_mqtt_c_matchTopicFilter.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)
UNWINDSET += strncmp.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)
UNWINDSET += __CPROVER_file_local_core_mqtt_c_matchWildcards.0:$(MAX_TOPIC_NAME_FILTER_LENGTH)

PROOF_SOURCES += $(PROOFDIR)/$(HARNESS_FILE).c
PROOF_SOURCES += $(SRCDIR)/test/cbmc/sources/mqtt_cbmc_state.c
PROJECT_SOURCES += $(SRCDIR)/source/core_mqtt.c

include ../Makefile.common
