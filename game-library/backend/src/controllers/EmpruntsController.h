#pragma once

#include <drogon/HttpController.h>
#include "../services/BibliothequeService.h"
#include <memory>

using namespace drogon;

/**
 * @class EmpruntsController
 * @brief REST API controller for borrow management
 */
class EmpruntsController : public drogon::HttpController<EmpruntsController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(EmpruntsController::getAllEmprunts, "/emprunts", Get);
    ADD_METHOD_TO(EmpruntsController::emprunter, "/emprunter", Post);
    ADD_METHOD_TO(EmpruntsController::rendre, "/rendre", Post);
    ADD_METHOD_TO(EmpruntsController::getEmpruntsForMembre, "/emprunts/membre/{idMembre}", Get);
    ADD_METHOD_TO(EmpruntsController::getOverdueEmprunts, "/emprunts/retard/all", Get);
    METHOD_LIST_END

    EmpruntsController() = default;

    void getAllEmprunts(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback);

    void emprunter(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);

    void rendre(const HttpRequestPtr& req,
                std::function<void(const HttpResponsePtr&)>&& callback);

    void getEmpruntsForMembre(const HttpRequestPtr& req,
                             std::function<void(const HttpResponsePtr&)>&& callback,
                             int idMembre);

    void getOverdueEmprunts(const HttpRequestPtr& req,
                           std::function<void(const HttpResponsePtr&)>&& callback);

private:
    static BibliothequeService& getService();
};
