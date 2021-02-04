//
// Created by lamp on 04.02.2021.
//

#ifndef THREADING_INCLUDED_LIBS_HPP_
#define THREADING_INCLUDED_LIBS_HPP_

#include <boost/filesystem.hpp>
#include <future>
#include <mutex>
#include <picosha2.h>
#include <boost/log/trivial.hpp>
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
#include <any>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace po = boost::program_options;

typedef src::severity_logger<logging::trivial::severity_level> logger;

#endif  // THREADING_INCLUDED_LIBS_HPP_
