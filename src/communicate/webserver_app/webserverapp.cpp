#include "webserverapp.h"

WebServer *WebserverApplication::server = nullptr;
WebSocketsServer *WebserverApplication::webSocket = nullptr;
DynamicJsonBuffer *WebserverApplication::jsonapi = nullptr;
bool WebserverApplication::WebSocketsStatus = false;

ACCOUNT_ENUM WebserverApplication::account = Account_None;
Driver *WebserverApplication::driver = nullptr;

void WebserverApplication::init()
{
    server->on("/", HTTP_GET, [&]() {
        if (account)
        {
            if (!handleFileRead("/index.html"))
                server->send(404, "text/plain", "FileNotFound");
        }
        else
        {
            if (!handleFileRead("/login.html"))
                server->send(404, "text/plain", "FileNotFound");
        }
    });
    server->on("/index.css", HTTP_GET, [&]() {
        if (!handleFileRead("/index.css"))
            server->send(404, "text/plain", "FileNotFound");
    });
    server->on("/index.js", HTTP_GET, [&]() {
        if (!handleFileRead("/index.js"))
            server->send(404, "text/plain", "FileNotFound");
    });
    server->on("/bootstrap.min.css", HTTP_GET, [&]() {
        if (!handleFileRead("/bootstrap.min.css.gz"))
            server->send(404, "text/plain", "FileNotFound");
    });
    server->on("/jquery-3.3.1.slim.min.js", HTTP_GET, [&]() {
        if (!handleFileRead("/jquery-3.3.1.slim.min.js.gz"))
            server->send(404, "text/plain", "FileNotFound");
    });
    server->on("/bootstrap-4.3.1.min.js", HTTP_GET, [&]() {
        if (!handleFileRead("/bootstrap-4.3.1.min.js.gz"))
            server->send(404, "text/plain", "FileNotFound");
    });
    server->on("/logo-solarbk.png", HTTP_GET, [&]() {
        if (!handleFileRead("/logo-solarbk.png"))
            server->send(404, "text/plain", "FileNotFound");
    });
    // server->onNotFound([&]{
    //     log_d("url: %s", server->uri().c_str());
    // });
    // upload file
    server->on("/f", HTTP_GET, []() {
        String path = server->arg("path");
        log_d("----> path -> %s", path.c_str());

        if (SD.exists(path))
        {
            File file = SD.open(path.c_str());
            server->sendHeader("Content-Disposition", "attachment; filename=" + path.substring(path.lastIndexOf("/") + 1, path.length()));
            size_t sent = server->streamFile(file, "application/octet-stream");
            file.close();
        }
        else
        {
            server->send(200, "text/xml");
        }
    });
    // Handle
    server->on("/c", HTTP_POST, this->Webserver_Handler);
    // OTA
    server->on(
        "/update", HTTP_POST, []() {
		server->sendHeader("Connection", "close");
		server->send(200, "text/xml", (Update.hasError()) ? (String("Đã xảy ra lỗi!\n") + "ERR") : "Thành công!");
		ESP.restart(); }, []() {
			HTTPUpload& upload = server->upload();
			if (upload.status == UPLOAD_FILE_START)
			{
				Serial.setDebugOutput(true);
				Serial.printf("Update: %s\n", upload.filename.c_str());

				String md5 = String(server->arg("md5"));
				if (Update.begin())   //start with max available size
				{
					Update.setMD5(md5.c_str());
				}
				else
				{
					Update.printError(Serial);
				}
			}
			else if (upload.status == UPLOAD_FILE_WRITE)
			{
				if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
				{
					Update.printError(Serial);
				}
			}
			else if (upload.status == UPLOAD_FILE_END)
			{
				if (Update.end(true))   //true to set the size to the current progress
				{
					log_d("Update Success: %u\nRebooting...\n", upload.totalSize);
					server->send(200, "text/xml", (Update.hasError()) ? (String("Đã xảy ra lỗi!\n") + "ERR") : "Thành công!");
				}
				else
				{
					Update.printError(Serial);
				}
				Serial.setDebugOutput(false);
			} });
}

void WebserverApplication::Webserver_Handler()
{
    String response = "";
    String json = "";
    int m = server->arg("m").toInt();
    if (server->hasArg("js"))
    {
        json = server->arg("js");
    }
    switch (m)
    {
    case 0:
    {
    }
    break;
    case 2:
    {
        JsonObject &root = jsonapi->parseObject(json);
        if (root.containsKey("WIFI"))
        {
            Wifi_Structure w = (driver->get_netif()).wifi;
            int en = root["WIFI"].as<int>();
            driver->set_wifi(en, w.SSID, w.PASS);
        }
        else if (root.containsKey("ETHERNET"))
        {
            Ethernet_Structure w = (driver->get_netif()).eth;
            int en = root["ETHERNET"].as<int>();
            driver->set_eth(en, w.DHCP, w.ip, w.gateway, w.subnet); // enable dhcp
        }
        else if (root.containsKey("3G"))
        {
            int en = root["3G"].as<int>();
            driver->set_gsm(en);
        }

        server->send(200, "text/xml", DeviceStatus(true));
    }
    break;
    case 3:
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.disconnect();
        }
        int8_t nn = WiFi.scanNetworks(false, true);
        response = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
        response += "<wifi>";
        for (uint8_t i = 0; i < nn; i++)
        {
            String ssid = WiFi.SSID(i);
            ssid.replace("<", "");
            ssid.replace(">", "");
            unsigned int rssi = map(WiFi.RSSI(i), (-90), (-30), 0, 100);
            if (ssid.length() == 0)
            {
                ssid = "?";
            }
            response += "<net>";
            response += "<ssid>" + String(ssid) + "</ssid>";
            response += "<rssi>" + String(rssi) + "</rssi>";
            response += "</net>";
        }
        response += "</wifi>";
        server->send(200, "text/xml", response);
    }
    break;
    case 4: // cai dat wifi
    {
        JsonObject &root = jsonapi->parseObject(json);
        String ssid = root["ssid"].as<String>();
        String pwd = root["pwd"].as<String>();
        driver->set_wifi(1, (char *)ssid.c_str(), (char *)pwd.c_str());
        server->send(200, "text/xml", "ok");
    }
    break;
    case 5:
    { // cai dat ethernet
        JsonObject &root = jsonapi->parseObject(json);
        if (root["DHCP"].as<int>())
        {
            uint32_t ip = htonl(root["ip"].as<uint32_t>());
            uint32_t gw = htonl(root["gateway"].as<uint32_t>());
            uint32_t sub = htonl(root["subnet"].as<uint32_t>());
            driver->set_eth(1, 0, ip, gw, sub); // ip tinh
        }
        else
        {
            driver->set_eth(1, 1, 0, 0, 0); // ip dong //
        }
        // conf->sd->LOG(CONFIG, String("Net_conf\t") + json);
        server->send(200, "text/xml", DeviceStatus(true));
    }
    break;
    case 6:
    {
        JsonObject &root = jsonapi->parseObject(json);
        Device_Name_Can_Be_Read dev = (Device_Name_Can_Be_Read)root["device"].as<int>();
        bool ret = false;
        int ss = root["ss"].as<uint8_t>(); // check sunspec device
        if (root["protocol"] == "RTU")
        {
            int id = root["id"].as<uint8_t>();

            if (ss)
            {
                ret = driver->add(dev, id, (uint8_t)1, (uint8_t)1, (uint16_t)40000);
            }
            else
            {
                ret = driver->add(dev, id, (uint8_t)1, (uint8_t)0, (uint16_t)0);
            }
        }
        else if (root["protocol"] == "TCP")
        {
            uint32_t id = root["id"].as<uint8_t>();
            uint32_t ip = root["ip"].as<uint32_t>();
            int32_t port = root["port"].as<int32_t>();

            if (ss)
            {
                ret = driver->add(dev, id, __bswap_32((uint32_t)ip), (int32_t)port, (uint8_t)1, (uint16_t)40000);
            }
            else
            {
                ret = driver->add(dev, id, __bswap_32((uint32_t)ip), (int32_t)port, (uint8_t)0, (uint16_t)0);
            }
        }
        else if (root["protocol"] == "IEC")
        {
            String id = root["id"].as<String>();
            uint16_t primary = root["vnsn3TPrimary"].as<uint16_t>();
            uint16_t secondary = root["vnsn3TSecondary"].as<uint16_t>();

            log_d("set iec");
            log_d("id %s", id.c_str());

            ret = driver->add(dev, (char *)id.c_str(), (uint8_t)1, primary, secondary);
        }
        // conf->sd->LOG(CONFIG, String("Device_Add\t") + json);
        server->send(200, "text/xml", DeviceStatus(ret));
    }
    break;
    case 7:
    { // delete device
        JsonObject &root = jsonapi->parseObject(json);
        Device_Name_Can_Be_Read dev = (Device_Name_Can_Be_Read)root["device"].as<int>();
        int ss = root["ss"].as<uint8_t>(); // check sunspec device
        bool ret = false;
        if (root["protocol"] == "RTU")
        {
            uint8_t id = root["id"].as<uint8_t>();
            ret = driver->delRTU(dev, id, 1, ss);
        }
        else if (root["protocol"] == "TCP")
        {
            uint8_t id = root["id"].as<uint8_t>();
            uint32_t ip = root["ip"].as<uint32_t>();
            int32_t port = root["port"].as<int32_t>();
            ret = driver->delTCP(dev, id, __bswap_32((uint32_t)ip), port, ss);
        }
        else if (root["protocol"] == "IEC")
        {
            String IEC_ID = root["id"].as<String>();
            uint16_t primary = (uint16_t)root["vnsn3TPrimary"].as<int>();
            uint16_t secondary = (uint16_t)root["vnsn3TSecondary"].as<int>();

            ret = driver->delIEC(dev, (char *)IEC_ID.c_str(), (uint8_t)1, primary, secondary);
        }
        // conf->sd->LOG(CONFIG, String("Device_Del\t") + json);
        server->send(200, "text/xml", DeviceStatus(ret));
    }
    break;
    case 8:
    {
        String sn = server->arg("Serial");
        driver->update_serial_number(sn);
        server->send(200, "text/xml", "OK");
    }
    break;
    case 20: // kiem tra user khi login
    case 21:
    {
        String user = server->arg("user");
        String pass = "", log = "";
        if (m == 21)
        {
            pass = server->arg("pass");
        }
        if ((user == (m == 20 ? "YWRtaW4=" : "admin")) && (m == 20 ? 1 : (pass == "solarbk.vn")))
        {
            account = Account_Admin;
            response = m == 20 ? "login" : "YWRtaW4=";
            log += "admin user login";
        }
        else if ((user == (m == 20 ? "bWFudQ==" : "manu")) && (m == 20 ? 1 : (pass == "1111")))
        {
            account = Account_Manufacturer;
            response = m == 20 ? "login" : "bWFudQ==";
            log += "manufacturer user login";
        }
        else if ((user == (m == 20 ? "aW5zdA==" : "inst")) && (m == 20 ? 1 : (pass == "1234")))
        {
            account = Account_Installer;
            response = m == 20 ? "login" : "aW5zdA==";
            log += "installer user login";
        }
        else if ((user == (m == 20 ? "dXNlcg==" : "user")) && (m == 20 ? 1 : (pass == "0000")))
        {
            account = Account_User;
            response = m == 20 ? "login" : "dXNlcg==";
            log += "user login";
        }

        server->send(200, "text/plain", response);
    }
    break;
    case 22:
    {
        account = Account_None;
        server->send(200, "text/plain", "LOGOUT");
    }
    break;
    case 83: {
        JsonObject &root = jsonapi->parseObject(json);
        String project = root["project"].as<String>();
        String region = root["region"].as<String>();
        String registry = root["registry"].as<String>();
        String device = root["device"].as<String>();
        String key = root["key"].as<String>();
        log_d("\-----DEVICE NAME-----\n");
        log_d("project: %s", project.c_str());
        log_d("region: %s", region.c_str());
        log_d("registry: %s", registry.c_str());
        log_d("devcice: %s", device.c_str());
        log_d("key: %s", key.c_str());
        driver->set_ggc((char *)project.c_str(), (char *)region.c_str(), (char *)registry.c_str(), (char *)device.c_str(), (char *)key.c_str());
        server->send(200, "text/plain", "OK");
    } break;
        case 99:
    { //  first load data
        server->send(200, "text/xml", DeviceStatus(true));
    }
    break;
    }
}

void WebserverApplication::Websocket_Handler(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch ((int)type)
    {
    case WStype_DISCONNECTED:
    {
        log_d("[%d] Websocket Disconnected!", num);
        WebSocketsStatus = false;
    }
    break;
    case WStype_CONNECTED:
    {
        WebSocketsStatus = true;
        log_d("[%d] Websocket Connected!", num);
    }
    break;
    case WStype_TEXT:
    {
        String js = (char *)payload;
        JsonObject &root = jsonapi->parseObject(js);
        log_d("-->%s", js.c_str());
        int cm = root["cm"].as<int>();
        switch (cm)
        {
        case 1:
        {
        }
        break;
        case 2:
        {
        }
        break;
        case 3:
        {
        }
        break;
        case 4:
        {
            String path = root["path"].as<String>();
            JsonObject &f_root = jsonapi->createObject();
            if (path.endsWith(".txt"))
            {
            }
            else
            {
                f_root["FILE"] = path;
                JsonArray &array = f_root.createNestedArray("array");
                // conf->sd->listDir(path.c_str(), 1);
                File root_path = SD.open(path.c_str());
                File file = root_path.openNextFile();

                while (file)
                {
                    //Serial.println(file.name());
                    array.add(String(file.name()));
                    if (file.isDirectory())
                    {
                        array.add("");
                    }
                    else
                    {
                        array.add(String(file.size()));
                    }
                    file = root_path.openNextFile();
                }
                path = "";
                f_root.printTo(path);
                // log_d("list -> %s", path.c_str());
                String s = "{\"cm\":4, \"path\":" + path + "}";
                webSocket->sendTXT(num, s);
            }
        }
        break;
        }
    }
    break;
    }
}

void WebserverApplication::web_task(void *arg)
{
    log_d("-- Webserver Task begin ---");
    for (;;)
    {
        server->handleClient();
        webSocket->loop();
        vTaskDelay(1);
    }
}

String WebserverApplication::getContentType(String filename)
{
    if (server->hasArg("download"))
        return "application/octet-stream";
    else if (filename.endsWith(".gz"))
        return "gzip"; /*application/x-gzip */
    else if (filename.endsWith(".htm"))
        return "text/html";
    else if (filename.endsWith(".html"))
        return "text/html";
    else if (filename.endsWith(".css"))
        return "text/css";
    else if (filename.endsWith(".js"))
        return "application/javascript";
    else if (filename.endsWith(".png"))
        return "image/png";
    else if (filename.endsWith(".gif"))
        return "image/gif";
    else if (filename.endsWith(".jpg"))
        return "image/jpeg";
    else if (filename.endsWith(".ico"))
        return "image/x-icon";
    else if (filename.endsWith(".xml"))
        return "text/xml";
    else if (filename.endsWith(".pdf"))
        return "application/x-pdf";
    else if (filename.endsWith(".zip"))
        return "application/x-zip";
    return "text/plain";
}

bool WebserverApplication::handleFileRead(String path)
{
    String contentType = getContentType(path);
    File file = SPIFFS.open(path, "r");
    server->streamFile(file, contentType); //size_t sent =
    file.close();
    return true;
}

String WebserverApplication::DeviceStatus(bool ret) //
{
    jsonapi->clear();
    JsonObject &root = jsonapi->createObject();
    NetIF_Task_Structure net = driver->get_netif();
    root["ret"] = (int)ret;

    JsonObject &connection = root.createNestedObject("connection");
    JsonObject &jwifi = connection.createNestedObject("wifi");
    jwifi["en"] = net.wifi.ENABLE;

    jwifi["connected"] = net.wifi.STATUS;
    jwifi["ssid"] = String(net.wifi.SSID);
    jwifi["rssi"] = map(WiFi.RSSI(), (-90), (-30), 0, 100);
    JsonObject &eth = connection.createNestedObject("eth");
    eth["en"] = net.eth.ENABLE;
    eth["DHCP"] = net.eth.DHCP;
    eth["ip"] = __builtin_bswap32(driver->net->eth_localIP());
    eth["subnet"] = __builtin_bswap32(driver->net->eth_subnetMask());
    eth["gateway"] = __builtin_bswap32(driver->net->eth_gatewayIP());
    JsonObject &gsm = connection.createNestedObject("gsm");
    gsm["en"] = net.gsm.ENABLE;
    gsm["ser"] = driver->gsm_service();
    gsm["rssi"] = driver->gsm_signal();

    JsonObject &device = root.createNestedObject("device");
    device["list_device"] = driver->devicesListString();

    System_Info_Structure sysIf = driver->get_sysif();

    JsonObject &info = root.createNestedObject("info");

    info["Serial"] = String(sysIf.Serial_Number);
    info["Reset"] = sysIf.Reset_Count;
    info["Lifetime"] = sysIf.Life_Time;

    info["Runningtime"] = driver->sysDriver->get_running_time(); //
    info["sdsize"] = 0;                                          //cardSize;
    info["sdusedsize"] = 0;                                      //usedSize;
    info["totaldevice"] = driver->total_devices();
    info["totaldevCon"] = driver->total_devices_connected();
    info["lastssoc"] = driver->net->Last_SSOC_Unix();

    String js;
    root.printTo(js);
    return js;
}