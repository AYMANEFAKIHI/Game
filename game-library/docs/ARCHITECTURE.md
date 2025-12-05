# Board Game Library - Architecture Documentation

## 🏗️ System Overview

The Board Game Library is a full-stack web application built with:
- **Backend**: C++ with Drogon Framework (REST API)
- **Frontend**: React with TailwindCSS (Modern UI)
- **Database**: MySQL (Persistence layer)

## 📊 Architecture Layers

```
┌─────────────────────────────────────────────────────┐
│           PRESENTATION LAYER (React)                │
│  - Components                                        │
│  - Pages (Games, Members, Borrows)                   │
│  - Navigation & Modals                               │
└──────────────────┬──────────────────────────────────┘
                   │ HTTP/REST
┌──────────────────▼──────────────────────────────────┐
│         API LAYER (Drogon Controllers)              │
│  - JeuxController                                    │
│  - MembresController                                │
│  - EmpruntsController                                │
│  - Request Validation & Response Formatting          │
└──────────────────┬──────────────────────────────────┘
                   │ Object Calls
┌──────────────────▼──────────────────────────────────┐
│      BUSINESS LOGIC LAYER (Services)                │
│  - BibliothequeService                               │
│  - Business Rules & Validation                       │
│  - Transaction Management                            │
└──────────────────┬──────────────────────────────────┘
                   │ CRUD Operations
┌──────────────────▼──────────────────────────────────┐
│         DATA LAYER (Models & ORM)                   │
│  - Jeu, Membre, Emprunt Classes                     │
│  - JSON Serialization                                │
│  - In-memory storage (can be replaced with DB)       │
└──────────────────┬──────────────────────────────────┘
                   │ SQL/Database Protocol
┌──────────────────▼──────────────────────────────────┐
│      DATABASE LAYER (MySQL)                         │
│  - jeux, membres, emprunts tables                    │
│  - Views for complex queries                         │
│  - Foreign key constraints                           │
└─────────────────────────────────────────────────────┘
```

## 🎯 Component Responsibilities

### Presentation Layer (Frontend)

**Purpose**: User interface and interaction

**Components**:
1. **Navigation** - Top bar with page navigation
2. **GameCard** - Individual game display with expand capability
3. **Modal** - Reusable dialog for forms
4. **Pages**:
   - GamesPage: Display, search, and add games
   - MembersPage: Display members and add new ones
   - BorrowsPage: Manage borrowing records

**Technologies**:
- React 18.2
- TailwindCSS for styling
- Lucide React for icons
- Axios for API calls

### API Layer (Controllers)

**Purpose**: Handle HTTP requests and responses

**Controllers**:

1. **JeuxController**
   - Routes for game CRUD operations
   - Handles polymorhic game types
   - Returns JSON responses

2. **MembresController**
   - Routes for member management
   - Validates member data

3. **EmpruntsController**
   - Routes for borrowing operations
   - Ensures game availability
   - Tracks borrow/return dates

**Responsibilities**:
- Request validation
- Error handling
- Response formatting
- HTTP status codes

### Business Logic Layer (Services)

**Purpose**: Implement business rules and orchestrate operations

**BibliothequeService**:
- Manages all business operations
- Validates rules (e.g., can't borrow unavailable game)
- Maintains data consistency
- Singleton pattern ensures single instance

**Key Methods**:
```cpp
// Game operations
int ajouterJeu(Jeu* jeu);
vector<Jeu*> getJeuxDisponibles();
void updateJeu(int id, Jeu* jeu);

// Member operations
int ajouterMembre(const Membre& membre);
void deleteMembre(int id);

// Borrow operations
int emprunter(int idJeu, int idMembre, string dateRetour);
void rendre(int idEmprunt);
vector<Emprunt*> getEmpruntsEnRetard();
```

### Data Layer (Models)

**Purpose**: Represent domain objects

**Classes**:

1. **Jeu** (Abstract Base Class)
   - Common properties: titre, categorie, disponible
   - Virtual methods for polymorphism
   - JSON serialization

2. **JeuCarte** (Card Game)
   - Additional: nbCartes

3. **JeuStrategie** (Strategy Game)
   - Additional: complexite (1-10)

4. **JeuSocieteClassique** (Party Game)
   - Additional: nbJoueurs, duree

5. **Membre** (Library Member)
   - Tracks active borrows
   - Personal information

6. **Emprunt** (Borrow Record)
   - Links games to members
   - Tracks dates
   - Determines overdue status

**Methods**:
- Getters/Setters for properties
- Business logic (emprunter, restituer)
- JSON serialization (toJson, fromJson)

### Database Layer

**Purpose**: Persistent data storage

**Tables**:
1. **jeux** - All games with polymorphic fields
2. **membres** - Library members
3. **emprunts** - Borrow records with FK constraints

**Views**:
- vw_jeux_disponibles
- vw_emprunts_actifs
- vw_emprunts_en_retard
- vw_statistiques_membre

## 🔄 Data Flow Examples

### Creating a Game

```
Frontend                    API Layer              Business Logic           Database
├─ Form Input
├─ POST /jeux          ► JeuxController    ► BibliothequeService      ► Store in Map
│                         ├─ Validation       ├─ Create Jeu object
│                         ├─ Parse JSON       ├─ Assign ID
│                         └─ Call Service     └─ Return ID
├─ Response
│  {id: 1, msg: "..."}
└─ Update UI
```

### Borrowing a Game

```
Frontend                    API Layer              Business Logic           Database
├─ Select Member
├─ POST /emprunter    ► EmpruntsController  ► BibliothequeService
│                        ├─ Validation        ├─ Check game available
│                        ├─ Extract data      ├─ Update game.disponible
│                        └─ Call Service      ├─ Add to member.emprunts
│                                             ├─ Create Emprunt record
│                                             └─ Return borrow ID
├─ Response
│  {id: 1, msg: "..."}
└─ Reload Games
```

### Returning a Game

```
Frontend                    API Layer              Business Logic           Database
├─ Click Return       ► EmpruntsController  ► BibliothequeService
│                        ├─ Extract ID        ├─ Find Emprunt record
│                        └─ Call Service      ├─ Update game.disponible
│                                             ├─ Remove from member
│                                             ├─ Set return date
│                                             └─ Mark as returned
├─ Response
│  {msg: "..."}
└─ Reload Borrows
```

## 🔐 Security Considerations

### Input Validation
- All endpoints validate required fields
- Type checking for IDs
- String length limits in models
- Date format validation

### Error Handling
- Try-catch blocks in controllers
- Meaningful error messages (without exposing internals)
- Proper HTTP status codes
- JSON error responses

### Data Integrity
- Foreign key constraints in database
- Referential integrity checks
- Date validations (return > borrow)
- Duplicate prevention (email for members)

### Future Enhancements
- Authentication (JWT tokens)
- Authorization (role-based access)
- Rate limiting
- SQL injection prevention (prepared statements)
- CORS configuration

## 🎯 Design Patterns Applied

### 1. **Strategy Pattern**
Game types (Jeu, JeuCarte, JeuStrategie) represent different strategies
```cpp
class Jeu { virtual afficherInfo(); }
class JeuCarte : public Jeu { afficherInfo() override; }
```

### 2. **Singleton Pattern**
BibliothequeService maintains single instance
```cpp
static BibliothequeService& getService() {
  if (!gService) gService = new BibliothequeService();
  return *gService;
}
```

### 3. **Repository Pattern**
BibliothequeService provides data access abstraction
```cpp
int ajouterJeu(unique_ptr<Jeu> jeu);
Jeu* getJeu(int id);
void deleteJeu(int id);
```

### 4. **Dependency Injection**
Controllers receive service as dependency
```cpp
BibliothequeService& service = JeuxController::getService();
```

### 5. **Template Method Pattern** (implicit)
Base Jeu class defines structure, subclasses implement details
```cpp
virtual void afficherInfo();
virtual toJson(): Json::Value;
```

### 6. **Observer Pattern** (future)
Could notify UI of changes via WebSocket

## 📈 Scalability Considerations

### Current Implementation (In-Memory)
- Suitable for: Development, testing, small deployments
- Limitations: Data lost on restart, no persistence

### Database Integration
```cpp
// Current (in-memory)
std::map<int, std::unique_ptr<Jeu>> jeux;

// Future (database)
class DatabaseJeuRepository : public JeuRepository {
  vector<Jeu*> getAllJeux() override;
  // Execute SELECT * FROM jeux;
}
```

### Caching Strategy
- Add Redis for frequently accessed data
- Cache game availability list
- Invalidate on updates

### API Scaling
- Horizontal scaling via load balancer
- Connection pooling to database
- Pagination for large result sets
- Query optimization with indexes

## 🧪 Testing Strategy

### Unit Tests
```cpp
// Test Jeu class
TEST(JeuTest, emprunter) {
  Jeu jeu(1, "Test", "Category");
  jeu.emprunter();
  ASSERT_FALSE(jeu.estDisponible());
}
```

### Integration Tests
```cpp
// Test BibliothequeService
TEST(BibliothequeTest, empruntAndRender) {
  BibliothequeService service;
  int gameId = service.ajouterJeu(...);
  int memberId = service.ajouterMembre(...);
  int borrowId = service.emprunter(gameId, memberId, "2025-02-15");
  service.rendre(borrowId);
  // Verify game is available again
}
```

### API Tests
```bash
# Test endpoints
curl -X GET http://localhost:8080/jeux
curl -X POST http://localhost:8080/jeux -d '{...}'
```

### Frontend Tests
```javascript
// React component testing with Jest/React Testing Library
test('GameCard renders correctly', () => {
  const { getByText } = render(<GameCard game={mockGame} />);
  expect(getByText('Catan')).toBeInTheDocument();
});
```

## 📚 Configuration Management

### Backend Configuration
```cpp
// In main.cpp
drogon::app().addListener("0.0.0.0", 8080);
drogon::app().setThreadNum(4);
```

Future: Use config files
```json
{
  "server": {
    "host": "0.0.0.0",
    "port": 8080,
    "threads": 4
  },
  "database": {
    "host": "localhost",
    "user": "root",
    "password": "****"
  }
}
```

### Frontend Configuration
```javascript
// .env file
VITE_API_URL=http://localhost:8080
VITE_API_TIMEOUT=5000
```

## 🚀 Deployment

### Docker Deployment
```dockerfile
# Backend
FROM ubuntu:22.04
RUN apt-get install drogon jsoncpp
COPY backend /app
WORKDIR /app
RUN cmake . && make
EXPOSE 8080
CMD ["./game-library"]

# Frontend
FROM node:18
COPY frontend /app
WORKDIR /app
RUN npm install && npm run build
EXPOSE 5173
CMD ["npm", "run", "preview"]
```

### Environment Setup
- Development: local machine
- Testing: Docker containers
- Production: Cloud platform (AWS, Azure, GCP)

## 📞 Monitoring

### Logging
```cpp
LOG_INFO << "Game borrowed: " << idJeu;
LOG_ERROR << "Game not found: " << id;
```

### Metrics
- API response times
- Database query performance
- Error rates
- Active users/sessions

### Alerts
- High error rate (>5%)
- Database connection failures
- API downtime

## 🔄 CI/CD Pipeline

```
┌──────────────┐
│ Code Push    │
└──────┬───────┘
       │
       ▼
┌──────────────────────┐
│ Build & Compile      │
│ ├─ Backend (CMake)   │
│ └─ Frontend (Vite)   │
└──────┬───────────────┘
       │
       ▼
┌──────────────────────┐
│ Run Tests            │
│ ├─ Unit Tests        │
│ ├─ Integration Tests │
│ └─ API Tests         │
└──────┬───────────────┘
       │
       ▼
┌──────────────────────┐
│ Deploy               │
│ ├─ Docker Build      │
│ ├─ Registry Push     │
│ └─ K8s Deploy        │
└──────────────────────┘
```

---

**Architecture Document**: January 2025
**Version**: 1.0.0
**Last Updated**: January 2025
