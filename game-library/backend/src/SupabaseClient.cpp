#include "../include/SupabaseClient.h"
#include <httplib.h>
#include <iostream>

SupabaseClient::SupabaseClient(const std::string& url, const std::string& key)
    : baseUrl_(url), apiKey_(key), authHeader_("Bearer " + key)
{
}

std::string SupabaseClient::buildUrl(const std::string& table, int id) const {
    if (id >= 0) {
        return baseUrl_ + "/rest/v1/" + table + "?id=eq." + std::to_string(id);
    }
    return baseUrl_ + "/rest/v1/" + table;
}

nlohmann::json SupabaseClient::performRequest(const std::string& method, const std::string& url, const nlohmann::json* body) const {
    httplib::Client cli(baseUrl_.c_str());

    httplib::Headers headers = {
        {"apikey", apiKey_},
        {"Authorization", authHeader_},
        {"Content-Type", "application/json"},
        {"Accept", "application/json"}
    };

    std::string path = url.substr(baseUrl_.size());

    auto logRequest = [&](const std::string& reqBody){
        std::cerr << "[SupabaseClient] " << method << " " << path << "\n";
        if (!reqBody.empty()) std::cerr << "Request body: " << reqBody << "\n";
    };

    if (method == "GET") {
        auto res = cli.Get(path.c_str(), headers);
        if (!res) throw std::runtime_error("HTTP request failed");
        return nlohmann::json::parse(res->body);
    } else if (method == "POST") {
        std::string b = body ? body->dump() : std::string();
        logRequest(b);
        auto res = cli.Post(path.c_str(), headers, b, "application/json");
        if (!res) throw std::runtime_error("HTTP request failed");
        return nlohmann::json::parse(res->body);
    } else if (method == "PATCH" || method == "PUT") {
        std::string b = body ? body->dump() : std::string();
        logRequest(b);
        auto res = cli.Patch(path.c_str(), headers, b, "application/json");
        if (!res) throw std::runtime_error("HTTP request failed");
        return nlohmann::json::parse(res->body);
    } else if (method == "DELETE") {
        auto res = cli.Delete(path.c_str(), headers);
        if (!res) throw std::runtime_error("HTTP request failed");
        return nlohmann::json::parse(res->body);
    }

    throw std::runtime_error("Unsupported HTTP method");
}

nlohmann::json SupabaseClient::get(const std::string& table) {
    auto url = buildUrl(table);
    return performRequest("GET", url, nullptr);
}

nlohmann::json SupabaseClient::insert(const std::string& table, const nlohmann::json& data) {
    auto url = buildUrl(table);
    return performRequest("POST", url, &data);
}

nlohmann::json SupabaseClient::update(const std::string& table, int id, const nlohmann::json& data) {
    // Supabase supports PATCH with filter
    std::string url = baseUrl_ + "/rest/v1/" + table + "?id=eq." + std::to_string(id);
    return performRequest("PATCH", url, &data);
}

nlohmann::json SupabaseClient::remove(const std::string& table, int id) {
    std::string url = baseUrl_ + "/rest/v1/" + table + "?id=eq." + std::to_string(id);
    return performRequest("DELETE", url, nullptr);
}
