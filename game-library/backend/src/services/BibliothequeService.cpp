#include "BibliothequeService.h"
#include <algorithm>

// ===================== HELPER METHODS =====================

Jeu* BibliothequeService::findJeu(int id) {
    auto it = jeux.find(id);
    if (it == jeux.end()) return nullptr;
    return it->second.get();
}

const Jeu* BibliothequeService::findJeuConst(int id) const {
    auto it = jeux.find(id);
    if (it == jeux.end()) return nullptr;
    return it->second.get();
}

Membre* BibliothequeService::findMembre(int id) {
    auto it = membres.find(id);
    if (it == membres.end()) return nullptr;
    return &it->second;
}

const Membre* BibliothequeService::findMembreConst(int id) const {
    auto it = membres.find(id);
    if (it == membres.end()) return nullptr;
    return &it->second;
}

// ===================== JEU OPERATIONS =====================

int BibliothequeService::ajouterJeu(std::unique_ptr<Jeu> jeu) {
    if (!jeu) throw std::invalid_argument("Jeu cannot be null");
    
    int id = nextJeuId++;
    jeux[id] = std::move(jeu);
    return id;
}

std::vector<Jeu*> BibliothequeService::getAllJeux() {
    std::vector<Jeu*> result;
    for (auto& pair : jeux) {
        result.push_back(pair.second.get());
    }
    return result;
}

std::vector<const Jeu*> BibliothequeService::getAllJeuxConst() const {
    std::vector<const Jeu*> result;
    for (const auto& pair : jeux) {
        result.push_back(pair.second.get());
    }
    return result;
}

Jeu* BibliothequeService::getJeu(int id) {
    Jeu* jeu = findJeu(id);
    if (!jeu) throw std::runtime_error("Game not found: " + std::to_string(id));
    return jeu;
}

const Jeu* BibliothequeService::getJeuConst(int id) const {
    const Jeu* jeu = findJeuConst(id);
    if (!jeu) throw std::runtime_error("Game not found: " + std::to_string(id));
    return jeu;
}

void BibliothequeService::updateJeu(int id, std::unique_ptr<Jeu> jeu) {
    if (!findJeu(id)) throw std::runtime_error("Game not found: " + std::to_string(id));
    jeux[id] = std::move(jeu);
}

void BibliothequeService::deleteJeu(int id) {
    if (!findJeu(id)) throw std::runtime_error("Game not found: " + std::to_string(id));
    jeux.erase(id);
}

std::vector<Jeu*> BibliothequeService::getJeuxDisponibles() {
    std::vector<Jeu*> result;
    for (auto& pair : jeux) {
        if (pair.second->estDisponible()) {
            result.push_back(pair.second.get());
        }
    }
    return result;
}

// ===================== MEMBRE OPERATIONS =====================

int BibliothequeService::ajouterMembre(const Membre& membre) {
    int id = nextMembreId++;
    Membre m = membre;
    m.setDateInscription("2025-01-01");  // Will be set properly in constructor
    membres[id] = m;
    return id;
}

std::vector<Membre*> BibliothequeService::getAllMembres() {
    std::vector<Membre*> result;
    for (auto& pair : membres) {
        result.push_back(&pair.second);
    }
    return result;
}

std::vector<const Membre*> BibliothequeService::getAllMembresConst() const {
    std::vector<const Membre*> result;
    for (const auto& pair : membres) {
        result.push_back(&pair.second);
    }
    return result;
}

Membre* BibliothequeService::getMembre(int id) {
    Membre* membre = findMembre(id);
    if (!membre) throw std::runtime_error("Member not found: " + std::to_string(id));
    return membre;
}

const Membre* BibliothequeService::getMembreConst(int id) const {
    const Membre* membre = findMembreConst(id);
    if (!membre) throw std::runtime_error("Member not found: " + std::to_string(id));
    return membre;
}

void BibliothequeService::updateMembre(int id, const Membre& membre) {
    if (!findMembre(id)) throw std::runtime_error("Member not found: " + std::to_string(id));
    membres[id] = membre;
}

void BibliothequeService::deleteMembre(int id) {
    if (!findMembre(id)) throw std::runtime_error("Member not found: " + std::to_string(id));
    membres.erase(id);
}

// ===================== EMPRUNT OPERATIONS =====================

int BibliothequeService::emprunter(int idJeu, int idMembre, const std::string& dateRetourPrevue) {
    Jeu* jeu = getJeu(idJeu);
    Membre* membre = getMembre(idMembre);
    
    if (!jeu->estDisponible()) {
        throw std::runtime_error("Game is not available");
    }
    
    // Mark game as borrowed
    jeu->emprunter();
    
    // Add to member's active borrows
    membre->ajouterEmprunt(idJeu);
    
    // Create borrow record
    int empruntId = nextEmpruntId++;
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    std::string dateEmprunt = oss.str();
    
    emprunts[empruntId] = Emprunt(empruntId, idJeu, idMembre, dateEmprunt, dateRetourPrevue);
    
    return empruntId;
}

void BibliothequeService::rendre(int idEmprunt) {
    Emprunt* emprunt = getEmprunt(idEmprunt);
    Jeu* jeu = getJeu(emprunt->getIdJeu());
    Membre* membre = getMembre(emprunt->getIdMembre());
    
    // Mark game as available
    jeu->restituer();
    
    // Remove from member's active borrows
    membre->retirerEmprunt(emprunt->getIdJeu());
    
    // Mark emprunt as returned
    emprunt->marquerCommeRetourne();
}

std::vector<Emprunt*> BibliothequeService::getAllEmprunts() {
    std::vector<Emprunt*> result;
    for (auto& pair : emprunts) {
        result.push_back(&pair.second);
    }
    return result;
}

std::vector<const Emprunt*> BibliothequeService::getAllEmpruntsConst() const {
    std::vector<const Emprunt*> result;
    for (const auto& pair : emprunts) {
        result.push_back(&pair.second);
    }
    return result;
}

std::vector<Emprunt*> BibliothequeService::getEmpruntsActifs(int idMembre) {
    std::vector<Emprunt*> result;
    for (auto& pair : emprunts) {
        if (pair.second.getIdMembre() == idMembre && !pair.second.estRetourne_()) {
            result.push_back(&pair.second);
        }
    }
    return result;
}

Emprunt* BibliothequeService::getEmprunt(int id) {
    auto it = emprunts.find(id);
    if (it == emprunts.end()) throw std::runtime_error("Borrow not found: " + std::to_string(id));
    return &it->second;
}

const Emprunt* BibliothequeService::getEmpruntConst(int id) const {
    auto it = emprunts.find(id);
    if (it == emprunts.end()) throw std::runtime_error("Borrow not found: " + std::to_string(id));
    return &it->second;
}

std::vector<Emprunt*> BibliothequeService::getEmpruntsEnRetard() {
    std::vector<Emprunt*> result;
    for (auto& pair : emprunts) {
        if (!pair.second.estRetourne_() && pair.second.estEnRetard()) {
            result.push_back(&pair.second);
        }
    }
    return result;
}
