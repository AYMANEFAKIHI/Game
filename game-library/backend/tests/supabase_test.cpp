#include <iostream>
#include <cstdlib>
#include "../include/SupabaseService.h"

int main() {
    try {
        gl::SupabaseService svc;

        std::cout << "Fetching games...\n";
        auto games = svc.getAllGames();
        std::cout << "Got " << games.size() << " games\n";

        // Simple smoke test: add and delete a temporary member
        gl::MembreRecord m;
        m.nom = "Test User";
        m.email = "test@example.com";

        std::cout << "Creating member...\n";
        auto created = svc.addMember(m);
        std::cout << "Created member id=" << created.id << "\n";

        std::cout << "Borrowing (if at least one game exists)...\n";
        if (!games.empty()) {
            auto emprunt = svc.borrowGame(games.front().id, created.id, "2025-12-05", "2025-12-12");
            std::cout << "Created emprunt id=" << emprunt.id << "\n";
            auto returned = svc.returnGame(emprunt.id);
            std::cout << "Returned emprunt id=" << returned.id << " retour=" << returned.retourne << "\n";
        }

        std::cout << "Test completed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 2;
    }
}
