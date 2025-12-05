#pragma once

#include <string>
#include <nlohmann/json.hpp>

class SupabaseClient {
public:
    SupabaseClient(const std::string& url, const std::string& key);
    nlohmann::json get(const std::string& table);
    nlohmann::json insert(const std::string& table, const nlohmann::json& data);
    nlohmann::json update(const std::string& table, int id, const nlohmann::json& data);
    nlohmann::json remove(const std::string& table, int id);

private:
    std::string baseUrl_;
    std::string apiKey_;
    std::string authHeader_;

    std::string buildUrl(const std::string& table, int id = -1) const;
    nlohmann::json performRequest(const std::string& method, const std::string& url, const nlohmann::json* body = nullptr) const;
};
