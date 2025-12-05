#pragma once

#include <drogon/HttpController.h>
#include "../services/BibliothequeService.h"
#include <memory>

using namespace drogon;

/**
 * @class MembresController
 * @brief REST API controller for member management
 */
class MembresController : public drogon::HttpController<MembresController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(MembresController::getAllMembres, "/membres", Get);
    ADD_METHOD_TO(MembresController::createMembre, "/membres", Post);
    ADD_METHOD_TO(MembresController::getMembreById, "/membres/{id}", Get);
    ADD_METHOD_TO(MembresController::updateMembre, "/membres/{id}", Put);
    ADD_METHOD_TO(MembresController::deleteMembre, "/membres/{id}", Delete);
    METHOD_LIST_END

    MembresController() = default;

    void getAllMembres(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback);

    void createMembre(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback);

    void getMembreById(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback,
                       int id);

    void updateMembre(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback,
                      int id);

    void deleteMembre(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback,
                      int id);

private:
    static BibliothequeService& getService();
};
