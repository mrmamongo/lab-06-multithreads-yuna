// Copyright 2020 Your Name <your_email>
#include <Hasher.hpp>

std::mutex Console::Hasher::mutie;

std::vector<json> Console::Hasher::correctValues;

std::string Console::fileName;
size_t Console::threadsCount;

Console::Console() {
  desc.add_options()("help,h", "help message")(
      "output,o", po::value<std::string>(), "Set output .json file. Base value is <logJson/log.json>")(
      "threads,t", po::value<int>(),
      "Sets threads count. Base value is <boost::thread::hardware_concurrency>")
      ("times,t", po::value<int>(), "<NOT WORKING> Sets number of true values, if you want to stop at a specific moment");

}


int Console::wmain(const po::variables_map& vm) {
  if (vm.count("help")) {
    std::cout << "This program is used to generate true hash values\nfrom random data and to write them into the .json file\n"
              << desc << "\nCOPYRIGHT 2021 LAMP\n";
    exit(0);
  }
  fileName = "jsonLog/log.json";
  threadsCount = std::thread::hardware_concurrency();
  if (vm.count("output")) {
    fileName = vm["output"].as<std::string>();
  }
  if (vm.count("threads")) {
    threadsCount = vm["threads"].as<int>();
  }

  std::cout << "Output file: " << fileName
            << "\nThreads count chosen: " << std::to_string(threadsCount)
            << "\nStarted encoding...\n";

  signal(SIGINT, Console::Hasher::sigHandler);
  Console::Hasher::startHashing();
  return 0;
}

void Console::Hasher::sigHandler(int signum) {
  json output;

  const std::string fieldName = "Encoding stopped\nCorrect Values";
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
  outFile << output;
  outFile.close();

  std::cout << "\nRight values:\n";
  for (auto& correctValue : correctValues) {
    std::cout << correctValue << '\n';
  }

  std::cout << "\nProgram executed with code " << signum << "\n";
  exit(signum);
}

void Console::Hasher::startHashing() {
  logging::add_common_attributes();

  std::chrono::time_point start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < threadsCount; ++i) {
    auto futures =
        std::async(std::launch::async, [&start] { Hasher::Hasher::encode(start); });
  }
}


[[noreturn]] void Console::Hasher::encode(
    std::chrono::time_point<std::chrono::system_clock>& start) {
  logger lg;
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
