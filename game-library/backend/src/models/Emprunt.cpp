#include "Emprunt.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

Emprunt::Emprunt(int id, int idJeu, int idMembre,
                 const std::string& dateEmprunt, const std::string& dateRetourPrevue)
    : id(id), idJeu(idJeu), idMembre(idMembre),
      dateEmprunt(dateEmprunt), dateRetourPrevue(dateRetourPrevue),
      estRetourne(false) {}

void Emprunt::marquerCommeRetourne() {
    estRetourne = true;
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    dateRetour = oss.str();
}

bool Emprunt::estEnRetard() const {
    if (estRetourne) return false;  // Already returned, not late
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    std::string today = oss.str();
    
    return today > dateRetourPrevue;
}

void Emprunt::afficherInfo() const {
    std::cout << "Borrow ID: " << id << "\n"
              << "Game ID: " << idJeu << "\n"
              << "Member ID: " << idMembre << "\n"
              << "Borrow Date: " << dateEmprunt << "\n"
              << "Expected Return: " << dateRetourPrevue << "\n"
              << "Actual Return: " << (dateRetour.empty() ? "Not returned" : dateRetour) << "\n"
              << "Status: " << (estRetourne ? "Returned" : 
                               (estEnRetard() ? "OVERDUE" : "Active")) << "\n";
}

Json::Value Emprunt::toJson() const {
    Json::Value json;
    json["id"] = id;
    json["idJeu"] = idJeu;
    json["idMembre"] = idMembre;
    json["dateEmprunt"] = dateEmprunt;
    json["dateRetour"] = dateRetour.empty() ? Json::Value::null : Json::Value(dateRetour);
    json["dateRetourPrevue"] = dateRetourPrevue;
    json["estRetourne"] = estRetourne;
    json["estEnRetard"] = estEnRetard();
    return json;
}

void Emprunt::fromJson(const Json::Value& json) {
    if (json.isMember("dateRetour") && !json["dateRetour"].isNull()) {
        dateRetour = json["dateRetour"].asString();
    }
    if (json.isMember("estRetourne")) estRetourne = json["estRetourne"].asBool();
}
