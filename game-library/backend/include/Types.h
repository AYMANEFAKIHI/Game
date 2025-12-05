#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace gl {

struct JeuRecord {
    int id = 0;
    std::string titre;
    std::string categorie;
    bool disponible = true;
    std::string description;
    std::string auteur;
    int anneePublication = 0;
};

struct MembreRecord {
    int id = 0;
    std::string nom;
    std::string email;
    std::string dateInscription;
};

struct EmpruntRecord {
    int id = 0;
    int idJeu = 0;
    int idMembre = 0;
    std::string dateEmprunt;
    std::string dateRetourPrevue;
    bool retourne = false;
};

// nlohmann::json conversions
inline void to_json(nlohmann::json& j, const JeuRecord& r) {
    j = nlohmann::json{
        {"id", r.id},
        {"titre", r.titre},
        {"categorie", r.categorie},
        {"disponible", r.disponible},
        {"description", r.description},
        {"auteur", r.auteur},
        {"anneePublication", r.anneePublication}
    };
}

inline void from_json(const nlohmann::json& j, JeuRecord& r) {
    if (j.contains("id")) r.id = j["id"].get<int>();
    if (j.contains("titre")) r.titre = j["titre"].get<std::string>();
    if (j.contains("categorie")) r.categorie = j["categorie"].get<std::string>();
    if (j.contains("disponible")) r.disponible = j["disponible"].get<bool>();
    if (j.contains("description")) r.description = j["description"].get<std::string>();
    if (j.contains("auteur")) r.auteur = j["auteur"].get<std::string>();
    if (j.contains("anneePublication")) r.anneePublication = j["anneePublication"].get<int>();
}

inline void to_json(nlohmann::json& j, const MembreRecord& r) {
    j = nlohmann::json{
        {"id", r.id},
        {"nom", r.nom},
        {"email", r.email},
        {"dateInscription", r.dateInscription}
    };
}

inline void from_json(const nlohmann::json& j, MembreRecord& r) {
    if (j.contains("id")) r.id = j["id"].get<int>();
    if (j.contains("nom")) r.nom = j["nom"].get<std::string>();
    if (j.contains("email")) r.email = j["email"].get<std::string>();
    if (j.contains("dateInscription")) r.dateInscription = j["dateInscription"].get<std::string>();
}

inline void to_json(nlohmann::json& j, const EmpruntRecord& r) {
    j = nlohmann::json{
        {"id", r.id},
        {"idJeu", r.idJeu},
        {"idMembre", r.idMembre},
        {"dateEmprunt", r.dateEmprunt},
        {"dateRetourPrevue", r.dateRetourPrevue},
        {"retourne", r.retourne}
    };
}

inline void from_json(const nlohmann::json& j, EmpruntRecord& r) {
    if (j.contains("id")) r.id = j["id"].get<int>();
    if (j.contains("idJeu")) r.idJeu = j["idJeu"].get<int>();
    if (j.contains("idMembre")) r.idMembre = j["idMembre"].get<int>();
    if (j.contains("dateEmprunt")) r.dateEmprunt = j["dateEmprunt"].get<std::string>();
    if (j.contains("dateRetourPrevue")) r.dateRetourPrevue = j["dateRetourPrevue"].get<std::string>();
    if (j.contains("retourne")) r.retourne = j["retourne"].get<bool>();
}

} // namespace gl
