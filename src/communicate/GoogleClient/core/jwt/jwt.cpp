#include "jwt.h"

char* mbedtlsError(int errnum) {
    static char buffer[200];
    mbedtls_strerror(errnum, buffer, sizeof(buffer));
    return buffer;
}

char* createGCPJWT(const char* projectId, uint8_t* privateKey, size_t privateKeySize, 
					uint32_t iat, uint32_t exp) {
    char base64Header[50];
    const char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
    base64url_encode(
        (unsigned char *)header,   // Data to encode.
        strlen(header),            // Length of data to encode.
        base64Header);             // Base64 encoded data.

    exp+= iat;
    char payload[100];
    sprintf(payload, "{\"iat\":%d,\"exp\":%d,\"aud\":\"%s\"}", iat, exp, projectId);

    char base64Payload[100];
    base64url_encode(
        (unsigned char *)payload,  // Data to encode.
        strlen(payload),           // Length of data to encode.
        base64Payload);            // Base64 encoded data.

    uint8_t headerAndPayload[800];
    sprintf((char*)headerAndPayload, "%s.%s", base64Header, base64Payload);

	//using RSASSA
    mbedtls_pk_context pk_context;
    mbedtls_pk_init(&pk_context);
    int rc = mbedtls_pk_parse_key(&pk_context, privateKey, privateKeySize, NULL, 0);
    if (rc != 0) {
        log_d("Get error: %s", mbedtlsError(rc));
        return "";
    }

    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    const char* pers="MyEntropy";  
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, 
                    &entropy, (const unsigned char*)pers, strlen(pers));

    uint8_t digest[32];
    size_t digest_size = sizeof(digest);
    rc = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), headerAndPayload, 
    				strlen((char*)headerAndPayload), digest);
    if (rc != 0) {
        return "";
    }
    
    uint8_t oBuf[2000];
    size_t retSize = sizeof(oBuf);
    rc = mbedtls_pk_sign(&pk_context, MBEDTLS_MD_SHA256, digest, digest_size, oBuf, 
                    &retSize, mbedtls_ctr_drbg_random, &ctr_drbg);
    if (rc != 0) {
        return "";
    }

    char base64Signature[600];
    base64url_encode((unsigned char *)oBuf, retSize, base64Signature);
    char* retData = (char*)malloc(strlen((char*)headerAndPayload) + 1 + strlen((char*)base64Signature) + 1);
    sprintf(retData, "%s.%s", headerAndPayload, base64Signature);
    mbedtls_pk_free(&pk_context);
    return retData;
}