#include <coroutine>
#include <future>
#include <iostream>
#include <string>
#include <variant>

#include <sdbusplus/async.hpp>
#include <sdbusplus/bus.hpp>

#include <phosphor-logging/log.hpp>

void logPEL(const std::string &dumpFilePath, const std::string &dumpFileType,
            const int dumpId, const std::string &pelSev,
            const std::string &errIntf) 
{
  try 
  {
    constexpr auto loggerObjectPath = "/xyz/openbmc_project/logging";
    constexpr auto loggerCreateInterface = "xyz.openbmc_project.Logging.Create";
    constexpr auto loggerService = "xyz.openbmc_project.Logging";
    constexpr auto dumpFileString = "File Name";
    constexpr auto dumpFileTypeString = "Dump Type";
    constexpr auto dumpIdString = "Dump ID";

    // Set up a connection to D-Bus object
    auto dBus = sdbusplus::bus::new_default();
    auto busMethod = dBus.new_method_call(loggerService, loggerObjectPath,
                                          loggerCreateInterface, "Create");

    const std::unordered_map<std::string_view, std::string_view> userDataMap = {
        {dumpIdString, std::to_string(dumpId)},
        {dumpFileString, dumpFilePath},
        {dumpFileTypeString, dumpFileType}};

    busMethod.append(errIntf, pelSev, userDataMap);

    std::cout << "Calling async method for logging PEL message" << std::endl
              << std::endl;
    auto slot = busMethod.call_async([&](sdbusplus::message::message &&reply) 
    {
      if (reply.is_method_error()) 
      {
        std::cout << "Error in calling async method to create PEL" << std::endl
                  << std::endl;
      }
      else
      {
        std::cout << "Success calling async method to create PEL" << std::endl
                  << std::endl;
      }
    });

    if (!slot) 
    {
      std::cout << "Slot contains null pointer" << std::endl;
    }
  } 
  catch (const std::exception &e) 
  {
    std::cerr << "Error in calling creating PEL. Exception caught : "
              << e.what() << std::endl;
  }
}

auto logPEL(const std::string &dumpFilePath, const std::string &dumpFileType,
            const int dumpId, const std::string &pelSev,
            const std::string &errIntf, sdbusplus::async::context &ctx)
    -> sdbusplus::async::task<> 
{
  try 
  {
    constexpr auto loggerObjectPath = "/xyz/openbmc_project/logging";
    constexpr auto loggerCreateInterface = "xyz.openbmc_project.Logging.Create";
    constexpr auto loggerService = "xyz.openbmc_project.Logging";
    constexpr auto dumpFileString = "File Name";
    constexpr auto dumpFileTypeString = "Dump Type";
    constexpr auto dumpIdString = "Dump ID";

    const auto systemd = sdbusplus::async::proxy()
                             .service(loggerService)
                             .path(loggerObjectPath)
                             .interface(loggerCreateInterface)
                             .preserve();

    const std::unordered_map<std::string_view, std::string_view> userDataMap = {
        {dumpIdString, std::to_string(dumpId)},
        {dumpFileString, dumpFilePath},
        {dumpFileTypeString, dumpFileType}};

    co_await systemd.call<>(ctx, "Create", errIntf, pelSev, userDataMap);

    // We are all done, so shutdown the server.
    ctx.request_stop();
    co_return;
  } 
  catch (const std::exception &e)
  {
    std::cerr << "Error in calling creating PEL. Standard exception caught : "
              << e.what() << std::endl;
  }
}

int main(int argc, char **argv) 
{
  std::cout << std::endl
            << "================<Main Program started>=================="
            << std::endl
            << std::endl;

  if (argc < 2)
    std::cout << "No argument passed with the program " << argv[0] << std::endl
              << std::endl;

  sdbusplus::async::context ctx;
  ctx.spawn(logPEL("/var/lib/phosphor/BMC/dumps/entry/TestDump", "BMC", 9999,
                    "xyz.openbmc_project.Logging.Entry.Level.Informational",
                    "xyz.openbmc_project.Dump.Error.Invalidate", ctx));
  ctx.run();

  /* logPEL("/var/lib/phosphor/BMC/dumps/entry/TestDump", "BMC", 9999,
         "xyz.openbmc_project.Logging.Entry.Level.Informational",
         "xyz.openbmc_project.Dump.Error.Invalidate"); */

  std::cout << "================<Main Program exited>=================="
            << std::endl
            << std::endl;
  return 0;
}