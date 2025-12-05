-- =====================================================
-- Board Game Library Database Schema
-- =====================================================

-- Create database
CREATE DATABASE IF NOT EXISTS game_library CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE game_library;

-- =====================================================
-- JEUX Table - Store all board games
-- =====================================================
CREATE TABLE IF NOT EXISTS jeux (
    id INT PRIMARY KEY AUTO_INCREMENT,
    titre VARCHAR(255) NOT NULL,
    type VARCHAR(50) NOT NULL,  -- 'Jeu', 'JeuCarte', 'JeuStrategie', 'JeuSocieteClassique'
    categorie VARCHAR(100) NOT NULL,
    description TEXT,
    auteur VARCHAR(255),
    anneePublication INT,
    disponible BOOLEAN DEFAULT TRUE,
    
    -- JeuCarte specific
    nbCartes INT NULL,
    
    -- JeuStrategie specific
    complexite INT NULL CHECK (complexite BETWEEN 1 AND 10),
    
    -- JeuSocieteClassique specific
    nbJoueurs INT NULL,
    duree INT NULL,
    
    -- Timestamps
    dateCreation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    dateModification TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Indexes for performance
    INDEX idx_titre (titre),
    INDEX idx_categorie (categorie),
    INDEX idx_disponible (disponible),
    INDEX idx_type (type)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- MEMBRES Table - Library members
-- =====================================================
CREATE TABLE IF NOT EXISTS membres (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nom VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE,
    telephone VARCHAR(20),
    adresse TEXT,
    dateInscription DATE DEFAULT CURDATE(),
    dateCreation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    dateModification TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Indexes
    INDEX idx_nom (nom),
    INDEX idx_email (email),
    INDEX idx_dateInscription (dateInscription)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- EMPRUNTS Table - Borrowing operations
-- =====================================================
CREATE TABLE IF NOT EXISTS emprunts (
    id INT PRIMARY KEY AUTO_INCREMENT,
    idJeu INT NOT NULL,
    idMembre INT NOT NULL,
    dateEmprunt DATE NOT NULL,
    dateRetourPrevue DATE NOT NULL,
    dateRetour DATE NULL,
    estRetourne BOOLEAN DEFAULT FALSE,
    
    -- Foreign keys
    CONSTRAINT fk_emprunt_jeu FOREIGN KEY (idJeu) REFERENCES jeux(id) ON DELETE CASCADE,
    CONSTRAINT fk_emprunt_membre FOREIGN KEY (idMembre) REFERENCES membres(id) ON DELETE CASCADE,
    
    -- Checks
    CHECK (dateRetourPrevue > dateEmprunt),
    CHECK (dateRetour IS NULL OR dateRetour >= dateEmprunt),
    
    -- Timestamps
    dateCreation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    dateModification TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    -- Indexes for performance
    INDEX idx_idJeu (idJeu),
    INDEX idx_idMembre (idMembre),
    INDEX idx_dateEmprunt (dateEmprunt),
    INDEX idx_estRetourne (estRetourne),
    INDEX idx_dateRetourPrevue (dateRetourPrevue)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- =====================================================
-- VIEWS for common queries
-- =====================================================

-- View: All available games
CREATE OR REPLACE VIEW vw_jeux_disponibles AS
SELECT * FROM jeux WHERE disponible = TRUE;

-- View: All active borrows
CREATE OR REPLACE VIEW vw_emprunts_actifs AS
SELECT e.*, j.titre as titre_jeu, m.nom as nom_membre
FROM emprunts e
JOIN jeux j ON e.idJeu = j.id
JOIN membres m ON e.idMembre = m.id
WHERE e.estRetourne = FALSE;

-- View: Overdue borrows
CREATE OR REPLACE VIEW vw_emprunts_en_retard AS
SELECT e.*, j.titre as titre_jeu, m.nom as nom_membre,
       DATEDIFF(CURDATE(), e.dateRetourPrevue) as jours_retard
FROM emprunts e
JOIN jeux j ON e.idJeu = j.id
JOIN membres m ON e.idMembre = m.id
WHERE e.estRetourne = FALSE AND CURDATE() > e.dateRetourPrevue;

-- View: Member borrow statistics
CREATE OR REPLACE VIEW vw_statistiques_membre AS
SELECT 
    m.id,
    m.nom,
    COUNT(CASE WHEN e.estRetourne = FALSE THEN 1 END) as emprunts_actifs,
    COUNT(CASE WHEN e.estRetourne = TRUE THEN 1 END) as emprunts_termines,
    COUNT(*) as total_emprunts
FROM membres m
LEFT JOIN emprunts e ON m.id = e.idMembre
GROUP BY m.id, m.nom;

-- =====================================================
-- SAMPLE DATA (optional)
-- =====================================================

-- Insert sample games
INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, nbCartes)
VALUES ('Uno', 'JeuCarte', 'Cartes', 'Un jeu de cartes amusant', 'Merle', 1971, TRUE, 108);

INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, nbCartes)
VALUES ('Magic', 'JeuCarte', 'Cartes', 'Le jeu de cartes collectif', 'Richard Garfield', 1993, TRUE, 60);

INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, complexite)
VALUES ('Chess', 'JeuStrategie', 'Stratégie', 'Le jeu d\'échecs classique', 'Inconnu', 600, TRUE, 8);

INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, complexite)
VALUES ('Catan', 'JeuStrategie', 'Stratégie', 'Coloniser une île', 'Klaus Teuber', 1995, TRUE, 7);

INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, nbJoueurs, duree)
VALUES ('Monopoly', 'JeuSocieteClassique', 'Société', 'Devenir le plus riche', 'Divers', 1935, TRUE, 4, 180);

INSERT INTO jeux (titre, type, categorie, description, auteur, anneePublication, disponible, nbJoueurs, duree)
VALUES ('Pictionary', 'JeuSocieteClassique', 'Société', 'Deviner des dessins', 'Pictionary Inc', 1985, TRUE, 4, 45);

-- Insert sample members
INSERT INTO membres (nom, email, telephone, adresse)
VALUES ('Jean Dupont', 'jean@example.com', '0612345678', '123 Rue de la Paix, Paris');

INSERT INTO membres (nom, email, telephone, adresse)
VALUES ('Marie Martin', 'marie@example.com', '0687654321', '456 Avenue de la République, Lyon');

INSERT INTO membres (nom, email, telephone, adresse)
VALUES ('Pierre Lefevre', 'pierre@example.com', '0698765432', '789 Boulevard de la Liberté, Marseille');
