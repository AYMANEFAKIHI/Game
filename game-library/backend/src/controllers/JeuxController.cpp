#include "JeuxController.h"
#include "../models/Jeu.h"
#include <json/json.h>

// Global service instance
static std::unique_ptr<BibliothequeService> gService;

BibliothequeService& JeuxController::getService() {
    if (!gService) {
        gService = std::make_unique<BibliothequeService>();
    }
    return *gService;
}

void JeuxController::getAllJeux(const HttpRequestPtr& req,
                                std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto& service = getService();
        auto jeux = service.getAllJeuxConst();
        
        Json::Value result(Json::arrayValue);
        for (const auto* jeu : jeux) {
            result.append(jeu->toJson());
        }
        
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

void JeuxController::createJeu(const HttpRequestPtr& req,
                               std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto json = req->getJsonObject();
        
        if (!json || !json->isMember("titre") || !json->isMember("categorie") ||
            !json->isMember("type")) {
            Json::Value error;
            error["error"] = "Missing required fields: titre, categorie, type";
            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }
        
        std::string type = (*json)["type"].asString();
        bool disponible = json->isMember("disponible") ? (*json)["disponible"].asBool() : true;
        std::string description = json->isMember("description") ? (*json)["description"].asString() : "";
        std::string auteur = json->isMember("auteur") ? (*json)["auteur"].asString() : "";
        int anneePublication = json->isMember("anneePublication") ? (*json)["anneePublication"].asInt() : 0;
        
        std::unique_ptr<Jeu> jeu;
        
        if (type == "JeuCarte") {
            if (!json->isMember("nbCartes")) {
                throw std::runtime_error("JeuCarte requires nbCartes");
            }
            int nbCartes = (*json)["nbCartes"].asInt();
            jeu = std::make_unique<JeuCarte>(0, (*json)["titre"].asString(),
                                            (*json)["categorie"].asString(),
                                            nbCartes, disponible, description,
                                            auteur, anneePublication);
        } else if (type == "JeuStrategie") {
            if (!json->isMember("complexite")) {
                throw std::runtime_error("JeuStrategie requires complexite");
            }
            int complexite = (*json)["complexite"].asInt();
            jeu = std::make_unique<JeuStrategie>(0, (*json)["titre"].asString(),
                                                (*json)["categorie"].asString(),
                                                complexite, disponible, description,
                                                auteur, anneePublication);
        } else if (type == "JeuSocieteClassique") {
            if (!json->isMember("nbJoueurs") || !json->isMember("duree")) {
                throw std::runtime_error("JeuSocieteClassique requires nbJoueurs and duree");
            }
            int nbJoueurs = (*json)["nbJoueurs"].asInt();
            int duree = (*json)["duree"].asInt();
            jeu = std::make_unique<JeuSocieteClassique>(0, (*json)["titre"].asString(),
                                                       (*json)["categorie"].asString(),
                                                       nbJoueurs, duree, disponible,
                                                       description, auteur, anneePublication);
        } else {
            jeu = std::make_unique<Jeu>(0, (*json)["titre"].asString(),
                                       (*json)["categorie"].asString(),
                                       disponible, description, auteur, anneePublication);
        }
        
        auto& service = getService();
        int id = service.ajouterJeu(std::move(jeu));
        
        Json::Value result;
        result["id"] = id;
        result["message"] = "Game created successfully";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k201Created);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

void JeuxController::getJeuById(const HttpRequestPtr& req,
                                std::function<void(const HttpResponsePtr&)>&& callback,
                                int id) {
    try {
        auto& service = getService();
        const auto* jeu = service.getJeuConst(id);
        
        auto resp = HttpResponse::newHttpJsonResponse(jeu->toJson());
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k404NotFound);
        callback(resp);
    }
}

void JeuxController::updateJeu(const HttpRequestPtr& req,
                               std::function<void(const HttpResponsePtr&)>&& callback,
                               int id) {
    try {
        auto& service = getService();
        auto json = req->getJsonObject();
        
        if (!json || !json->isMember("type")) {
            Json::Value error;
            error["error"] = "Missing required field: type";
            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }
        
        const auto* oldJeu = service.getJeuConst(id);
        std::string type = (*json)["type"].asString();
        
        bool disponible = json->isMember("disponible") ? (*json)["disponible"].asBool() : oldJeu->estDisponible();
        std::string titre = json->isMember("titre") ? (*json)["titre"].asString() : oldJeu->getTitre();
        std::string categorie = json->isMember("categorie") ? (*json)["categorie"].asString() : oldJeu->getCategorie();
        std::string description = json->isMember("description") ? (*json)["description"].asString() : oldJeu->getDescription();
        std::string auteur = json->isMember("auteur") ? (*json)["auteur"].asString() : oldJeu->getAuteur();
        int anneePublication = json->isMember("anneePublication") ? (*json)["anneePublication"].asInt() : oldJeu->getAnneePublication();
        
        std::unique_ptr<Jeu> jeu;
        
        if (type == "JeuCarte") {
            const JeuCarte* carte = dynamic_cast<const JeuCarte*>(oldJeu);
            int nbCartes = json->isMember("nbCartes") ? (*json)["nbCartes"].asInt() : 
                          (carte ? carte->getNbCartes() : 0);
            jeu = std::make_unique<JeuCarte>(id, titre, categorie, nbCartes, disponible,
                                            description, auteur, anneePublication);
        } else if (type == "JeuStrategie") {
            const JeuStrategie* strat = dynamic_cast<const JeuStrategie*>(oldJeu);
            int complexite = json->isMember("complexite") ? (*json)["complexite"].asInt() :
                            (strat ? strat->getComplexite() : 5);
            jeu = std::make_unique<JeuStrategie>(id, titre, categorie, complexite, disponible,
                                                description, auteur, anneePublication);
        } else if (type == "JeuSocieteClassique") {
            const JeuSocieteClassique* classique = dynamic_cast<const JeuSocieteClassique*>(oldJeu);
            int nbJoueurs = json->isMember("nbJoueurs") ? (*json)["nbJoueurs"].asInt() :
                           (classique ? classique->getNbJoueurs() : 2);
            int duree = json->isMember("duree") ? (*json)["duree"].asInt() :
                       (classique ? classique->getDuree() : 30);
            jeu = std::make_unique<JeuSocieteClassique>(id, titre, categorie, nbJoueurs, duree,
                                                       disponible, description, auteur, anneePublication);
        } else {
            jeu = std::make_unique<Jeu>(id, titre, categorie, disponible, description, auteur, anneePublication);
        }
        
        service.updateJeu(id, std::move(jeu));
        
        Json::Value result;
        result["message"] = "Game updated successfully";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

void JeuxController::deleteJeu(const HttpRequestPtr& req,
                               std::function<void(const HttpResponsePtr&)>&& callback,
                               int id) {
    try {
        auto& service = getService();
        service.deleteJeu(id);
        
        Json::Value result;
        result["message"] = "Game deleted successfully";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k404NotFound);
        callback(resp);
    }
}

void JeuxController::getAvailableJeux(const HttpRequestPtr& req,
                                     std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto& service = getService();
        auto jeux = service.getJeuxDisponibles();
        
        Json::Value result(Json::arrayValue);
        for (const auto* jeu : jeux) {
            result.append(jeu->toJson());
        }
        
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}
