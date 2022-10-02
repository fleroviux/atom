
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
      auto sink_collection = std::make_shared<Logger::SinkCollection>();
      sink_collection->Install(GetLogger().GetSinkCollection());
      registry[name] = Logger{sink_collection, name};
    }

    return registry[name];
  }

} // namespace atom