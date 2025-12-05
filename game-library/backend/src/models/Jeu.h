#pragma once

#include <string>
#include <ctime>
#include <memory>
#include <vector>
#include <json/json.h>

/**
 * @class Jeu
 * @brief Base class representing a board game
 * 
 * This is the base class for all board games in the library.
 * It provides common functionality for game management including
 * borrowing and returning games.
 */
class Jeu {
protected:
    int id;
    std::string titre;
    std::string categorie;
    bool disponible;
    std::string description;
    std::string auteur;
    int anneePublication;

public:
    // Constructor
    Jeu(int id, const std::string& titre, const std::string& categorie,
        bool disponible = true, const std::string& description = "",
        const std::string& auteur = "", int anneePublication = 0);

    // Virtual destructor for proper polymorphism
    virtual ~Jeu() = default;

    // Getters
    int getId() const { return id; }
    const std::string& getTitre() const { return titre; }
    const std::string& getCategorie() const { return categorie; }
    bool estDisponible() const { return disponible; }
    const std::string& getDescription() const { return description; }
    const std::string& getAuteur() const { return auteur; }
    int getAnneePublication() const { return anneePublication; }

    // Setters
    void setDisponible(bool disp) { disponible = disp; }
    void setDescription(const std::string& desc) { description = desc; }
    void setAuteur(const std::string& aut) { auteur = aut; }

    // Game operations
    virtual void emprunter();
    virtual void restituer();
    virtual void afficherInfo() const;

    // Virtual method for JSON serialization
    virtual Json::Value toJson() const;
    virtual void fromJson(const Json::Value& json);

    // Get game type
    virtual std::string getType() const { return "Jeu"; }
};

/**
 * @class JeuCarte
 * @brief Card game specialization
 */
class JeuCarte : public Jeu {
private:
    int nbCartes;

public:
    JeuCarte(int id, const std::string& titre, const std::string& categorie,
             int nbCartes, bool disponible = true, const std::string& description = "",
             const std::string& auteur = "", int anneePublication = 0);

    int getNbCartes() const { return nbCartes; }
    void setNbCartes(int nb) { nbCartes = nb; }

    void afficherInfo() const override;
    Json::Value toJson() const override;
    void fromJson(const Json::Value& json) override;
    std::string getType() const override { return "JeuCarte"; }
};

/**
 * @class JeuStrategie
 * @brief Strategy game specialization
 */
class JeuStrategie : public Jeu {
private:
    int complexite;  // 1-10 scale

public:
    JeuStrategie(int id, const std::string& titre, const std::string& categorie,
                 int complexite, bool disponible = true, const std::string& description = "",
                 const std::string& auteur = "", int anneePublication = 0);

    int getComplexite() const { return complexite; }
    void setComplexite(int comp) { complexite = comp; }

    void afficherInfo() const override;
    Json::Value toJson() const override;
    void fromJson(const Json::Value& json) override;
    std::string getType() const override { return "JeuStrategie"; }
};

/**
 * @class JeuSocieteClassique
 * @brief Classic party game specialization
 */
class JeuSocieteClassique : public Jeu {
private:
    int nbJoueurs;
    int duree;  // in minutes

public:
    JeuSocieteClassique(int id, const std::string& titre, const std::string& categorie,
                        int nbJoueurs, int duree, bool disponible = true,
                        const std::string& description = "", const std::string& auteur = "",
                        int anneePublication = 0);

    int getNbJoueurs() const { return nbJoueurs; }
    int getDuree() const { return duree; }
    void setNbJoueurs(int nb) { nbJoueurs = nb; }
    void setDuree(int d) { duree = d; }

    void afficherInfo() const override;
    Json::Value toJson() const override;
    void fromJson(const Json::Value& json) override;
    std::string getType() const override { return "JeuSocieteClassique"; }
};
