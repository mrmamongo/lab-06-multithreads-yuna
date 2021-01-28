// Copyright 2020 lamp

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <mutex>
#include <boost/log/trivial.hpp>
#include "../third-party/PicoSHA2/picosha2.h"
#include <csignal>
#include <future>
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace po = boost::program_options;

typedef src::severity_logger<logging::trivial::severity_level> logger;

class Console {
  static po::options_description desc;
  static void initiate();
  static std::string fileName;
  static size_t threadsCount;
 public:
  Console(int, char**);
  class Hasher {
   private:
    typedef std::vector<json> values;
    static values correctValues;
    static std::mutex mutie;

    [[noreturn]] static void encode(
        std::chrono::time_point<std::chrono::system_clock>& start);
   public:

    static void startHashing();
    static void sigHandler(int signum);
  };
};

#endif // INCLUDE_HEADER_HPP_
