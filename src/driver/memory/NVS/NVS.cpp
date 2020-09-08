#include "NVS.h"

NVS_EPPROM NVS;

void NVS_EPPROM::NVS_SetString(const char *name, const char *key, char *in)
{
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(name, false);
		preferences.putString(key, in);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
}

void NVS_EPPROM::NVS_GetString(const char *name, const char *key, char *out, size_t max_size)
{
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		memset(out, 0, max_size);
		preferences.begin(name, true);
		preferences.getString(key, out, max_size);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
}

void NVS_EPPROM::NVS_SetUInt(const char *name, const char *key, uint32_t in)
{
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(name, false);
		preferences.putUInt(key, in);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
}

uint32_t NVS_EPPROM::NVS_GetUInt(const char *name, const char *key)
{
	uint32_t out = 0;
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(name, true);
		out = preferences.getUInt(key);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
	return out;
}

void NVS_EPPROM::NVS_SetULong64(const char *name, const char *key, uint64_t in)
{
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(name, false);
		preferences.putULong64(key, in);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
}

uint64_t NVS_EPPROM::NVS_GetULong64(const char *name, const char *key)
{
	uint32_t out = 0;
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(name, true);
		out = preferences.getULong64(key);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
	return out;
}
size_t NVS_EPPROM::getBytesLength(const char *key)
{
	size_t len = 0;
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(key);
		len = preferences.getBytesLength(key);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}

	return len;
}
size_t NVS_EPPROM::putBytes(const char *key, const void *value, size_t len)
{
	size_t t = 0;
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(key);
		t = preferences.putBytes(key, value, len);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
	return t;
}
size_t NVS_EPPROM::getBytes(const char *key, void *buf, size_t maxLen)
{
	size_t t = 0;
	if (xSemaphoreTake(xSemaphoreNVS, 1000))
	{
		preferences.begin(key);
		t = preferences.getBytes(key, buf, maxLen);
		preferences.end();
		xSemaphoreGive(xSemaphoreNVS);
	}
	return t;
}