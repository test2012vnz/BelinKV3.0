#ifndef _SDCARD_H_
#define _SDCARD_H_

#include <MD5Builder.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

class SDCardCLass
{

public:
    SDCardCLass(int8_t sck = -1, int8_t miso = -1, int8_t mosi = -1, int8_t ss = -1)
    {
        SPIss = ss;
        SDspi.begin(sck, miso, mosi, ss);
        begin();
        // print();
    }
    ~SDCardCLass()
    {
    }

    bool begin(uint32_t frequency = 4000000, const char *mountpoint = "/sd");
    void end();

    sdcard_type_t cardType();
    uint64_t cardSize();
    uint64_t totalBytes();
    uint64_t usedBytes();

    void listDir(const char *dirname, uint8_t levels);
    bool existFile(String path);
    bool createDir(String path);
    bool removeDir(String path);

private:
    bool status = false;
    SPIClass SDspi; //spi
    int8_t SPIss = -1;

    String get_path()
    {
        String path = "";
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            return path;
        }
        String p_month = "";
        path = "/" + String(timeinfo.tm_year + 1900) + "/" + String(timeinfo.tm_mon + 1) + "/" + String(timeinfo.tm_mday) + ".txt";
        if (!existFile(path))
        {
            createDir(path);
        }
        listDir("/", 1);
        return path;
    }

    uint8_t getFileCount(String dir)
    {
        File d = SD.open(dir);
        int count_files = 0;
        File file = d.openNextFile();
        while (file)
        {
            if (file.isDirectory())
            {
                count_files++;
            }
            file = d.openNextFile();
        }
        file.close();
        d.close();
        return count_files;
    }
    String getFirstDirName(String dir)
    {
        File d = SD.open(dir);
        String name = String(d.openNextFile().name());
        d.close();
        return name;
    }
    void print()
    {
        if (status)
        {
            log_d("\n---- SD Card Init OK ----");
            log_d("Used size: %d", usedBytes() / 1024);
            log_d("Card size: %d", cardSize() / 1024);
            log_d("\n------------------------");
        }
        else
        {
            log_d("---- SD Card not Init ----");
        }
        listDir("/DATA", 0);
        log_d("File count: %d", getFileCount("/DATA"));
        log_d("First name: %s", getFirstDirName("/DATA").c_str());
        removeDir(getFirstDirName("/DATA").c_str());
        log_d("\nFile count: %d", getFileCount("/DATA"));
        listDir("/DATA", 0);
        
    }
};

#endif