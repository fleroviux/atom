
#include <atom/logger/logger.hh>
#include <unordered_map>

namespace atom {

  Logger& get_logger() {
    static Logger logger{"atom"};

    return logger;
  }

  Logger& get_named_logger(std::string const& name) {
    static std::unordered_map<std::string, Logger> registry;

    if (!registry.contains(name)) {
      auto sink_collection = std::make_shared<Logger::SinkCollection>();
      sink_collection->Install(get_logger().GetSinkCollection());
      registry[name] = Logger{sink_collection, name};
    }

    return registry[name];
  }

} // namespace atom
