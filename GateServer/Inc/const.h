#ifndef CONST_H
#define CONST_H

#include <queue>
#include <memory>
#include <atomic>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <condition_variable>
#include <boost/beast/http.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace LA
{
    enum class ErrorCodes
    {
        SUCCESS = 0,
        ERROR_JSON = 1001,
        RPCFAILED = 1002,
    };
};
#endif
