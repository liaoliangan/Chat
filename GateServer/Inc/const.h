#ifndef CONST_H
#define CONST_H

#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

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
