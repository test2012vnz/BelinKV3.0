#include "sdcard.h"

bool SDCardCLass::begin(uint32_t frequency, const char *mountpoint)
{
    status = false;
    if (SPIss)
    {
        if (SD.begin(SPIss, SDspi, frequency, mountpoint))
        {
            log_d("---- SD Card Init OK ----");
            status = true;
        }
    }
    else
    {
        if (SD.begin())
        {
            log_d("---- SD Card Init OK ----");
            status = true;
        }
    }
    return status;
}

void SDCardCLass::end()
{
    if (status)
    {
        SD.end();
    }
}
uint64_t SDCardCLass::cardSize()
{
    if (status)
    {
        return SD.cardSize();
    }
    return 0;
}
uint64_t SDCardCLass::totalBytes()
{
    if (status)
    {
        return SD.totalBytes();
    }
    return 0;
}
uint64_t SDCardCLass::usedBytes()
{
    if (status)
    {
        return SD.usedBytes();
    }
    return 0;
}

bool SDCardCLass::createDir(String path)
{
    if (!status)
    {
        return false;
    }
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
    if (!status)
    {
        return;
    }
    File root = SD.open(dirname);
    if (!status || !root)
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
    if (!status)
    {
        return false;
    }
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
    if (!status)
    {
        return false;
    }
    if (SD.exists(path.c_str()))
    {
        return true;
    }
    return false;
}

void SDCardCLass::appendFile(String path, String message)
{
    if (!status)
    {
        return;
    }
    File file = SD.open(path.c_str(), FILE_APPEND);
    if (!file)
    {

        log_d("File append: FILE NOT EXISTS %s", path.c_str());
    }
    if (file.println(message.c_str()))
    {
        log_d("Appended to file: %s", path.c_str());
    }
    else
    {
        log_d("Appended FAILED to file: %s", path.c_str());
    }
    file.close();
}
