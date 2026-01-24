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
    std::string GetValue(const std::string & key);
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

#endif
