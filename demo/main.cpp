// Copyright 2020 lamp
#include <iostream>
#include "Hasher.hpp"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;


void init() {

  logging::add_file_log(
      keywords::file_name = "logs/log_%5N.log",
      keywords::rotation_size = 10*1024*1024,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%"
      );
  //sink->set_filter(logging::trivial::severity >= logging::trivial::info);

  srand(time(nullptr));
}

std::vector<json> correctValues;

void signalHandler( int signum ) {
  json output;
  const std::string fileName = "jsonLog/log.json";
  const std::string fieldName = "Correct Values";
  std::ifstream fileIn(fileName);

  if (fileIn) {
    fileIn >> output;
    std::cout << "Previous values:\n" << output.dump() << "\n";
  }
  fileIn.close();
  std::remove(fileName.c_str());

  if (correctValues.empty()) {
    std::cout << "There are no correct values! Other ones will be written to logs dir\n";
  } else {
    for (auto&& value : correctValues) {
      output[fieldName].emplace_back(value);
    }
  }

  std::ofstream outFile(fileName);

  if (!outFile) {
    std::cout << "Json Log file not found, creating.....\n";
    outFile.open(fileName, std::ios::out);
  }
  outFile << output;
  outFile.close();

  std::cout << "\nRight values:\n";
  for (auto & correctValue : correctValues) {
    std::cout << correctValue << '\n';
  }

  std::cout << "\nprogram executed with code " << signum << "\n";
  exit(signum);
}

int main() {
  signal(SIGINT, signalHandler);
  init();
  logging::add_common_attributes();

  src::severity_logger<logging::trivial::severity_level> lg;

  size_t threadsCount = boost::thread::hardware_concurrency();

  std::chrono::time_point start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < threadsCount; ++i) {
    auto futures =
        std::async(std::launch::async, [&start]{
          encode(correctValues, start);
        });
  }

  return 0;
}