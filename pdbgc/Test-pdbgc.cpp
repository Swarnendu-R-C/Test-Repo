#include <iostream>
#include <string>
#include <variant>

#include <phosphor-logging/log.hpp>

#include "Test-pdbgc.hpp"

void logPEL(const std::string& dumpFilePath, const std::string& dumpFileType,
            const std::string& dumpId, const std::string& pelSev,
            const std::string& errIntf) 
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
        {dumpIdString, dumpId},
        {dumpFileString, dumpFilePath},
        {dumpFileTypeString, dumpFileType}};

    busMethod.append(errIntf, pelSev, userDataMap);

    std::cout << "Calling async method for logging PEL message" << std::endl
              << std::endl;

    while (dBus.process_discard())
      ;
    
    auto slot = busMethod.call_async([&](sdbusplus::message::message &&reply) 
    {
      if (reply.is_method_error()) 
      {
        std::cerr << "Error in calling async method to create PEL" << std::endl
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
    dBus.wait(std::chrono::seconds(1));
    dBus.process_discard();
  } 
  catch (const std::exception &e) 
  {
    std::cerr << "Error in calling creating PEL. Exception caught : "
              << e.what() << std::endl;
  }
}

auto logPEL(const std::string dumpFilePath, const std::string dumpFileType,
            const std::string dumpId, const std::string pelSev,
            const std::string errIntf, sdbusplus::async::context &ctx)
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
        {dumpIdString, dumpId},
        {dumpFileString, dumpFilePath},
        {dumpFileTypeString, dumpFileType}};

    
    std::cout << "Going for writing PEL via co-routine" << std::endl << std::endl;
    co_await systemd.call<>(ctx, "Create", errIntf, pelSev, userDataMap);
    std::cout << "Writing PEL via co-routine is done now" << std::endl << std::endl;

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

  //sdbusplus::async::context ctx;
  constexpr auto strDumpFilePath = "/var/lib/phosphor/BMC/dumps/entry/TestDump";
  constexpr auto strDumpFileType = "BMC";
  const auto     strDumpID = std::to_string(9999);
  constexpr auto strPelSev = "xyz.openbmc_project.Logging.Entry.Level.Informational";
  constexpr auto strErrIntf = "xyz.openbmc_project.Dump.Error.Invalidate";

  /* ctx.spawn(logPEL(strDumpFilePath, strDumpFileType, strDumpID, strPelSev, strErrIntf, ctx));
  ctx.run(); */

  logPEL(strDumpFilePath, strDumpFileType, strDumpID, strPelSev, strErrIntf);

  std::cout << "================<Main Program exited>=================="
            << std::endl
            << std::endl;
  return 0;
}