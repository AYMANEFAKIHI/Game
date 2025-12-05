#pragma once

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include "Types.h"
#include "SupabaseClient.h"

namespace gl {

class SupabaseService {
public:
    SupabaseService();

    // Games
    std::vector<JeuRecord> getAllGames();
    JeuRecord addGame(const JeuRecord& data);
    JeuRecord updateGame(int id, const JeuRecord& data);
    void deleteGame(int id);

    // Members
    std::vector<MembreRecord> getMembers();
    MembreRecord addMember(const MembreRecord& data);

    // Borrowing
    EmpruntRecord borrowGame(int idJeu, int idMembre, const std::string& dateEmprunt, const std::string& dateRetourPrevue);
    EmpruntRecord returnGame(int empruntId);

private:
    SupabaseClient client_;

    static std::string readEnvVarOrFile(const std::string& key);
    static std::string projectRootPath();
};

} // namespace gl
