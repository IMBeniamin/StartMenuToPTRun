#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <utility>

bool LogConfig::ENABLE_DEBUG = false;
bool LogConfig::ENABLE_CONSOLE = false;
bool LogConfig::ENABLE_FILE = false;

LogMessage::LogMessage(const Severity severity, const Category category, std::string message)
    : severity_(severity), category_(category), message_(std::move(message)), timestamp_(currentDateTime()) {
}

std::string LogMessage::toString() const {
    return timestamp_ + " | " + severityToString(severity_) + " | " + categoryToString(category_) + " | " + message_;
}

// Helper function to get the current date and time
std::string LogMessage::currentDateTime() {
    // Get current time point and extract milliseconds
    const auto now = std::chrono::system_clock::now();
    const auto now_ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    const auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // Format the time into HH:mm:ss
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time_t), "%H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

    return oss.str();
}

// Logger Class Implementation
Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    logfile_ = std::ofstream("log_" + std::to_string(now) + ".log", std::ios::out | std::ios::app);
    if (!logfile_) {
        std::cerr << "Unable to open log file!" << std::endl;
    }
}

Logger::~Logger() {
    if (logfile_.is_open()) {
        logfile_.close();
    }
}

void Logger::log(const Severity &severity, const Category &category, const std::string &message) {
    if (!LogConfig::ENABLE_DEBUG && severity == DEBUG_LEVEL) {
        return;
    }
    const LogMessage logMessage(severity, category, message);

    if (LogConfig::ENABLE_FILE) {
        logfile_ << logMessage.toString() << std::endl;
    }
    if (LogConfig::ENABLE_CONSOLE) {
        std::cout << logMessage.toString() << std::endl;
    }
}
