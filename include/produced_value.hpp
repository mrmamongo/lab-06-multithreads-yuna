//
// Created by lamp on 04.02.2021.
//

#ifndef THREADING_PRODUCED_VALUE_HPP_
#define THREADING_PRODUCED_VALUE_HPP_

#include "included_libs.hpp"
#include <console.hpp>
const std::string hash_end = "0000";

namespace hasher {
class produced_value {
 private:
  json _value;
  std::string _timestamp;
  std::string _hash;
  inline static std::string to_string(const json& value) {
    if(value.is_string()) {
      return value.get<std::string>();
    } else if (value.is_number_integer()) {
      return std::to_string(value.get<int>());
    } else if (value.is_number_float()) {
      return std::to_string(value.get<float>());
    } else {
      return "INCORRECT_VALUE_TYPE";
    }
  }
 public:
  inline static const json to_json(const produced_value& value){
    return json {
        {"value",value._value},
        {"timestamp", value._timestamp},
        {"hash", value._hash}
    };
  };
  inline static const json to_json(const json& value, const std::string& timestamp, const std::string& hash){
    return json {
        {"value", value},
        {"timestamp", timestamp},
        {"hash", hash}
    };
  }

  produced_value(const json& value, const std::string& timestamp, const std::string& hash)
      :_value(value), _timestamp(timestamp), _hash(hash){}


  /*
   * receives value and produces correct sha256 key for it
   */
  static produced_value encode(
      const json& value, const std::chrono::time_point<std::chrono::system_clock>& start
  ){
    std::string hash = "FILLER", timestamp;
    for (size_t nonce = 0; hash.substr(hash.length() - hash_end.length()) != hash_end; ++nonce) {
      hash = picosha2::hash256_hex_string(std::string(to_string(value) + std::to_string(nonce)));
    }
    timestamp = std::to_string(std::chrono::nanoseconds(start - std::chrono::system_clock::now()).count());
    return produced_value(value, timestamp, hash);
  }

  /*
   * generates values and produces sha256 key for it continuously
   */
  [[noreturn]] static void encode(std::chrono::time_point<std::chrono::system_clock>& start) {

    logger lg;

    while (true) {
      std::string random_string = std::to_string(rand());
      std::string result = picosha2::hash256_hex_string(random_string);

      //TODO: enable mutex
      //mutie.lock();
      auto end = std::chrono::system_clock::now();
      auto timestamp = (std::chrono::nanoseconds(end - start).count());
      start = end;
      //mutie.unlock();

      if (result.substr(result.length() - hash_end.length()) == hash_end) {
        BOOST_LOG_SEV(lg, logging::trivial::info)
          << "\nCorrect value: '" << random_string << "'" << std::endl
          << "With hash: '" << result << "'\n";
        console::correct_values.emplace_back(to_json(random_string, std::to_string(timestamp), result));
      } else {
        BOOST_LOG_SEV(lg, logging::trivial::trace)
          << "\nValue to encode '" << random_string << "'" << std::endl
          << "With hash: '" << result << "'\n";
      }
    }
  }
};
}


#endif  // THREADING_PRODUCED_VALUE_HPP_
