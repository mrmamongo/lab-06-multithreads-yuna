// Copyright 2020 Your Name <your_email>
#include <Hasher.hpp>


std::mutex mutie;
std::vector<json> correctValues;

void Hasher::sigHandler(int signum) {
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
  for (auto& correctValue : correctValues) {
    std::cout << correctValue << '\n';
  }

  std::cout << "\nprogram executed with code " << signum << "\n";
  exit(signum);
}

void Hasher::startHashing() {
  logging::add_common_attributes();

  src::severity_logger<logging::trivial::severity_level> lg;

  size_t threadsCount = boost::thread::hardware_concurrency();

  std::chrono::time_point start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < threadsCount; ++i) {
    auto futures =
        std::async(std::launch::async, [&start] { Hasher::Hasher::encode(start); });
  }
}

void Hasher::initiate() {
  logging::add_file_log(
      keywords::file_name = "logs/log_%5N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

  srand(time(nullptr));
}

[[noreturn]] void Hasher::encode(
    std::chrono::time_point<std::chrono::system_clock>& start) {
  src::severity_logger<logging::trivial::severity_level> lg;
  while (true) {
    const std::string hashEnd = "0000";
    std::string randomString = std::to_string(rand());
    std::string result = picosha2::hash256_hex_string(randomString);

    mutie.lock();
    auto end = std::chrono::system_clock::now();
    auto timestamp = (std::chrono::nanoseconds(end - start).count());
    start = end;
    mutie.unlock();

    if (result.substr(result.length() - hashEnd.length()) == hashEnd) {
      BOOST_LOG_SEV(lg, logging::trivial::info)
          << "\nCorrect value: '" << randomString << "'" << std::endl
          << "With hash: '" << result << "'\n";
      json j = {
          {"timestamp", timestamp}, {"hash", result}, {"data", randomString}};
      correctValues.emplace_back(j);
    } else {
      BOOST_LOG_SEV(lg, logging::trivial::trace)
          << "\nValue to encode '" << randomString << "'" << std::endl
          << "With hash: '" << result << "'\n";
    }
  }
}
