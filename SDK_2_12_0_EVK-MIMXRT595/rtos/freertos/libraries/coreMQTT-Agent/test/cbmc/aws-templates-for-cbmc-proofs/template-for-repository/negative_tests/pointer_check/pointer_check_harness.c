/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

/**
 * A negative test for --pointer-check flag
 */
void pointer_check_harness() {
    int *src;
    int test = *src;
}
