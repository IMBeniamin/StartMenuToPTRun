#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>

enum Severity {
    DEBUG_LEVEL,
    INFO_LEVEL,
    WARNING_LEVEL,
    CRITICAL_LEVEL,
};

enum Category {
    APPLICATION,
    EVENTS,
    DEBUG,
};

// Compile-time conversion functions using constexpr
constexpr const char *severityToString(const Severity severity) {
    switch (severity) {
        case Severity::DEBUG_LEVEL: return "DEBUG";
        case Severity::INFO_LEVEL: return "INFO";
        case Severity::WARNING_LEVEL: return "WARNING";
        case Severity::CRITICAL_LEVEL: return "ERROR";
        default: return "UNKNOWN";
    }
}

constexpr const char *categoryToString(const Category category) {
    switch (category) {
        case Category::APPLICATION: return "GENERAL";
        case Category::EVENTS: return "EVENTS";
        case Category::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

class LogConfig {
public:
    static bool ENABLE_DEBUG;
    static bool ENABLE_CONSOLE;
    static bool ENABLE_FILE;
};

// LogMessage Class Definition
class LogMessage {
public:
    LogMessage(Severity severity, Category category, std::string message);

    [[nodiscard]] std::string toString() const;

private:
    Severity severity_;
    Category category_;
    std::string message_;
    std::string timestamp_;

    // Helper function to get the current date and time
    static std::string currentDateTime();
};

// Logger Class Definition
class Logger {
public:
    // Singleton instance getter
    static Logger &getInstance();

    void log(const Severity &severity, const Category &category, const std::string &message);

    // Prevent copying and assignment
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

private:
    std::ofstream logfile_;
    std::mutex mutex_;

    // Private constructor and destructor for singleton
    Logger();

    ~Logger();
};

#endif // LOGGER_H
