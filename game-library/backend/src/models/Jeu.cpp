#include "Jeu.h"
#include <iostream>
#include <stdexcept>

// ==================== Jeu Implementation ====================

Jeu::Jeu(int id, const std::string& titre, const std::string& categorie,
         bool disponible, const std::string& description,
         const std::string& auteur, int anneePublication)
    : id(id), titre(titre), categorie(categorie), disponible(disponible),
      description(description), auteur(auteur), anneePublication(anneePublication) {}

void Jeu::emprunter() {
    if (!disponible) {
        throw std::runtime_error("This game is not available for borrowing");
    }
    disponible = false;
}

void Jeu::restituer() {
    disponible = true;
}

void Jeu::afficherInfo() const {
    std::cout << "Game: " << titre << "\n"
              << "Category: " << categorie << "\n"
              << "Available: " << (disponible ? "Yes" : "No") << "\n"
              << "Description: " << description << "\n"
              << "Author: " << auteur << "\n"
              << "Year: " << anneePublication << "\n";
}

Json::Value Jeu::toJson() const {
    Json::Value json;
    json["id"] = id;
    json["titre"] = titre;
    json["categorie"] = categorie;
    json["disponible"] = disponible;
    json["description"] = description;
    json["auteur"] = auteur;
    json["anneePublication"] = anneePublication;
    json["type"] = "Jeu";
    return json;
}

void Jeu::fromJson(const Json::Value& json) {
    if (json.isMember("titre")) titre = json["titre"].asString();
    if (json.isMember("categorie")) categorie = json["categorie"].asString();
    if (json.isMember("disponible")) disponible = json["disponible"].asBool();
    if (json.isMember("description")) description = json["description"].asString();
    if (json.isMember("auteur")) auteur = json["auteur"].asString();
    if (json.isMember("anneePublication")) anneePublication = json["anneePublication"].asInt();
}

// ==================== JeuCarte Implementation ====================

JeuCarte::JeuCarte(int id, const std::string& titre, const std::string& categorie,
                   int nbCartes, bool disponible, const std::string& description,
                   const std::string& auteur, int anneePublication)
    : Jeu(id, titre, categorie, disponible, description, auteur, anneePublication),
      nbCartes(nbCartes) {}

void JeuCarte::afficherInfo() const {
    Jeu::afficherInfo();
    std::cout << "Number of Cards: " << nbCartes << "\n";
}

Json::Value JeuCarte::toJson() const {
    Json::Value json = Jeu::toJson();
    json["nbCartes"] = nbCartes;
    json["type"] = "JeuCarte";
    return json;
}

void JeuCarte::fromJson(const Json::Value& json) {
    Jeu::fromJson(json);
    if (json.isMember("nbCartes")) nbCartes = json["nbCartes"].asInt();
}

// ==================== JeuStrategie Implementation ====================

JeuStrategie::JeuStrategie(int id, const std::string& titre, const std::string& categorie,
                           int complexite, bool disponible, const std::string& description,
                           const std::string& auteur, int anneePublication)
    : Jeu(id, titre, categorie, disponible, description, auteur, anneePublication),
      complexite(complexite) {}

void JeuStrategie::afficherInfo() const {
    Jeu::afficherInfo();
    std::cout << "Complexity (1-10): " << complexite << "\n";
}

Json::Value JeuStrategie::toJson() const {
    Json::Value json = Jeu::toJson();
    json["complexite"] = complexite;
    json["type"] = "JeuStrategie";
    return json;
}

void JeuStrategie::fromJson(const Json::Value& json) {
    Jeu::fromJson(json);
    if (json.isMember("complexite")) complexite = json["complexite"].asInt();
}

// ==================== JeuSocieteClassique Implementation ====================

JeuSocieteClassique::JeuSocieteClassique(int id, const std::string& titre, const std::string& categorie,
                                         int nbJoueurs, int duree, bool disponible,
                                         const std::string& description, const std::string& auteur,
                                         int anneePublication)
    : Jeu(id, titre, categorie, disponible, description, auteur, anneePublication),
      nbJoueurs(nbJoueurs), duree(duree) {}

void JeuSocieteClassique::afficherInfo() const {
    Jeu::afficherInfo();
    std::cout << "Number of Players: " << nbJoueurs << "\n"
              << "Duration (minutes): " << duree << "\n";
}

Json::Value JeuSocieteClassique::toJson() const {
    Json::Value json = Jeu::toJson();
    json["nbJoueurs"] = nbJoueurs;
    json["duree"] = duree;
    json["type"] = "JeuSocieteClassique";
    return json;
}

void JeuSocieteClassique::fromJson(const Json::Value& json) {
    Jeu::fromJson(json);
    if (json.isMember("nbJoueurs")) nbJoueurs = json["nbJoueurs"].asInt();
    if (json.isMember("duree")) duree = json["duree"].asInt();
}
