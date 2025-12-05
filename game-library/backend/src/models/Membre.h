#pragma once

#include <string>
#include <vector>
#include <json/json.h>

/**
 * @class Membre
 * @brief Represents a library member
 */
class Membre {
private:
    int id;
    std::string nom;
    std::string email;
    std::string telephone;
    std::string adresse;
    std::vector<int> empruntsActifs;  // Vector of game IDs currently borrowed
    std::string dateInscription;

public:
    // Constructor
    Membre(int id, const std::string& nom, const std::string& email = "",
           const std::string& telephone = "", const std::string& adresse = "");

    // Getters
    int getId() const { return id; }
    const std::string& getNom() const { return nom; }
    const std::string& getEmail() const { return email; }
    const std::string& getTelephone() const { return telephone; }
    const std::string& getAdresse() const { return adresse; }
    const std::vector<int>& getEmpruntsActifs() const { return empruntsActifs; }
    const std::string& getDateInscription() const { return dateInscription; }

    // Setters
    void setNom(const std::string& n) { nom = n; }
    void setEmail(const std::string& e) { email = e; }
    void setTelephone(const std::string& t) { telephone = t; }
    void setAdresse(const std::string& a) { adresse = a; }
    void setDateInscription(const std::string& date) { dateInscription = date; }

    // Borrow management
    void ajouterEmprunt(int idJeu);
    void retirerEmprunt(int idJeu);
    bool hasEmprunt(int idJeu) const;
    int getNombreEmpruntsActifs() const { return empruntsActifs.size(); }

    // JSON serialization
    Json::Value toJson() const;
    void fromJson(const Json::Value& json);

    // Display info
    void afficherInfo() const;
};
