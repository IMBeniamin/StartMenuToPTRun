#include "Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <utility>
#include <shlobj.h>

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

std::string Logger::getLogFilePath() {
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    PWSTR ppszPath = nullptr; // Declare a PWSTR for the output

    const auto pathCallResult = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &ppszPath);

    if (SUCCEEDED(pathCallResult)) {
        std::wstring appdataPathW(ppszPath);
        const std::string appdataPath(appdataPathW.begin(), appdataPathW.end());
        CoTaskMemFree(ppszPath);
        return appdataPath + "\\KeyboardHook\\log_" + std::to_string(now) + ".log";
    }
    log(CRITICAL_LEVEL, Category::APPLICATION, "Unable to get log file path!");
    LogConfig::ENABLE_FILE = false;
    return "";
}

bool Logger::setLogFileStream(const std::string &logFilePath) {
    logfile_ = std::ofstream(logFilePath);
    if (logfile_.is_open()) {
        log(DEBUG_LEVEL, Category::APPLICATION, "File Logger initialized");
        return true;
    }
    log(CRITICAL_LEVEL, Category::APPLICATION, "Unable to open log stream!");
    return false;
}


Logger::Logger() {
    if (!LogConfig::ENABLE_FILE && !LogConfig::ENABLE_CONSOLE) {
        std::cerr << "No output enabled for logger!" << std::endl;
    } else if (LogConfig::ENABLE_FILE) {
        const auto logFilePath = getLogFilePath();
        if (logFilePath.empty()) {
            return;
        }
        if (auto setLogFileResult = setLogFileStream(logFilePath))
            return;

        // Failed to open the log file, try creating the folder
        const auto folderPath = logFilePath.substr(0, logFilePath.find_last_of('\\'));
        if (CreateDirectory(folderPath.c_str(), nullptr)) {
            log(DEBUG_LEVEL, Category::APPLICATION, "Log folder created");
            setLogFileStream(logFilePath);
        } else {
            log(CRITICAL_LEVEL, Category::APPLICATION, "Unable to create log folder!");
            LogConfig::ENABLE_FILE = false;
        }
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
