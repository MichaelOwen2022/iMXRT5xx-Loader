/*
 * FreeRTOS V202107.00
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef AWS_TEST_OTA_PAL_RSA_SHA1_SIGNATURE_H_
#define AWS_TEST_OTA_PAL_RSA_SHA1_SIGNATURE_H_

#include <stdint.h>

/**
 * @brief Invalid signature for OTA PAL testing.
 */
static const uint8_t ucInvalidSignature[] =
{
    0x1a, 0x90, 0xe6, 0xd8, 0x25, 0x0b, 0xa5, 0x53, 0xa1, 0xb3, 0x10, 0xdb,
    0x5b, 0x36, 0xda, 0x81, 0xbf, 0x77, 0x63, 0xc1, 0xc9, 0xe6, 0xd2, 0x5e,
    0x2e, 0x2c, 0x32, 0xbc, 0x0f, 0x78, 0xc4, 0x42, 0xf2, 0xfb, 0x20, 0x39,
    0x0a, 0xdc, 0xbd, 0x52, 0x68, 0x6a, 0xb4, 0xfa, 0xf5, 0xc0, 0x04, 0xb9,
    0x5a, 0x7e, 0xf2, 0x44, 0x72, 0x90, 0x21, 0x0c, 0xe5, 0x4a, 0x9f, 0x8d,
    0x9d, 0x6f, 0x54, 0x2c, 0x57, 0x20, 0x4e, 0xdf, 0xf8, 0x00, 0x6f, 0xc0,
    0x91, 0xc3, 0xbe, 0xe7, 0x0a, 0xe2, 0x65, 0x82, 0x00, 0xaa, 0xf8, 0x8b,
    0xe6, 0x0a, 0x58, 0xe3, 0x4a, 0x86, 0xe4, 0xce, 0x20, 0xd5, 0x4a, 0x39,
    0x2d, 0x7b, 0xef, 0x71, 0x99, 0x62, 0xf3, 0xa9, 0xe1, 0x21, 0x28, 0x73,
    0x79, 0x3d, 0x60, 0xe9, 0x1d, 0x38, 0xb0, 0x65, 0x88, 0xef, 0x06, 0x83,
    0x14, 0x72, 0xd4, 0x29, 0x34, 0x0f, 0x43, 0x87, 0x9a, 0xf8, 0x09, 0xb8,
    0xec, 0xc6, 0x4d, 0x2f, 0x5f, 0xc8, 0xf0, 0x09, 0x25, 0x03, 0x05, 0x30,
    0x96, 0x26, 0x48, 0xc9, 0x2e, 0xaf, 0xfb, 0xb5, 0x4e, 0xbe, 0xbe, 0x99,
    0xc3, 0xb4, 0x80, 0xd8, 0xd3, 0x33, 0xf7, 0xc9, 0xf9, 0x16, 0xfe, 0xed,
    0xef, 0xaa, 0x28, 0xf4, 0x65, 0x80, 0xb3, 0x6a, 0xe4, 0xb5, 0x1d, 0x72,
    0x62, 0xb7, 0xb8, 0x26, 0x62, 0xbf, 0x2b, 0xa7, 0x89, 0x3d, 0x45, 0x69,
    0x3e, 0x65, 0x7a, 0x89, 0x05, 0x83, 0xe8, 0x14, 0x57, 0xe2, 0x6b, 0x64,
    0x7c, 0xba, 0xe2, 0xd6, 0xee, 0xfe, 0x49, 0x56, 0x20, 0x27, 0x70, 0x2c,
    0x77, 0x46, 0xf7, 0x9d, 0x6c, 0x61, 0x41, 0xe0, 0xdf, 0x2c, 0xe2, 0xd0,
    0x44, 0x6f, 0x4a, 0xd8, 0x16, 0x20, 0xf9, 0xcf, 0x2d, 0x67, 0x79, 0x10,
    0x1c, 0x86, 0xf6, 0xf4, 0xe9, 0xb0, 0x0e, 0x40, 0x32, 0xce, 0x11, 0xde,
    0x28, 0x86, 0x43, 0x6d
};

static int ucInvalidSignatureLength = 256;

/**
 * @brief Valid signature matching the test block in the OTA PAL tests.
 */
static const uint8_t ucValidSignature[] =
{
    0x14, 0x79, 0x9d, 0x06, 0x72, 0x31, 0x8a, 0x45, 0xb0, 0x2a, 0xb8, 0x56,
    0xc6, 0x48, 0x4a, 0xd5, 0xae, 0x1a, 0xe8, 0xe9, 0x3d, 0x35, 0x75, 0xa9,
    0x0b, 0x76, 0x3a, 0x12, 0x5d, 0xf9, 0x99, 0x80, 0x9b, 0xca, 0xff, 0xe1,
    0xa0, 0x2f, 0x1a, 0x63, 0x3f, 0xee, 0xa3, 0x05, 0x13, 0xc1, 0xf0, 0x71,
    0x90, 0xa9, 0x97, 0xac, 0xa0, 0x6e, 0x30, 0x59, 0xaa, 0xd5, 0x6b, 0x6f,
    0x0c, 0x9a, 0x76, 0x2c, 0xa2, 0xd3, 0x5a, 0x81, 0xda, 0xb2, 0x90, 0x4d,
    0x8f, 0x0d, 0x4d, 0xa9, 0x0a, 0xcd, 0x6d, 0xaa, 0x3e, 0x54, 0x68, 0x67,
    0x10, 0x03, 0x13, 0xb7, 0xfa, 0xa3, 0x85, 0x56, 0x2b, 0xc2, 0x1d, 0x20,
    0x7b, 0x50, 0xed, 0x90, 0xdd, 0x27, 0xf9, 0x24, 0xf2, 0xcc, 0x21, 0x0d,
    0x75, 0x9f, 0xa6, 0xdd, 0x62, 0xc9, 0xca, 0xb5, 0x0f, 0xec, 0x1a, 0x3a,
    0xe6, 0x08, 0x84, 0xd6, 0xde, 0x59, 0x4b, 0x37, 0xa8, 0xce, 0x28, 0x5f,
    0x2e, 0x74, 0xca, 0x47, 0x0d, 0x0f, 0x30, 0xc4, 0xbe, 0xaf, 0xf3, 0x6c,
    0xc7, 0x0d, 0x2d, 0x03, 0x0d, 0x05, 0xd3, 0xe5, 0xbd, 0x80, 0xc1, 0x3e,
    0xa8, 0x56, 0x2d, 0xbb, 0x9e, 0x17, 0x2b, 0xcd, 0xce, 0x37, 0x45, 0x36,
    0x07, 0x64, 0xdc, 0xbc, 0x15, 0x84, 0x5e, 0xca, 0xd9, 0xa6, 0xab, 0xd9,
    0x23, 0xcd, 0xe4, 0x8a, 0x4e, 0xee, 0x74, 0xcf, 0xb3, 0x0d, 0x24, 0x28,
    0xeb, 0x95, 0xec, 0xa8, 0xb1, 0x70, 0x29, 0x08, 0x46, 0x82, 0xcd, 0x33,
    0xcb, 0x90, 0x33, 0x59, 0xcd, 0x7c, 0xbf, 0xbb, 0x8b, 0x7c, 0xc6, 0xbe,
    0x18, 0x36, 0xf7, 0x01, 0x4e, 0xe0, 0x12, 0x81, 0xf9, 0x2d, 0x7a, 0x04,
    0x7f, 0x0e, 0x2b, 0xe6, 0x80, 0xd5, 0xb3, 0x87, 0x33, 0x04, 0xcb, 0x9c,
    0x05, 0x93, 0xb0, 0x02, 0x37, 0xf1, 0x1d, 0x8b, 0x96, 0x9f, 0xdc, 0x9c,
    0xad, 0xe0, 0x8e, 0xd3
};
static const int ucValidSignatureLength = 256;

/**
 * @brief The type of signature method this file defines for the valid signature.
 */
#define otapalconfigCODE_SIGNING_CERTIFICATE                             \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "MIIDPDCCAiSgAwIBAgIBATANBgkqhkiG9w0BAQUFADB8MQswCQYDVQQGEwJJTDEP\n" \
    "MA0GA1UECAwGU2hhcm9uMRAwDgYDVQQHDAdSYWFuYW5hMR8wHQYDVQQKDBZUZXhh\n" \
    "cyBJbnN0cnVtZW50cyBJbmMuMQwwCgYDVQQLDANSTkQxGzAZBgNVBAMMEmR1bW15\n" \
    "LXJvb3QtY2EtY2VydDAeFw0xODA2MjcxOTI4NTNaFw0xOTA2MjcxOTI4NTNaMCEx\n" \
    "HzAdBgNVBAMMFnRlc3Rfc2lnbmVyQGFtYXpvbi5jb20wggEiMA0GCSqGSIb3DQEB\n" \
    "AQUAA4IBDwAwggEKAoIBAQC5craIexo46oq3OTj+3fLz17MCtw/icXku8sJ16el0\n" \
    "29Cibq4u8bdJeDnGlc42UySw0UlodWnzd5I52tXoXcQZs++m6uQ3JvvRHFXpNuV6\n" \
    "Cyo//AdQ2MoqdKK8/pZqu4X71/xXHcIMBb3kS1RSkunJB65mPuPOfjcem6IXDtRk\n" \
    "E346aN6wMfeIlmwOoFK4RBoWse459ELB5aq0tSwUmHv7UcyHYQf4Cxkz5HjWfx0y\n" \
    "NYKCR4tPoudjg8E9R6uqik7DFlqrijNW2XdXS67d/cVToDJFn1SnYcHStARAoZUK\n" \
    "gP0JwXTapO31QThi9q+q63FETBcnEQ29rfmeN1/pBOI/AgMBAAGjJDAiMAsGA1Ud\n" \
    "DwQEAwIHgDATBgNVHSUEDDAKBggrBgEFBQcDAzANBgkqhkiG9w0BAQUFAAOCAQEA\n" \
    "W29xvDV66B7NYAOSqXIG11JH+Pk2GFzZl5gNehkcDPPMHumBRNHHZqVAO4mah3Ej\n" \
    "StqWmTXnIs5YKX9Ro7ch3NfKHMt4/JFvR7v9VolVIVwZZB+AQU1BD8OAGlHNEq28\n" \
    "TbOdkpdv9Q69W4TVsqXAkVhONekLkEJQTZyhW7db28nb/LizftfN4ps+uuE2Xl9c\n" \
    "YHmgWb/xqi9NIcsyQL08urJVCnyGuLQgj+GfitELFsCfc3ohhacNENsXupRIOz08\n" \
    "NCa9WuCyk5uwoo6mn6JIErBMLqLTBcs82vq9d7WIFHf4QpgTs2FuelY/Hyw7HRFo\n" \
    "Ml3tXnR4B4lqeJy/BP6/GA==\n"                                         \
    "-----END CERTIFICATE-----"

#endif /* AWS_TEST_OTA_PAL_RSA_SHA1_SIGNATURE_H_  */
