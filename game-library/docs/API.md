# Board Game Library - API Documentation

## Base URL
```
http://localhost:8080
```

## Response Format
All responses are in JSON format with appropriate HTTP status codes.

### Success Response
```json
{
  "id": 1,
  "message": "Operation successful"
}
```

### Error Response
```json
{
  "error": "Error message describing what went wrong"
}
```

---

## 🎮 Games Endpoints

### GET /jeux
Retrieve all games in the library.

**Response**: `200 OK`
```json
[
  {
    "id": 1,
    "titre": "Catan",
    "categorie": "Strategy",
    "type": "JeuStrategie",
    "description": "Build settlements on an island",
    "auteur": "Klaus Teuber",
    "anneePublication": 1995,
    "disponible": true,
    "complexite": 7
  },
  {
    "id": 2,
    "titre": "Uno",
    "categorie": "Card Game",
    "type": "JeuCarte",
    "description": "Match colors and numbers",
    "disponible": true,
    "nbCartes": 108
  }
]
```

---

### POST /jeux
Create a new game.

**Request Body**:
```json
{
  "titre": "Monopoly",
  "categorie": "Board Game",
  "type": "JeuSocieteClassique",
  "description": "Property trading game",
  "auteur": "Charles Darrow",
  "anneePublication": 1935,
  "disponible": true,
  "nbJoueurs": 4,
  "duree": 180
}
```

**Response**: `201 Created`
```json
{
  "id": 3,
  "message": "Game created successfully"
}
```

**Errors**: `400 Bad Request` - Missing required fields (titre, categorie, type)

---

### GET /jeux/{id}
Retrieve a specific game by ID.

**Parameters**:
- `id` (path): Game ID

**Response**: `200 OK`
```json
{
  "id": 1,
  "titre": "Catan",
  "categorie": "Strategy",
  "type": "JeuStrategie",
  "complexite": 7,
  "description": "Build settlements on an island",
  "auteur": "Klaus Teuber",
  "anneePublication": 1995,
  "disponible": true
}
```

**Errors**: `404 Not Found` - Game doesn't exist

---

### PUT /jeux/{id}
Update an existing game.

**Parameters**:
- `id` (path): Game ID

**Request Body** (all fields optional):
```json
{
  "titre": "Catan Extended",
  "disponible": false,
  "complexite": 8
}
```

**Response**: `200 OK`
```json
{
  "message": "Game updated successfully"
}
```

**Errors**: 
- `404 Not Found` - Game doesn't exist
- `400 Bad Request` - Invalid data

---

### DELETE /jeux/{id}
Delete a game from the library.

**Parameters**:
- `id` (path): Game ID

**Response**: `200 OK`
```json
{
  "message": "Game deleted successfully"
}
```

**Errors**: `404 Not Found` - Game doesn't exist

---

### GET /jeux/disponibles/all
Retrieve all available (borrowable) games.

**Response**: `200 OK`
```json
[
  {
    "id": 1,
    "titre": "Catan",
    "categorie": "Strategy",
    "type": "JeuStrategie",
    "disponible": true,
    "complexite": 7
  }
]
```

---

## 👥 Members Endpoints

### GET /membres
Retrieve all library members.

**Response**: `200 OK`
```json
[
  {
    "id": 1,
    "nom": "Jean Dupont",
    "email": "jean@example.com",
    "telephone": "0612345678",
    "adresse": "123 Rue de la Paix, Paris",
    "dateInscription": "2025-01-15",
    "empruntsActifs": [1, 3]
  }
]
```

---

### POST /membres
Create a new library member.

**Request Body**:
```json
{
  "nom": "Marie Martin",
  "email": "marie@example.com",
  "telephone": "0687654321",
  "adresse": "456 Avenue de la République, Lyon"
}
```

**Response**: `201 Created`
```json
{
  "id": 2,
  "message": "Member created successfully"
}
```

**Errors**: `400 Bad Request` - Missing required field (nom)

---

### GET /membres/{id}
Retrieve a specific member.

**Parameters**:
- `id` (path): Member ID

**Response**: `200 OK`
```json
{
  "id": 1,
  "nom": "Jean Dupont",
  "email": "jean@example.com",
  "telephone": "0612345678",
  "adresse": "123 Rue de la Paix, Paris",
  "dateInscription": "2025-01-15",
  "empruntsActifs": [1, 3]
}
```

**Errors**: `404 Not Found` - Member doesn't exist

---

### PUT /membres/{id}
Update member information.

**Parameters**:
- `id` (path): Member ID

**Request Body** (all fields optional):
```json
{
  "email": "newemail@example.com",
  "telephone": "0698765432"
}
```

**Response**: `200 OK`
```json
{
  "message": "Member updated successfully"
}
```

**Errors**: `404 Not Found` - Member doesn't exist

---

### DELETE /membres/{id}
Delete a member from the library.

**Parameters**:
- `id` (path): Member ID

**Response**: `200 OK`
```json
{
  "message": "Member deleted successfully"
}
```

**Errors**: `404 Not Found` - Member doesn't exist

---

## 📚 Borrowing Endpoints

### GET /emprunts
Retrieve all borrowing records.

**Response**: `200 OK`
```json
[
  {
    "id": 1,
    "idJeu": 1,
    "idMembre": 1,
    "dateEmprunt": "2025-01-15",
    "dateRetourPrevue": "2025-02-15",
    "dateRetour": null,
    "estRetourne": false,
    "estEnRetard": false
  }
]
```

---

### POST /emprunter
Create a new borrowing record.

**Request Body**:
```json
{
  "idJeu": 1,
  "idMembre": 1,
  "dateRetourPrevue": "2025-02-15"
}
```

**Response**: `201 Created`
```json
{
  "id": 1,
  "message": "Game borrowed successfully"
}
```

**Errors**:
- `400 Bad Request` - Missing required fields or game not available
- `404 Not Found` - Game or member doesn't exist

---

### POST /rendre
Return a borrowed game.

**Request Body**:
```json
{
  "idEmprunt": 1
}
```

**Response**: `200 OK`
```json
{
  "message": "Game returned successfully"
}
```

**Errors**:
- `400 Bad Request` - Invalid emprunt ID
- `404 Not Found` - Borrow record doesn't exist

---

### GET /emprunts/membre/{idMembre}
Get active borrows for a specific member.

**Parameters**:
- `idMembre` (path): Member ID

**Response**: `200 OK`
```json
[
  {
    "id": 1,
    "idJeu": 1,
    "idMembre": 1,
    "dateEmprunt": "2025-01-15",
    "dateRetourPrevue": "2025-02-15",
    "dateRetour": null,
    "estRetourne": false,
    "estEnRetard": false
  }
]
```

---

### GET /emprunts/retard/all
Get all overdue borrowing records.

**Response**: `200 OK`
```json
[
  {
    "id": 2,
    "idJeu": 3,
    "idMembre": 2,
    "dateEmprunt": "2025-01-01",
    "dateRetourPrevue": "2025-01-31",
    "dateRetour": null,
    "estRetourne": false,
    "estEnRetard": true
  }
]
```

---

## HTTP Status Codes

| Code | Meaning | Usage |
|------|---------|-------|
| 200 | OK | Successful GET, PUT, DELETE |
| 201 | Created | Successful POST |
| 400 | Bad Request | Invalid input data |
| 404 | Not Found | Resource doesn't exist |
| 500 | Internal Server Error | Server error |

---

## Error Handling

All errors follow this format:
```json
{
  "error": "Detailed error message"
}
```

Common error messages:
- `"Missing required fields: ..."`
- `"Game is not available"`
- `"Game not found: {id}"`
- `"Member not found: {id}"`
- `"Borrow not found: {id}"`

---

## Request/Response Content-Type

All endpoints use:
```
Content-Type: application/json
```

---

## Example Workflow

1. **Create a Member**:
```bash
curl -X POST http://localhost:8080/membres \
  -H "Content-Type: application/json" \
  -d '{"nom": "Alice", "email": "alice@example.com"}'
```

2. **Get Available Games**:
```bash
curl http://localhost:8080/jeux/disponibles/all
```

3. **Borrow a Game**:
```bash
curl -X POST http://localhost:8080/emprunter \
  -H "Content-Type: application/json" \
  -d '{"idJeu": 1, "idMembre": 1, "dateRetourPrevue": "2025-02-15"}'
```

4. **Check Member's Borrows**:
```bash
curl http://localhost:8080/emprunts/membre/1
```

5. **Return a Game**:
```bash
curl -X POST http://localhost:8080/rendre \
  -H "Content-Type: application/json" \
  -d '{"idEmprunt": 1}'
```

---

## Rate Limiting

Currently no rate limiting is implemented. Add as needed in production.

## Authentication

Currently no authentication is implemented. Add JWT or OAuth2 for production use.

## Pagination

Pagination is not yet implemented. For large datasets, consider adding:
- `?page=1&limit=20` query parameters

---

**Last Updated**: January 2025
**Version**: 1.0.0
