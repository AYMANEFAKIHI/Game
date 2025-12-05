#include "EmpruntsController.h"
#include "../models/Emprunt.h"
#include <json/json.h>

extern std::unique_ptr<BibliothequeService> gService;

BibliothequeService& EmpruntsController::getService() {
    static BibliothequeService service;
    return service;
}

void EmpruntsController::getAllEmprunts(const HttpRequestPtr& req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto& service = getService();
        auto emprunts = service.getAllEmpruntsConst();
        
        Json::Value result(Json::arrayValue);
        for (const auto* emprunt : emprunts) {
            result.append(emprunt->toJson());
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

void EmpruntsController::emprunter(const HttpRequestPtr& req,
                                   std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto json = req->getJsonObject();
        
        if (!json || !json->isMember("idJeu") || !json->isMember("idMembre") ||
            !json->isMember("dateRetourPrevue")) {
            Json::Value error;
            error["error"] = "Missing required fields: idJeu, idMembre, dateRetourPrevue";
            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }
        
        int idJeu = (*json)["idJeu"].asInt();
        int idMembre = (*json)["idMembre"].asInt();
        std::string dateRetourPrevue = (*json)["dateRetourPrevue"].asString();
        
        auto& service = getService();
        int empruntId = service.emprunter(idJeu, idMembre, dateRetourPrevue);
        
        Json::Value result;
        result["id"] = empruntId;
        result["message"] = "Game borrowed successfully";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k201Created);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
    }
}

void EmpruntsController::rendre(const HttpRequestPtr& req,
                                std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto json = req->getJsonObject();
        
        if (!json || !json->isMember("idEmprunt")) {
            Json::Value error;
            error["error"] = "Missing required field: idEmprunt";
            auto resp = HttpResponse::newHttpJsonResponse(error);
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }
        
        int idEmprunt = (*json)["idEmprunt"].asInt();
        
        auto& service = getService();
        service.rendre(idEmprunt);
        
        Json::Value result;
        result["message"] = "Game returned successfully";
        auto resp = HttpResponse::newHttpJsonResponse(result);
        resp->setStatusCode(k200OK);
        callback(resp);
    } catch (const std::exception& e) {
        Json::Value error;
        error["error"] = e.what();
        auto resp = HttpResponse::newHttpJsonResponse(error);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
    }
}

void EmpruntsController::getEmpruntsForMembre(const HttpRequestPtr& req,
                                             std::function<void(const HttpResponsePtr&)>&& callback,
                                             int idMembre) {
    try {
        auto& service = getService();
        auto emprunts = service.getEmpruntsActifs(idMembre);
        
        Json::Value result(Json::arrayValue);
        for (const auto* emprunt : emprunts) {
            result.append(emprunt->toJson());
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

void EmpruntsController::getOverdueEmprunts(const HttpRequestPtr& req,
                                           std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        auto& service = getService();
        auto emprunts = service.getEmpruntsEnRetard();
        
        Json::Value result(Json::arrayValue);
        for (const auto* emprunt : emprunts) {
            result.append(emprunt->toJson());
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
