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
        listDir("/DATA", 0);
        uint8_t count_data_dir = getFileCount("/DATA"); // check total dir in "/DATA"
        log_d("count data : %d", count_data_dir);
        while (count_data_dir > 4)
        {
            log_d("count data : %d", count_data_dir);
            removeDir(getFirstDirName("/DATA").c_str());
            count_data_dir = getFileCount("/DATA");
        }
        // del_first_line("/DATA/202009/09.txt");
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

    void appendFile(String path, String message);

    void write_log(String message)
    {
        if (!status)
        {
            return;
        }
        String path = get_path();
        if (path != "")
        {

            listDir("/DATA", 2);
            appendFile(path.c_str(), message.c_str());
        }
    }

private:
    bool status = false;
    SPIClass SDspi; //spi
    int8_t SPIss = -1;

    // custom
    bool del_first_dir()
    {
        return false;
    }
    String del_first_line(String path)
    {
        if (!SD.exists(path.c_str()))
        {
            return "";
        }
        File myFile = SD.open(path.c_str(), FILE_WRITE);
        if (myFile)
        {
            myFile.println("testing 1, 2, 3.");
            myFile.close();
            Serial.println("1st write done");
        }

        myFile = SD.open(path.c_str(), FILE_WRITE);
        if (myFile)
        {
            myFile.println("testing 4, 5, 6.");
            myFile.close();
            Serial.println("2nd write done");
        }

        return "";
    }
    //
    String get_path()
    {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo))
        {
            
            log_d("-----No time----");
            return "";
        }
        char buff[20] = {};
        sprintf(buff, "/DATA/%04d%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1);
        if (!existFile(String(buff)))
        {
            createDir(String(buff));
        }
        memset(buff, 0, 20);
        sprintf(buff, "/DATA/%04d%02d/%02d.txt", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
        return String(buff);
    }

    uint8_t getFileCount(String dir)
    {
        if (!SD.exists(dir.c_str()))
        {
            return 0;
        }
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
        if (!SD.exists(dir.c_str()))
        {
            return "";
        }
        String name = "";
        File root = SD.open(dir);
        File f = root.openNextFile();
        if (f)
        {
            name = f.name();
        }
        f.close();
        root.close();
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
    }
};

#endif