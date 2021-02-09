// Copyright 2020 lamp
#include "Hasher.hpp"


void initiate() {
  logging::add_file_log(
      keywords::file_name = "logs/log_%5N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%");

  srand(time(nullptr));
}

po::options_description Console::desc("Allowed options");

int main(int argc, char** argv) {
  Console console;
  initiate();
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(console.getDesc())
      .allow_unregistered()
      .run();
  po::store(parsed, vm);
  po::notify(vm);


  Console::wmain(vm);
}
