#ifndef NVS_ESP32_H
#define NVS_ESP32_H
#include "Arduino.h"
#include <Preferences.h>
#include "string.h"

class NVS_EPPROM
{
public:
	NVS_EPPROM()
	{
		xSemaphoreNVS = xSemaphoreCreateMutex();
		xSemaphoreGive(xSemaphoreNVS);
	}
	void NVS_SetString(const char *name, const char *key, char *in);
	void NVS_GetString(const char *name, const char *key, char *out, size_t max_size);
	void NVS_SetUInt(const char *name, const char *key, uint32_t in);
	uint32_t NVS_GetUInt(const char *name, const char *key);
	void NVS_SetULong64(const char *name, const char *key, uint64_t in);
	uint64_t NVS_GetULong64(const char *name, const char *key);
	size_t getBytesLength(const char* key);
	size_t putBytes(const char* key, const void* value, size_t len);
	size_t getBytes(const char* key, void * buf, size_t maxLen);

	

private:
	char n[15];
	Preferences preferences;
	SemaphoreHandle_t xSemaphoreNVS = nullptr;

private:
};

extern NVS_EPPROM NVS;
#endif