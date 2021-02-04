//
// Created by lamp on 04.02.2021.
//

#include <console.hpp>

std::mutex hasher::console::mutie;

std::vector<json> hasher::console::correct_values;

po::options_description hasher::console::desc("Allowed options");
std::string hasher::console::out_file = "jsonLog/log.json";
size_t hasher::console::threadsCount = std::thread::hardware_concurrency();

void hasher::console::wmain(int argc, char** argv) {
  desc.add_options()("help,h", "help message")(
      "output,o", po::value<std::string>(),
      "Set output .json file. Base value is <logJson/log.json>")(
      "threads,t", po::value<int>(),
      "Sets threads count. Base value is <boost::thread::hardware_concurrency>")
      ("input,i", po::value<std::string>(), "input.... SET DESCRIPTION");

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(desc)
                                  .allow_unregistered()
                                  .run();
  po::store(parsed, vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << "This program is used to generate true hash values\nfrom random data and to write them into the .json file\n"
              << desc << "\nCOPYRIGHT 2021 LAMP\n";
    exit(0);
  }

  if (vm.count("output")) {
    out_file = vm["output"].as<std::string>();
  }
  if (vm.count("threads")) {
    threadsCount = vm["threads"].as<int>();
  }
  std::cout << "Output file: " << out_file
            << "\nThreads count chosen: " << std::to_string(threadsCount)
            << "\nStarted encoding...\n";
  initiate();
}

void hasher::console::initiate() {
  logging::add_file_log(
      keywords::file_name = "logs/log_%5N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

  srand(time(nullptr));
}
