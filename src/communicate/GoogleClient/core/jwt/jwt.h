#ifndef __JWT_H__
#define __JWT_H__

#include <Arduino.h>
#include <mbedtls/pk.h>
#include <mbedtls/error.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include "base64url.h"

char* mbedtlsError(int errnum);
char* createGCPJWT(const char* projectId, uint8_t* privateKey, size_t privateKeySize, 
					uint32_t iat, uint32_t exp=3600);


#endif