#include "sdcard.h"

bool SDCardCLass::begin(uint32_t frequency, const char *mountpoint)
{
    status = false;
    if (SPIss)
    {
        if (SD.begin(SPIss, SDspi, frequency, mountpoint))
        {
            status = true;
        }
    }
    else
    {
        if (SD.begin())
        {
            status = true;
        }
    }
    return status;
}

void SDCardCLass::end()
{
    SD.end();
}
uint64_t SDCardCLass::cardSize()
{
    return SD.cardSize();
}
uint64_t SDCardCLass::totalBytes()
{
    return SD.totalBytes();
}
uint64_t SDCardCLass::usedBytes()
{
    return SD.usedBytes();
}

bool SDCardCLass::createDir(String path)
{
    log_d("Creating Dir: %s", path.c_str());
    int breakDir = path.indexOf("/", 1);
    if (breakDir > 0)
    {
        do
        {
            String dir = path.substring(0, breakDir);
            if (!SD.mkdir(dir.c_str()))
            {
                return false;
            }
            breakDir = path.indexOf("/", breakDir + 1);
        } while (breakDir > 0);
    }

    if (!SD.mkdir(path.c_str()))
    {
        log_d("mkdir failed");
        return false;
    }
    log_d("mkdir done");
    return true;
}

void SDCardCLass::listDir(const char *dirname, uint8_t levels)
{
    File root = SD.open(dirname);
    if (!root)
    {
        log_d("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        log_d("Not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            log_d("  DIR : %s", file.name());
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            log_d("  FILE: %s  SIZE: %d", file.name(), file.size());
        }
        file = root.openNextFile();
    }
    file.close();
    root.close();
}

bool SDCardCLass::removeDir(String path)
{
    log_d("Removing Dir: %s", path.c_str());
    if (SD.rmdir(path.c_str()))
    {
        log_d("rmdir done");
        return true;
    }
    log_d("rmdir failed");
    return false;
}

bool SDCardCLass::existFile(String path)
{
    if (SD.exists(path.c_str()))
    {
        return true;
    }
    return false;
}