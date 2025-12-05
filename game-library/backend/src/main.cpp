/**
 * @file main.cpp
 * @brief Board Game Library API - Main Entry Point
 * 
 * This file initializes and starts the Drogon HTTP server
 * for the Board Game Library REST API.
 * 
 * @author Development Team
 * @date January 2025
 * @version 1.0.0
 */

#include <drogon/drogon.h>
#include "src/controllers/JeuxController.h"
#include "src/controllers/MembresController.h"
#include "src/controllers/EmpruntsController.h"

/**
 * Main entry point for the Board Game Library API
 * 
 * Initializes:
 * - HTTP server on port 8080
 * - Controller routing (automatically registered)
 * - Thread pool with 4 worker threads
 * - Dynamic view caching disabled
 * 
 * @return 0 on successful completion, 1 on error
 */
int main() {
    try {
        LOG_INFO << "╔════════════════════════════════════════════╗";
        LOG_INFO << "║  Board Game Library Management System      ║";
        LOG_INFO << "║  Version 1.0.0                            ║";
        LOG_INFO << "║  Powered by Drogon Framework              ║";
        LOG_INFO << "╚════════════════════════════════════════════╝";
        
        // Set up HTTP listener on port 8080
        drogon::app().addListener("0.0.0.0", 8080);
        
        // Set number of threads
        drogon::app().setThreadNum(4);
        
        // Enable document generation
        drogon::app().enableDynamicViewsCache(false);
        
        LOG_INFO << "";
        LOG_INFO << "Server Configuration:";
        LOG_INFO << "  - Host: 0.0.0.0";
        LOG_INFO << "  - Port: 8080";
        LOG_INFO << "  - Worker Threads: 4";
        LOG_INFO << "";
        LOG_INFO << "Available Endpoints:";
        LOG_INFO << "  - GET/POST/PUT/DELETE /jeux (game management)";
        LOG_INFO << "  - GET/POST/PUT/DELETE /membres (member management)";
        LOG_INFO << "  - GET/POST /emprunter (borrowing)";
        LOG_INFO << "  - POST /rendre (returning borrowed games)";
        LOG_INFO << "  - GET /emprunts (view all borrows)";
        LOG_INFO << "  - GET /emprunts/retard/all (view overdue borrows)";
        LOG_INFO << "";
        LOG_INFO << "API Documentation: See docs/API.md";
        LOG_INFO << "Frontend: http://localhost:5173";
        LOG_INFO << "API Base: http://localhost:8080";
        LOG_INFO << "";
        LOG_INFO << "Starting server...";
        
        // Run the application
        drogon::app().run();
        
        return 0;
    } catch (const std::exception& e) {
        LOG_ERROR << "Fatal error: " << e.what();
        return 1;
    }
}
