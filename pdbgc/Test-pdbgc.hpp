#include <iostream>
#include <string>
#include <coroutine>
#include <exception>

#include <sdbusplus/async.hpp>
#include <sdbusplus/bus.hpp>

void logPEL(const std::string& dumpFilePath, const std::string& dumpFileType,
            const std::string& dumpId, const std::string& pelSev,
            const std::string& errIntf);

auto logPEL(const std::string dumpFilePath, const std::string dumpFileType,
            const std::string dumpId, const std::string pelSev,
            const std::string errIntf, sdbusplus::async::context &ctx)
    -> sdbusplus::async::task<>;

