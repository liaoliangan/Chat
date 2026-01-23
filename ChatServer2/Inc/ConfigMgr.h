#ifndef CONFIGMGR_H
#define CONFIGMGR_H

#include "const.h"
#include "Singleton.h"
struct SectionInfo
{
    SectionInfo();
    ~SectionInfo();
    SectionInfo(const SectionInfo &sectionInfo);
    SectionInfo &operator=(const SectionInfo &sectionInfo);
    std::map<std::string, std::string> _section_datas;
    std::string operator[](const std::string &key);
    std::string GetValue(const std::string &key);
};

class ConfigMgr
{
    friend class Singleton<ConfigMgr>;

public:
    ~ConfigMgr();
    ConfigMgr(const ConfigMgr &src) = delete;
    ConfigMgr &operator=(const ConfigMgr &src) = delete;
    SectionInfo operator[](const std::string &section);
    static ConfigMgr &GetInstance();
    std::string GetValue(const std::string &section, const std::string &key);

private:
    ConfigMgr();
    std::map<std::string, SectionInfo> _config_map;
};

/*----------------------------------------------------------------------------*/
// 通过shell读取配置文件
/*
std::string exec(const char *cmd)
{
    char buffer[128];
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
    {
        result += buffer;
    }
    // 去掉换行符
    if (!result.empty() && result.back() == '\n')
        result.pop_back();
    return result;
}

std::string getConfig(const std::string &file, const std::string &section, const std::string &key)
{
    std::string cmd = "../script/read_config.sh " + file + " " + section + " " + key;
    return exec(cmd.c_str());
}

int main()
{
    std::string host = getConfig("config.ini", "GateServer", "Port");
    std::string port = getConfig("config.ini", "VerifyServer", "Port");

    std::cout << "GateServer Port: " << host << std::endl;
    std::cout << "VerifyServer Port: " << port << std::endl;

    return 0;
}
*/

/*----------------------------------------------------------------------------*/
// py读取配置文件
/*
#include <Python.h>
#include <iostream>
#include <string>
#include <map>
#include <nlohmann/json.hpp>  // 需要包含json库

using json = nlohmann::json;

class ConfigReader {
private:
    std::map<std::string, std::map<std::string, std::string>> config_data;

public:
    bool loadConfig(const std::string& config_file) {
        // 初始化Python解释器
        Py_Initialize();

        if (!Py_IsInitialized()) {
            std::cerr << "Python initialization failed!" << std::endl;
            return false;
        }

        // 添加当前目录到Python路径
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('.')");

        // 导入Python模块
        PyObject* pModule = PyImport_ImportModule("config_reader");
        if (!pModule) {
            std::cerr << "Failed to load Python module!" << std::endl;
            PyErr_Print();
            Py_Finalize();
            return false;
        }

        // 获取函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "read_config");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            std::cerr << "Function not found or not callable!" << std::endl;
            Py_XDECREF(pFunc);
            Py_DECREF(pModule);
            Py_Finalize();
            return false;
        }

        // 构建参数
        PyObject* pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(config_file.c_str()));

        // 调用函数
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pArgs);

        if (pValue) {
            // 将Python字符串转换为C字符串
            const char* result = PyUnicode_AsUTF8(pValue);
            if (result) {
                try {
                    json j = json::parse(result);
                    // 解析JSON数据到map中
                    for (auto& [section, values] : j.items()) {
                        std::map<std::string, std::string> section_map;
                        for (auto& [key, value] : values.items()) {
                            section_map[key] = value.get<std::string>();
                        }
                        config_data[section] = section_map;
                    }
                    Py_DECREF(pValue);
                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    Py_Finalize();
                    return true;
                } catch (const std::exception& e) {
                    std::cerr << "JSON parsing error: " << e.what() << std::endl;
                }
            }
            Py_DECREF(pValue);
        } else {
            PyErr_Print();
        }

        Py_DECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        return false;
    }

    std::string get(const std::string& section, const std::string& key,
                   const std::string& default_value = "") {
        auto section_it = config_data.find(section);
        if (section_it != config_data.end()) {
            auto key_it = section_it->second.find(key);
            if (key_it != section_it->second.end()) {
                return key_it->second;
            }
        }
        return default_value;
    }
};

int main() {
    ConfigReader reader;

    if (reader.loadConfig("config.ini")) {
        std::cout << "Database host: " << reader.get("database", "host") << std::endl;
        std::cout << "Database port: " << reader.get("database", "port") << std::endl;
        std::cout << "Log level: " << reader.get("logging", "level") << std::endl;
    } else {
        std::cerr << "Failed to load configuration!" << std::endl;
        return 1;
    }

    return 0;
}
*/

#endif
