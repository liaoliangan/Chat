#include "ConfigMgr.h"
#include <filesystem>
#include <Python.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
SectionInfo::SectionInfo() = default;

SectionInfo::~SectionInfo()
{
    _section_datas.clear();
}

SectionInfo::SectionInfo(const SectionInfo &sectionInfo)
{
    _section_datas = sectionInfo._section_datas;
}

SectionInfo &SectionInfo::operator=(const SectionInfo &sectionInfo)
{
    if (this == &sectionInfo)
    {
        return *this;
    }
    _section_datas = sectionInfo._section_datas;
    return *this;
}

std::string SectionInfo::operator[](const std::string &key)
{
    if (_section_datas.find(key) == _section_datas.end())
    {
        return "";
    }
    return _section_datas[key];
}

ConfigMgr::ConfigMgr()
{
    // std::filesystem::path current_path = std::filesystem::current_path();
    // std::filesystem::path config_path = current_path / "../cfg/config.ini";
    // std::cout << "Config path: " << config_path << std::endl;

    // boost::property_tree::ptree pt;
    // boost::property_tree::read_ini(config_path.string(), pt);

    // for (const auto &section_pair : pt)
    // {
    //     const std::string &section_name = section_pair.first;
    //     const boost::property_tree::ptree &section_tree = section_pair.second;

    //     std::map<std::string, std::string> section_config;
    //     for (const auto &key_value_pair : section_tree)
    //     {
    //         const std::string &key = key_value_pair.first;
    //         const std::string &value = key_value_pair.second.get_value<std::string>();
    //         section_config[key] = value;
    //     }
    //     SectionInfo sectionInfo;
    //     sectionInfo._section_datas = section_config;

    //     _config_map[section_name] = sectionInfo;
    // }

    // // 输出所有的section和key-value对
    // for (const auto &section_entry : _config_map)
    // {
    //     const std::string &section_name = section_entry.first;
    //     SectionInfo section_config = section_entry.second;
    //     std::cout << "[" << section_name << "]" << std::endl;
    //     for (const auto &key_value_pair : section_config._section_datas)
    //     {
    //         std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
    //     }
    // }

    // 1. 初始化 Python 解释器
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        throw std::runtime_error("Python init failed");
    }

    std::filesystem::path exeDir = std::filesystem::canonical("/proc/self/exe").parent_path(); // build/
    std::filesystem::path scriptDir = exeDir.parent_path() / "script";            // project/script/

    // 2. 把 script 目录加入 sys.path
    PyRun_SimpleString("import sys");
    std::string addPath = "sys.path.insert(0, r'" + scriptDir.string() + "')";
    PyRun_SimpleString(addPath.c_str());

    // 2. 导入自定义模块
    PyObject *pModule = PyImport_ImportModule("config_reader");
    if (!pModule)
    {
        PyErr_Print();
        Py_Finalize();
        throw std::runtime_error("cannot import config_reader");
    }

    // 3. 调用 read_ini(path)
    PyObject *pFunc = PyObject_GetAttrString(pModule, "read_ini");
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        throw std::runtime_error("read_ini not found");
    }

    std::string iniPath = "../cfg/config.ini";
    PyObject *pArgs = PyTuple_Pack(1, PyUnicode_FromString(iniPath.c_str()));
    PyObject *pRet = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    if (!pRet)
    {
        PyErr_Print();
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        throw std::runtime_error("python call failed");
    }

    // 4. 取 json 字符串
    const char *jsonStr = PyUnicode_AsUTF8(pRet);
    if (!jsonStr)
    {
        Py_DECREF(pRet);
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        throw std::runtime_error("return not string");
    }

    // 5. 解析 json → map
    auto j = json::parse(jsonStr);
    for (auto &[sec, kv] : j.items())
    {
        SectionInfo si;
        for (auto &[k, v] : kv.items())
            si._section_datas[k] = v.get<std::string>();
        _config_map[sec] = si;
    }

    // 6. 清理 Python
    Py_DECREF(pRet);
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();

    // 7. 打印（同原逻辑）
    for (auto &[sec, si] : _config_map)
    {
        std::cout << "[" << sec << "]\n";
        for (auto &[k, v] : si._section_datas)
            std::cout << k << "=" << v << "\n";
    }
}

ConfigMgr::~ConfigMgr()
{
    _config_map.clear();
}

ConfigMgr::ConfigMgr(const ConfigMgr &src)
{
    _config_map = src._config_map;
}

ConfigMgr &ConfigMgr::operator=(const ConfigMgr &src)
{
    if (this == &src)
        return *this;
    _config_map = src._config_map;
    return *this;
}

SectionInfo ConfigMgr::operator[](const std::string &section)
{
    if (_config_map.find(section) == _config_map.end())
    {
        return SectionInfo();
    }
    return _config_map[section];
}
