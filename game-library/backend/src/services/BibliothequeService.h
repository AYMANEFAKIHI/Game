#pragma once

#include "Jeu.h"
#include "Membre.h"
#include "Emprunt.h"
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @class BibliothequeService
 * @brief Central service managing all library operations
 * 
 * This class implements the library's business logic and manages
 * all games, members, and borrowing operations.
 */
class BibliothequeService {
private:
    std::map<int, std::unique_ptr<Jeu>> jeux;
    std::map<int, Membre> membres;
    std::map<int, Emprunt> emprunts;
    
    int nextJeuId = 1;
    int nextMembreId = 1;
    int nextEmpruntId = 1;

    // Private helper methods
    Jeu* findJeu(int id);
    const Jeu* findJeuConst(int id) const;
    Membre* findMembre(int id);
    const Membre* findMembreConst(int id) const;

public:
    BibliothequeService() = default;
    ~BibliothequeService() = default;

    // ===================== JEU OPERATIONS =====================
    
    /**
     * Add a new game to the library
     */
    int ajouterJeu(std::unique_ptr<Jeu> jeu);
    
    /**
     * Get all games
     */
    std::vector<Jeu*> getAllJeux();
    std::vector<const Jeu*> getAllJeuxConst() const;
    
    /**
     * Get a specific game
     */
    Jeu* getJeu(int id);
    const Jeu* getJeuConst(int id) const;
    
    /**
     * Update a game
     */
    void updateJeu(int id, std::unique_ptr<Jeu> jeu);
    
    /**
     * Delete a game
     */
    void deleteJeu(int id);
    
    /**
     * Get available games
     */
    std::vector<Jeu*> getJeuxDisponibles();
    
    // ===================== MEMBRE OPERATIONS =====================
    
    /**
     * Add a new member
     */
    int ajouterMembre(const Membre& membre);
    
    /**
     * Get all members
     */
    std::vector<Membre*> getAllMembres();
    std::vector<const Membre*> getAllMembresConst() const;
    
    /**
     * Get a specific member
     */
    Membre* getMembre(int id);
    const Membre* getMembreConst(int id) const;
    
    /**
     * Update a member
     */
    void updateMembre(int id, const Membre& membre);
    
    /**
     * Delete a member
     */
    void deleteMembre(int id);
    
    // ===================== EMPRUNT OPERATIONS =====================
    
    /**
     * Create a new borrow operation
     * @throws std::runtime_error if game is not available
     */
    int emprunter(int idJeu, int idMembre, const std::string& dateRetourPrevue);
    
    /**
     * Return a borrowed game
     */
    void rendre(int idEmprunt);
    
    /**
     * Get all borrow operations
     */
    std::vector<Emprunt*> getAllEmprunts();
    std::vector<const Emprunt*> getAllEmpruntsConst() const;
    
    /**
     * Get active borrows for a member
     */
    std::vector<Emprunt*> getEmpruntsActifs(int idMembre);
    
    /**
     * Get a specific borrow
     */
    Emprunt* getEmprunt(int id);
    const Emprunt* getEmpruntConst(int id) const;
    
    /**
     * Get overdue borrows
     */
    std::vector<Emprunt*> getEmpruntsEnRetard();
};
