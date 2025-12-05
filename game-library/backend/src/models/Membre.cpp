#include "Membre.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

Membre::Membre(int id, const std::string& nom, const std::string& email,
               const std::string& telephone, const std::string& adresse)
    : id(id), nom(nom), email(email), telephone(telephone), adresse(adresse) {
    // Set current date
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    dateInscription = oss.str();
}

void Membre::ajouterEmprunt(int idJeu) {
    if (!hasEmprunt(idJeu)) {
        empruntsActifs.push_back(idJeu);
    }
}

void Membre::retirerEmprunt(int idJeu) {
    auto it = std::find(empruntsActifs.begin(), empruntsActifs.end(), idJeu);
    if (it != empruntsActifs.end()) {
        empruntsActifs.erase(it);
    }
}

bool Membre::hasEmprunt(int idJeu) const {
    return std::find(empruntsActifs.begin(), empruntsActifs.end(), idJeu) != empruntsActifs.end();
}

void Membre::afficherInfo() const {
    std::cout << "Member: " << nom << "\n"
              << "Email: " << email << "\n"
              << "Phone: " << telephone << "\n"
              << "Address: " << adresse << "\n"
              << "Registration Date: " << dateInscription << "\n"
              << "Active Borrows: " << empruntsActifs.size() << "\n";
}

Json::Value Membre::toJson() const {
    Json::Value json;
    json["id"] = id;
    json["nom"] = nom;
    json["email"] = email;
    json["telephone"] = telephone;
    json["adresse"] = adresse;
    json["dateInscription"] = dateInscription;
    
    Json::Value emprunts(Json::arrayValue);
    for (int eid : empruntsActifs) {
        emprunts.append(eid);
    }
    json["empruntsActifs"] = emprunts;
    
    return json;
}

void Membre::fromJson(const Json::Value& json) {
    if (json.isMember("nom")) nom = json["nom"].asString();
    if (json.isMember("email")) email = json["email"].asString();
    if (json.isMember("telephone")) telephone = json["telephone"].asString();
    if (json.isMember("adresse")) adresse = json["adresse"].asString();
    if (json.isMember("dateInscription")) dateInscription = json["dateInscription"].asString();
    
    if (json.isMember("empruntsActifs") && json["empruntsActifs"].isArray()) {
        empruntsActifs.clear();
        for (const auto& eid : json["empruntsActifs"]) {
            empruntsActifs.push_back(eid.asInt());
        }
    }
}
