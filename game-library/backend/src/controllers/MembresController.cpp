#include "MembresController.h"
#include "../models/Membre.h"
#include <json/json.h>

// Use external service instance (defined in JeuxController.cpp)
extern std::unique_ptr<BibliothequeService> gService;

BibliothequeService& MembresController::getService() {
    static BibliothequeService service;
    return service;
}

void MembresController::getAllMembres(const HttpRequestPtr& req,
                                      std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto& service = getService();
        auto membres = service.getAllMembresConst();
        
        Json::Value result(Json::arrayValue);
        for (const auto* membre : membres) {
            result.append(membre->toJson());
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

void MembresController::createMembre(const HttpRequestPtr& req,
                                     std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto json = req->getJsonObject();
        
        if (!json || !json->isMember("nom")) {
            Json::Value error;
            error["error"] = "Missing required field: nom";
            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }
        
        std::string nom = (*json)["nom"].asString();
        std::string email = json->isMember("email") ? (*json)["email"].asString() : "";
        std::string telephone = json->isMember("telephone") ? (*json)["telephone"].asString() : "";
        std::string adresse = json->isMember("adresse") ? (*json)["adresse"].asString() : "";
        
        Membre membre(0, nom, email, telephone, adresse);
        auto& service = getService();
        int id = service.ajouterMembre(membre);
        
        Json::Value result;
        result["id"] = id;
        result["message"] = "Member created successfully";
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

void MembresController::getMembreById(const HttpRequestPtr& req,
                                      std::function<void(const HttpResponsePtr&)>&& callback,
                                      int id) {
    try {
        auto& service = getService();
        const auto* membre = service.getMembreConst(id);
        
        auto resp = HttpResponse::newHttpJsonResponse(membre->toJson());
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

void MembresController::updateMembre(const HttpRequestPtr& req,
                                     std::function<void(const HttpResponsePtr&)>&& callback,
                                     int id) {
    try {
        auto& service = getService();
        auto json = req->getJsonObject();
        
        const auto* oldMembre = service.getMembreConst(id);
        
        std::string nom = json->isMember("nom") ? (*json)["nom"].asString() : oldMembre->getNom();
        std::string email = json->isMember("email") ? (*json)["email"].asString() : oldMembre->getEmail();
        std::string telephone = json->isMember("telephone") ? (*json)["telephone"].asString() : oldMembre->getTelephone();
        std::string adresse = json->isMember("adresse") ? (*json)["adresse"].asString() : oldMembre->getAdresse();
        
        Membre membre(id, nom, email, telephone, adresse);
        service.updateMembre(id, membre);
        
        Json::Value result;
        result["message"] = "Member updated successfully";
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

void MembresController::deleteMembre(const HttpRequestPtr& req,
                                     std::function<void(const HttpResponsePtr&)>&& callback,
                                     int id) {
    try {
        auto& service = getService();
        service.deleteMembre(id);
        
        Json::Value result;
        result["message"] = "Member deleted successfully";
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
