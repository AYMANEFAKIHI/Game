#pragma once

#include <string>
#include <json/json.h>

/**
 * @class Emprunt
 * @brief Represents a borrowing operation
 */
class Emprunt {
private:
    int id;
    int idJeu;
    int idMembre;
    std::string dateEmprunt;
    std::string dateRetour;  // Empty if not returned yet
    std::string dateRetourPrevue;
    bool estRetourne;

public:
    // Constructor
    Emprunt(int id, int idJeu, int idMembre, 
            const std::string& dateEmprunt, const std::string& dateRetourPrevue);

    // Getters
    int getId() const { return id; }
    int getIdJeu() const { return idJeu; }
    int getIdMembre() const { return idMembre; }
    const std::string& getDateEmprunt() const { return dateEmprunt; }
    const std::string& getDateRetour() const { return dateRetour; }
    const std::string& getDateRetourPrevue() const { return dateRetourPrevue; }
    bool estRetourne_() const { return estRetourne; }

    // Setters
    void setDateRetour(const std::string& date) { 
        dateRetour = date;
        estRetourne = true;
    }

    // Operations
    void marquerCommeRetourne();
    bool estEnRetard() const;

    // JSON serialization
    Json::Value toJson() const;
    void fromJson(const Json::Value& json);

    // Display info
    void afficherInfo() const;
};
