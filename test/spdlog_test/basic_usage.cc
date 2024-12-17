#include "spdlog/spdlog.h"
#include <spdlog/common.h>

int main() {
  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numers like {:08d}", 12);
  spdlog::info("{1} is {0}", "hh", "handsome");

  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("This message should be displayed..");

  spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
  return 0;
}