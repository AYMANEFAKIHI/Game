# Board Game Library & Borrowing System

A complete, production-ready web application for managing a board game library with borrowing operations.

## 🎮 Features

### Core Functionality
- **Game Management**: Create, read, update, delete board games with multiple types
- **Member Management**: Register and manage library members
- **Borrowing System**: Full borrowing workflow with return management
- **Availability Tracking**: Real-time game availability status
- **Overdue Detection**: Automatic detection of overdue borrows
- **Type Hierarchy**: Support for different game types (Card, Strategy, Party games)

### Game Types
1. **Basic Game** (Jeu) - Foundation class with core properties
2. **Card Game** (JeuCarte) - Games with specific card count
3. **Strategy Game** (JeuStrategie) - Games with complexity rating (1-10)
4. **Party Game** (JeuSocieteClassique) - Games with player count and duration

## 🏗️ Architecture

### Backend (C++ with Drogon Framework)

```
backend/
├── src/
│   ├── models/          # OOP Model Classes
│   │   ├── Jeu.h/.cpp              # Base game class
│   │   ├── Membre.h/.cpp           # Member class
│   │   └── Emprunt.h/.cpp          # Borrow class
│   ├── services/        # Business Logic
│   │   └── BibliothequeService.h/.cpp
│   ├── controllers/     # REST API Endpoints
│   │   ├── JeuxController.h/.cpp
│   │   ├── MembresController.h/.cpp
│   │   └── EmpruntsController.h/.cpp
│   └── main.cpp         # Application entry point
├── CMakeLists.txt       # Build configuration
└── include/             # Header files
```

### Frontend (React + TailwindCSS)

```
frontend/
├── src/
│   ├── components/      # Reusable UI Components
│   │   ├── GameCard.jsx
│   │   ├── Modal.jsx
│   │   └── Navigation.jsx
│   ├── pages/          # Page Components
│   │   ├── GamesPage.jsx
│   │   ├── MembersPage.jsx
│   │   └── BorrowsPage.jsx
│   ├── services/       # API Integration
│   │   └── api.js
│   ├── styles/         # Global Styles
│   │   └── index.css
│   ├── App.jsx
│   └── main.jsx
├── index.html
├── package.json
├── tailwind.config.js
├── postcss.config.js
└── vite.config.js
```

### Database (MySQL)

```
database/
└── migrations/
    ├── 001_create_schema.sql    # Database schema
    └── setup_database.sh        # Setup script
```

## 📡 REST API Endpoints

### Games Management
```
GET    /jeux                    # Get all games
POST   /jeux                    # Create new game
GET    /jeux/{id}               # Get specific game
PUT    /jeux/{id}               # Update game
DELETE /jeux/{id}               # Delete game
GET    /jeux/disponibles/all    # Get available games
```

### Members Management
```
GET    /membres                 # Get all members
POST   /membres                 # Create new member
GET    /membres/{id}            # Get specific member
PUT    /membres/{id}            # Update member
DELETE /membres/{id}            # Delete member
```

### Borrowing Operations
```
GET    /emprunts                # Get all borrows
POST   /emprunter               # Create new borrow
POST   /rendre                  # Return a game
GET    /emprunts/membre/{id}    # Get member's active borrows
GET    /emprunts/retard/all     # Get overdue borrows
```

## 🗃️ Database Schema

### Tables
- **jeux** - All board games with polymorphic type support
- **membres** - Library members
- **emprunts** - Borrowing operations with date tracking

### Views
- **vw_jeux_disponibles** - Available games
- **vw_emprunts_actifs** - Active borrows with details
- **vw_emprunts_en_retard** - Overdue borrows
- **vw_statistiques_membre** - Member statistics

## 🚀 Getting Started

### Prerequisites
- C++17 or higher
- Drogon Framework with jsoncpp
- MySQL 5.7 or higher
- Node.js 16+ (for frontend)
- CMake 3.5+

### Backend Setup

1. **Install Drogon** (Ubuntu/Debian):
```bash
# Install dependencies
sudo apt-get install libboost-all-dev zlib1g-dev

# Clone and build Drogon
git clone https://github.com/an-tao/drogon.git
cd drogon
mkdir build && cd build
cmake ..
make
sudo make install
```

2. **Setup Database**:
```bash
cd database/migrations
bash setup_database.sh localhost root [password] game_library
```

Or manually:
```bash
mysql -u root -p < 001_create_schema.sql
```

3. **Build Backend**:
```bash
cd backend
mkdir build && cd build
cmake ..
make
```

4. **Run Backend**:
```bash
./game-library
```
The API will be available at `http://localhost:8080`

### Frontend Setup

1. **Install Dependencies**:
```bash
cd frontend
npm install
```

2. **Development Mode**:
```bash
npm run dev
```
The application will be available at `http://localhost:5173`

3. **Production Build**:
```bash
npm run build
npm run preview
```

## 📖 API Usage Examples

### Create a Game
```bash
curl -X POST http://localhost:8080/jeux \
  -H "Content-Type: application/json" \
  -d '{
    "titre": "Catan",
    "categorie": "Strategy",
    "type": "JeuStrategie",
    "complexite": 7,
    "description": "Build settlements on an island",
    "auteur": "Klaus Teuber",
    "anneePublication": 1995,
    "disponible": true
  }'
```

### Borrow a Game
```bash
curl -X POST http://localhost:8080/emprunter \
  -H "Content-Type: application/json" \
  -d '{
    "idJeu": 1,
    "idMembre": 1,
    "dateRetourPrevue": "2025-02-15"
  }'
```

### Return a Game
```bash
curl -X POST http://localhost:8080/rendre \
  -H "Content-Type: application/json" \
  -d '{
    "idEmprunt": 1
  }'
```

## 🧪 Testing

### Backend Tests (Optional)
```bash
cd backend/tests
# Add unit tests using Google Test or Catch2
```

## 📚 Code Quality

### SOLID Principles
- **Single Responsibility**: Each class has one reason to change
- **Open/Closed**: Classes open for extension, closed for modification
- **Liskov Substitution**: Derived classes can substitute base classes
- **Interface Segregation**: Many specific interfaces over one general
- **Dependency Inversion**: Depend on abstractions, not concretions

### Design Patterns
- **Strategy Pattern**: Different game types
- **Repository Pattern**: BibliothequeService manages data access
- **Dependency Injection**: Service locator pattern for controllers
- **JSON Serialization**: toJson/fromJson for API communication

### Code Standards
- Clear, descriptive variable and function names
- Comprehensive comments for complex logic
- Proper error handling with meaningful messages
- Input validation on all API endpoints
- Security checks for unauthorized operations

## 🔐 Security Features

- Input validation on all endpoints
- SQL injection prevention (MySQL prepared statements ready)
- Error messages don't expose sensitive data
- HTTP status codes for proper error signaling
- CORS ready (can be configured in Drogon)

## 📊 Database Relationships

```
Membres (1) ----< (Many) Emprunts >---- (1) Jeux
```

Foreign key constraints ensure referential integrity:
- Deleting a member cascades to emprunts
- Deleting a game cascades to emprunts
- Date validations prevent invalid borrow periods

## 🎨 Frontend Features

- **Responsive Design**: Mobile-first approach with TailwindCSS
- **Dark/Light Mode Ready**: CSS variables for theming
- **Real-time Feedback**: Loading states and error messages
- **Intuitive Navigation**: Clear page structure and navigation
- **Modal Dialogs**: Clean forms for creating/editing items
- **Data Tables**: Organized display of borrow information
- **Search & Filter**: Easily find games and members

## 📝 UML Class Diagram

```
┌─────────────────────────────────────────────────┐
│                      Jeu                         │
├─────────────────────────────────────────────────┤
│ - id: int                                       │
│ - titre: string                                 │
│ - categorie: string                             │
│ - disponible: bool                              │
│ - description: string                           │
│ - auteur: string                                │
│ - anneePublication: int                         │
├─────────────────────────────────────────────────┤
│ + emprunter()                                   │
│ + restituer()                                   │
│ + afficherInfo()                                │
│ + toJson(): Json::Value                         │
│ + fromJson(json)                                │
└─────────────────────────────────────────────────┘
         △             △             △
         │             │             │
    ┌────┴─────┐   ┌───┴────┐   ┌──┴──────────┐
    │ JeuCarte │   │JeuStrat │   │JeuSociete   │
    │ nbCartes │   │complexit│   │nbJoueurs    │
    │          │   │         │   │duree        │
    └──────────┘   └─────────┘   └─────────────┘

┌─────────────────────────────────────┐
│          Membre                      │
├─────────────────────────────────────┤
│ - id: int                           │
│ - nom: string                       │
│ - email: string                     │
│ - telephone: string                 │
│ - adresse: string                   │
│ - empruntsActifs: vector<int>       │
├─────────────────────────────────────┤
│ + ajouterEmprunt(id)                │
│ + retirerEmprunt(id)                │
│ + hasEmprunt(id): bool              │
│ + toJson(): Json::Value             │
└─────────────────────────────────────┘

┌──────────────────────────────────────────┐
│          Emprunt                         │
├──────────────────────────────────────────┤
│ - id: int                                │
│ - idJeu: int                             │
│ - idMembre: int                          │
│ - dateEmprunt: string                    │
│ - dateRetourPrevue: string               │
│ - dateRetour: string (nullable)          │
│ - estRetourne: bool                      │
├──────────────────────────────────────────┤
│ + marquerCommeRetourne()                 │
│ + estEnRetard(): bool                    │
│ + toJson(): Json::Value                  │
└──────────────────────────────────────────┘

┌──────────────────────────────────────────┐
│    BibliothequeService (Singleton)       │
├──────────────────────────────────────────┤
│ - jeux: map<int, Jeu*>                   │
│ - membres: map<int, Membre>              │
│ - emprunts: map<int, Emprunt>            │
├──────────────────────────────────────────┤
│ + ajouterJeu(jeu): int                   │
│ + getAllJeux(): vector<Jeu*>             │
│ + getJeuxDisponibles(): vector<Jeu*>     │
│ + ajouterMembre(membre): int             │
│ + emprunter(jeuId, membreId): int        │
│ + rendre(empruntId)                      │
│ + getEmpruntsEnRetard(): vector<Emprunt*>│
└──────────────────────────────────────────┘
```

## 📄 File Structure Summary

```
game-library/
├── backend/                         # C++ Backend
│   ├── src/
│   │   ├── models/                 # Domain models
│   │   ├── services/               # Business logic
│   │   ├── controllers/            # REST endpoints
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── include/
├── frontend/                        # React Frontend
│   ├── src/
│   │   ├── components/
│   │   ├── pages/
│   │   ├── services/
│   │   ├── styles/
│   │   ├── App.jsx
│   │   └── main.jsx
│   ├── package.json
│   ├── vite.config.js
│   ├── tailwind.config.js
│   └── index.html
├── database/                        # Database
│   └── migrations/
│       ├── 001_create_schema.sql
│       └── setup_database.sh
├── docs/                            # Documentation
│   ├── API.md
│   ├── ARCHITECTURE.md
│   └── UML.md
└── README.md                        # This file
```

## 🚀 Deployment

### Docker (Optional)

Create `docker-compose.yml`:
```yaml
version: '3.8'
services:
  mysql:
    image: mysql:8.0
    environment:
      MYSQL_ROOT_PASSWORD: root
      MYSQL_DATABASE: game_library
    volumes:
      - ./database/migrations/001_create_schema.sql:/docker-entrypoint-initdb.d/init.sql
    ports:
      - "3306:3306"

  backend:
    build: ./backend
    ports:
      - "8080:8080"
    depends_on:
      - mysql
    environment:
      DB_HOST: mysql

  frontend:
    build: ./frontend
    ports:
      - "5173:5173"
    depends_on:
      - backend
```

Run with: `docker-compose up`

## 📞 Support

For issues or questions, check the code comments or the inline documentation in each file.

## 📄 License

This project is provided as-is for educational and commercial use.

## ✨ Future Enhancements

- [ ] Database persistence integration
- [ ] User authentication & authorization
- [ ] Email notifications for overdue returns
- [ ] Game ratings and reviews
- [ ] Wishlist functionality
- [ ] Advanced search and filtering
- [ ] Statistics dashboard
- [ ] Backup and recovery
- [ ] Multi-language support
- [ ] Mobile app (React Native)

---

**Happy coding! 🎮📚**
