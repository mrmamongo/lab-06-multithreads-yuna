// Copyright 2020 Your Name <your_email>
#include <Hasher.hpp>

std::mutex mutie;

[[noreturn]] void encode(
    std::vector<json>& CorrectValues, std::chrono::time_point<std::chrono::system_clock>& start
    ) {
  src::severity_logger<logging::trivial::severity_level> lg;
  while (true) {
    const std::string hashEnd = "0000";
    std::string randomString = std::to_string(rand());
    std::string result = picosha2::hash256_hex_string(randomString);

    mutie.lock();
    auto end = std::chrono::system_clock::now();
    auto timestamp = (std::chrono::nanoseconds (end - start).count());
    start = end;
    mutie.unlock();

    if (result.substr(result.length() - hashEnd.length()) == hashEnd) {
      BOOST_LOG_SEV(lg, logging::trivial::info)
          << "\nCorrect value: '" << randomString << "'" << std::endl
          << "With hash: '" << result << "'\n";
      json j = {
          {"timestamp", timestamp},
          {"hash", result},
          {"data", randomString}
      };
      CorrectValues.emplace_back(j);
    } else {
      BOOST_LOG_SEV(lg, logging::trivial::trace)
          << "\nValue to encode '" << randomString << "'" << std::endl
          << "With hash: '" << result << "'\n";
    }
  }
}
