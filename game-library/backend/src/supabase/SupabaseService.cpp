#include "../../include/SupabaseService.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

namespace gl {

SupabaseService::SupabaseService()
    : client_(readEnvVarOrFile("SUPABASE_URL"), readEnvVarOrFile("SUPABASE_ANON_KEY"))
{
}

std::string SupabaseService::projectRootPath() {
    // Assume two levels up from backend/ is project root
    return std::string("..") + std::string("/..");
}

std::string SupabaseService::readEnvVarOrFile(const std::string& key) {
    const char* val = std::getenv(key.c_str());
    if (val && std::string(val).size() > 0) return std::string(val);

    // Try to load from .env in project root
    std::string envPath = projectRootPath() + "/.env";
    std::ifstream ifs(envPath);
    if (!ifs) {
        std::cerr << "[SupabaseService] Warning: cannot open .env at " << envPath << " and env var " << key << " not set.\n";
        return std::string();
    }

    std::string line;
    while (std::getline(ifs, line)) {
        // skip comments and empty
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string k = line.substr(0, pos);
        std::string v = line.substr(pos + 1);
        if (k == key) return v;
    }

    return std::string();
}

// ------------------- Games -------------------
std::vector<JeuRecord> SupabaseService::getAllGames() {
    try {
        auto res = client_.get("jeux");
        std::vector<JeuRecord> out;
        if (res.is_array()) {
            for (const auto& item : res) {
                out.push_back(item.get<JeuRecord>());
            }
        }
        return out;
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] getAllGames error: " << e.what() << "\n";
        throw;
    }
}

JeuRecord SupabaseService::addGame(const JeuRecord& data) {
    try {
        nlohmann::json j = data;
        auto res = client_.insert("jeux", j);
        if (res.is_array() && !res.empty()) return res.front().get<JeuRecord>();
        return res.get<JeuRecord>();
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] addGame error: " << e.what() << "\n";
        throw;
    }
}

JeuRecord SupabaseService::updateGame(int id, const JeuRecord& data) {
    try {
        nlohmann::json j = data;
        auto res = client_.update("jeux", id, j);
        if (res.is_array() && !res.empty()) return res.front().get<JeuRecord>();
        return res.get<JeuRecord>();
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] updateGame error: " << e.what() << "\n";
        throw;
    }
}

void SupabaseService::deleteGame(int id) {
    try {
        client_.remove("jeux", id);
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] deleteGame error: " << e.what() << "\n";
        throw;
    }
}

// ------------------- Members -------------------
std::vector<MembreRecord> SupabaseService::getMembers() {
    try {
        auto res = client_.get("membres");
        std::vector<MembreRecord> out;
        if (res.is_array()) {
            for (const auto& item : res) out.push_back(item.get<MembreRecord>());
        }
        return out;
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] getMembers error: " << e.what() << "\n";
        throw;
    }
}

MembreRecord SupabaseService::addMember(const MembreRecord& data) {
    try {
        nlohmann::json j = data;
        auto res = client_.insert("membres", j);
        if (res.is_array() && !res.empty()) return res.front().get<MembreRecord>();
        return res.get<MembreRecord>();
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] addMember error: " << e.what() << "\n";
        throw;
    }
}

// ------------------- Borrowing -------------------
EmpruntRecord SupabaseService::borrowGame(int idJeu, int idMembre, const std::string& dateEmprunt, const std::string& dateRetourPrevue) {
    try {
        nlohmann::json j;
        j["idJeu"] = idJeu;
        j["idMembre"] = idMembre;
        j["dateEmprunt"] = dateEmprunt;
        j["dateRetourPrevue"] = dateRetourPrevue;
        auto res = client_.insert("emprunts", j);
        if (res.is_array() && !res.empty()) return res.front().get<EmpruntRecord>();
        return res.get<EmpruntRecord>();
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] borrowGame error: " << e.what() << "\n";
        throw;
    }
}

EmpruntRecord SupabaseService::returnGame(int empruntId) {
    try {
        nlohmann::json j;
        j["retourne"] = true;
        auto res = client_.update("emprunts", empruntId, j);
        if (res.is_array() && !res.empty()) return res.front().get<EmpruntRecord>();
        return res.get<EmpruntRecord>();
    } catch (const std::exception& e) {
        std::cerr << "[SupabaseService] returnGame error: " << e.what() << "\n";
        throw;
    }
}

} // namespace gl
