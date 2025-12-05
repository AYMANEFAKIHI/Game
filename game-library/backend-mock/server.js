/**
 * Mock Backend Server for Board Game Library
 * Demonstrates the REST API without requiring C++ compilation
 * Port: 8080
 */

const http = require('http');
const url = require('url');

// In-memory data store
let jeux = [
    { id: 1, titre: 'Catan', categorie: 'Stratégie', type: 'JeuStrategie', disponible: true, description: 'Jeu stratégique classique', auteur: 'Klaus Teuber', anneePublication: 1995, complexite: 7 },
    { id: 2, titre: 'Carcassonne', categorie: 'Placement', type: 'JeuStrategie', disponible: true, description: 'Jeu de placement de tuiles', auteur: 'Klaus-Jürgen Wrede', anneePublication: 2000, complexite: 5 },
    { id: 3, titre: 'Magic The Gathering', categorie: 'Cartes', type: 'JeuCarte', disponible: false, description: 'Jeu de cartes collectibles', auteur: 'Richard Garfield', anneePublication: 1993, nbCartes: 300 },
];

let membres = [
    { id: 1, nom: 'Alice Dupont', email: 'alice@example.com', telephone: '06 01 02 03 04', adresse: '123 Rue de Paris', dateInscription: '2025-01-01', empruntsActifs: [1] },
    { id: 2, nom: 'Bob Martin', email: 'bob@example.com', telephone: '06 05 06 07 08', adresse: '456 Rue de Lyon', dateInscription: '2025-02-01', empruntsActifs: [] },
];

let emprunts = [
    { id: 1, idJeu: 3, idMembre: 1, dateEmprunt: '2025-11-01', dateRetourPrevue: '2025-12-01', dateRetour: null, estRetourne: false, estEnRetard: false },
];

let nextJeuId = 4;
let nextMembreId = 3;
let nextEmpruntId = 2;

const PORT = 3001;

// Helper functions
function setHeaders(res, statusCode = 200) {
    res.writeHead(statusCode, {
        'Content-Type': 'application/json',
        'Access-Control-Allow-Origin': '*',
        'Access-Control-Allow-Methods': 'GET, POST, PUT, DELETE, OPTIONS',
        'Access-Control-Allow-Headers': 'Content-Type'
    });
}

function sendJson(res, data, statusCode = 200) {
    setHeaders(res, statusCode);
    res.end(JSON.stringify(data));
}

function sendError(res, message, statusCode = 400) {
    sendJson(res, { error: message }, statusCode);
}

// Route handlers
function handleJeux(req, res, method, parts) {
    if (method === 'GET') {
        if (parts.length === 3 && parts[2] === 'disponibles') {
            // GET /jeux/disponibles/all
            const disponibles = jeux.filter(j => j.disponible);
            sendJson(res, disponibles);
        } else if (parts.length === 3) {
            // GET /jeux/:id
            const id = parseInt(parts[2]);
            const jeu = jeux.find(j => j.id === id);
            if (jeu) {
                sendJson(res, jeu);
            } else {
                sendError(res, 'Jeu non trouvé', 404);
            }
        } else {
            // GET /jeux
            sendJson(res, jeux);
        }
    } else if (method === 'POST') {
        // POST /jeux
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', () => {
            try {
                const newJeu = JSON.parse(body);
                newJeu.id = nextJeuId++;
                newJeu.disponible = true;
                jeux.push(newJeu);
                sendJson(res, newJeu, 201);
            } catch (e) {
                sendError(res, 'Invalid JSON', 400);
            }
        });
    } else if (method === 'PUT') {
        // PUT /jeux/:id
        const id = parseInt(parts[2]);
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', () => {
            try {
                const updated = JSON.parse(body);
                const jeu = jeux.find(j => j.id === id);
                if (jeu) {
                    Object.assign(jeu, updated);
                    sendJson(res, jeu);
                } else {
                    sendError(res, 'Jeu non trouvé', 404);
                }
            } catch (e) {
                sendError(res, 'Invalid JSON', 400);
            }
        });
    } else if (method === 'DELETE') {
        // DELETE /jeux/:id
        const id = parseInt(parts[2]);
        const idx = jeux.findIndex(j => j.id === id);
        if (idx !== -1) {
            const deleted = jeux.splice(idx, 1)[0];
            sendJson(res, { message: 'Jeu supprimé', id: deleted.id });
        } else {
            sendError(res, 'Jeu non trouvé', 404);
        }
    }
}

function handleMembres(req, res, method, parts) {
    if (method === 'GET') {
        if (parts.length === 3) {
            // GET /membres/:id
            const id = parseInt(parts[2]);
            const membre = membres.find(m => m.id === id);
            if (membre) {
                sendJson(res, membre);
            } else {
                sendError(res, 'Membre non trouvé', 404);
            }
        } else {
            // GET /membres
            sendJson(res, membres);
        }
    } else if (method === 'POST') {
        // POST /membres
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', () => {
            try {
                const newMembre = JSON.parse(body);
                newMembre.id = nextMembreId++;
                newMembre.dateInscription = new Date().toISOString().split('T')[0];
                newMembre.empruntsActifs = [];
                membres.push(newMembre);
                sendJson(res, newMembre, 201);
            } catch (e) {
                sendError(res, 'Invalid JSON', 400);
            }
        });
    } else if (method === 'PUT') {
        // PUT /membres/:id
        const id = parseInt(parts[2]);
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', () => {
            try {
                const updated = JSON.parse(body);
                const membre = membres.find(m => m.id === id);
                if (membre) {
                    Object.assign(membre, updated);
                    sendJson(res, membre);
                } else {
                    sendError(res, 'Membre non trouvé', 404);
                }
            } catch (e) {
                sendError(res, 'Invalid JSON', 400);
            }
        });
    } else if (method === 'DELETE') {
        // DELETE /membres/:id
        const id = parseInt(parts[2]);
        const idx = membres.findIndex(m => m.id === id);
        if (idx !== -1) {
            const deleted = membres.splice(idx, 1)[0];
            sendJson(res, { message: 'Membre supprimé', id: deleted.id });
        } else {
            sendError(res, 'Membre non trouvé', 404);
        }
    }
}

function handleEmprunts(req, res, method, parts) {
    if (method === 'GET') {
        if (parts.length === 3 && parts[2] === 'retard') {
            // GET /emprunts/retard/all
            const today = new Date().toISOString().split('T')[0];
            const overdue = emprunts.filter(e => !e.estRetourne && e.dateRetourPrevue < today);
            sendJson(res, overdue);
        } else if (parts.length >= 4 && parts[2] === 'membre') {
            // GET /emprunts/membre/:id
            const memberId = parseInt(parts[3]);
            const memberBorrows = emprunts.filter(e => e.idMembre === memberId && !e.estRetourne);
            sendJson(res, memberBorrows);
        } else {
            // GET /emprunts
            sendJson(res, emprunts);
        }
    } else if (method === 'POST') {
        // POST /emprunter or POST /rendre
        const action = parts[1];
        let body = '';
        req.on('data', chunk => body += chunk);
        req.on('end', () => {
            try {
                const data = JSON.parse(body);
                if (action === 'emprunter') {
                    // Create new borrow
                    const jeu = jeux.find(j => j.id === data.idJeu);
                    const membre = membres.find(m => m.id === data.idMembre);
                    if (!jeu || !membre) {
                        return sendError(res, 'Jeu ou Membre non trouvé', 404);
                    }
                    if (!jeu.disponible) {
                        return sendError(res, 'Jeu non disponible', 400);
                    }
                    const today = new Date();
                    const returnDate = new Date(today.getTime() + (data.joursDuree || 14) * 24 * 60 * 60 * 1000);
                    const newEmprunt = {
                        id: nextEmpruntId++,
                        idJeu: data.idJeu,
                        idMembre: data.idMembre,
                        dateEmprunt: today.toISOString().split('T')[0],
                        dateRetourPrevue: returnDate.toISOString().split('T')[0],
                        dateRetour: null,
                        estRetourne: false,
                        estEnRetard: false
                    };
                    emprunts.push(newEmprunt);
                    jeu.disponible = false;
                    if (!membre.empruntsActifs) membre.empruntsActifs = [];
                    membre.empruntsActifs.push(newEmprunt.id);
                    sendJson(res, newEmprunt, 201);
                } else if (action === 'rendre') {
                    // Return a game
                    const emprunt = emprunts.find(e => e.id === data.idEmprunt);
                    if (!emprunt) {
                        return sendError(res, 'Emprunt non trouvé', 404);
                    }
                    const jeu = jeux.find(j => j.id === emprunt.idJeu);
                    const membre = membres.find(m => m.id === emprunt.idMembre);
                    emprunt.dateRetour = new Date().toISOString().split('T')[0];
                    emprunt.estRetourne = true;
                    if (jeu) jeu.disponible = true;
                    if (membre) {
                        membre.empruntsActifs = membre.empruntsActifs.filter(id => id !== emprunt.id);
                    }
                    sendJson(res, emprunt);
                }
            } catch (e) {
                sendError(res, 'Invalid JSON', 400);
            }
        });
    }
}

// Main request handler
const server = http.createServer((req, res) => {
    // Handle CORS preflight
    if (req.method === 'OPTIONS') {
        setHeaders(res);
        res.end();
        return;
    }

    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;
    const parts = pathname.split('/').filter(p => p);
    const method = req.method;

    console.log(`[${new Date().toISOString()}] ${method} ${pathname}`);

    try {
        if (parts[0] === 'jeux') {
            handleJeux(req, res, method, parts);
        } else if (parts[0] === 'membres') {
            handleMembres(req, res, method, parts);
        } else if (parts[0] === 'emprunts' || parts[0] === 'emprunter' || parts[0] === 'rendre') {
            handleEmprunts(req, res, method, parts);
        } else {
            sendError(res, 'Endpoint not found', 404);
        }
    } catch (err) {
        console.error('Error:', err);
        sendError(res, 'Internal Server Error', 500);
    }
});

server.listen(PORT, () => {
    console.log('\n╔════════════════════════════════════════════╗');
    console.log('║  Board Game Library Mock Backend           ║');
    console.log('║  Version 1.0.0 (Node.js Mock)             ║');
    console.log('╚════════════════════════════════════════════╝\n');
    console.log(`✅ Server running on http://localhost:${PORT}`);
    console.log('\nAvailable Endpoints:');
    console.log('  GET/POST/PUT/DELETE /jeux');
    console.log('  GET/POST/PUT/DELETE /membres');
    console.log('  GET /emprunts');
    console.log('  POST /emprunter');
    console.log('  POST /rendre');
    console.log('  GET /emprunts/retard/all');
    console.log('  GET /emprunts/membre/:id\n');
});

process.on('SIGINT', () => {
    console.log('\n\n👋 Server shutting down...');
    process.exit(0);
});
