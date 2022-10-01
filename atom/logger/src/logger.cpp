
#include <atom/logger/logger.hpp>
#include <unordered_map>

namespace atom {

  Logger& GetLogger() {
    static Logger logger{"atom"};

    return logger;
  }

  Logger& GetNamedLogger(std::string_view name) {
    static std::unordered_map<std::string_view, Logger> registry;

    if (!registry.contains(name)) {
      registry[name] = Logger{GetLogger().GetSinkCollection(), name};
    }

    return registry[name];
  }

} // namespace atom