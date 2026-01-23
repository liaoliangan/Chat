#include "ConfigMgr.h"
#include <filesystem>
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
std::string SectionInfo::GetValue(const std::string &key)
{
    if (_section_datas.find(key) == _section_datas.end())
    {
        return "";
    }
    // 这里可以添加一些边界检查
    return _section_datas[key];
}

ConfigMgr::ConfigMgr()
{
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path config_path = current_path / "../cfg/config.ini";
    std::cout << "Config path: " << config_path << std::endl;

    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(config_path.string(), pt);

    for (const auto &section_pair : pt)
    {
        const std::string &section_name = section_pair.first;
        const boost::property_tree::ptree &section_tree = section_pair.second;

        std::map<std::string, std::string> section_config;
        for (const auto &key_value_pair : section_tree)
        {
            const std::string &key = key_value_pair.first;
            const std::string &value = key_value_pair.second.get_value<std::string>();
            section_config[key] = value;
        }
        SectionInfo sectionInfo;
        sectionInfo._section_datas = section_config;

        _config_map[section_name] = sectionInfo;
    }

    // 输出所有的section和key-value对
    for (const auto &section_entry : _config_map)
    {
        const std::string &section_name = section_entry.first;
        SectionInfo section_config = section_entry.second;
        std::cout << "[" << section_name << "]" << std::endl;
        for (const auto &key_value_pair : section_config._section_datas)
        {
            std::cout << key_value_pair.first << "=" << key_value_pair.second << std::endl;
        }
    }
}

ConfigMgr::~ConfigMgr()
{
    _config_map.clear();
}

SectionInfo ConfigMgr::operator[](const std::string &section)
{
    if (_config_map.find(section) == _config_map.end())
    {
        return SectionInfo();
    }
    return _config_map[section];
}

ConfigMgr &ConfigMgr::GetInstance()
{
    static ConfigMgr cfg_mgr;
    return cfg_mgr;
}
std::string ConfigMgr::GetValue(const std::string &section, const std::string &key)
{
    if (_config_map.find(section) == _config_map.end())
    {
        return "";
    }

    return _config_map[section].GetValue(key);
}
