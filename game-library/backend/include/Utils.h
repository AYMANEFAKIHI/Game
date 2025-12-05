#pragma once

#include <ctime>
#include <sstream>
#include <iomanip>

// Common include file for time utilities

namespace GameLibrary {
    namespace Utils {
        /**
         * Get current date in YYYY-MM-DD format
         */
        inline std::string getCurrentDate() {
            auto now = std::time(nullptr);
            auto tm = *std::localtime(&now);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d");
            return oss.str();
        }

        /**
         * Format date string
         */
        inline std::string formatDate(std::time_t timestamp) {
            auto tm = *std::localtime(&timestamp);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d");
            return oss.str();
        }

        /**
         * Calculate days between two dates (YYYY-MM-DD format)
         */
        inline int daysBetween(const std::string& date1, const std::string& date2) {
            // Simple implementation - compare date strings
            return (date2 > date1) ? 1 : (date2 < date1) ? -1 : 0;
        }
    }
}
