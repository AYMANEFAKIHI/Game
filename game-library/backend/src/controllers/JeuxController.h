#pragma once

#include <drogon/HttpController.h>
#include "../services/BibliothequeService.h"
#include <memory>

using namespace drogon;

/**
 * @class JeuxController
 * @brief REST API controller for game management
 */
class JeuxController : public drogon::HttpController<JeuxController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(JeuxController::getAllJeux, "/jeux", Get);
    ADD_METHOD_TO(JeuxController::createJeu, "/jeux", Post);
    ADD_METHOD_TO(JeuxController::getJeuById, "/jeux/{id}", Get);
    ADD_METHOD_TO(JeuxController::updateJeu, "/jeux/{id}", Put);
    ADD_METHOD_TO(JeuxController::deleteJeu, "/jeux/{id}", Delete);
    ADD_METHOD_TO(JeuxController::getAvailableJeux, "/jeux/disponibles/all", Get);
    METHOD_LIST_END

    JeuxController() = default;

    // Get all games
    void getAllJeux(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);

    // Create a new game
    void createJeu(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback);

    // Get game by ID
    void getJeuById(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback,
                   int id);

    // Update a game
    void updateJeu(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback,
                  int id);

    // Delete a game
    void deleteJeu(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback,
                  int id);

    // Get available games
    void getAvailableJeux(const HttpRequestPtr& req,
                         std::function<void(const HttpResponsePtr&)>&& callback);

private:
    static BibliothequeService& getService();
};
