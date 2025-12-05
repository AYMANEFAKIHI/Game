# 🚀 Quick Start Guide - Board Game Library

## 5-Minute Setup

### Prerequisites
- Node.js 16+ and npm
- C++17 compiler
- MySQL 5.7+
- CMake 3.5+

### Step 1: Clone and Navigate (30 seconds)
```bash
cd game-library
```

### Step 2: Setup Database (2 minutes)
```bash
# Option A: MySQL command line
mysql -u root -p < database/migrations/001_create_schema.sql

# Option B: Using script (Linux/macOS)
cd database/migrations
bash setup_database.sh localhost root your_password game_library
```

### Step 3: Build Backend (2 minutes)
```bash
cd backend
mkdir build && cd build
cmake ..
make -j$(nproc)
./game-library &
```

### Step 4: Build Frontend (1 minute)
```bash
cd frontend
npm install
npm run dev
```

### Step 5: Open in Browser
- Frontend: **http://localhost:5173**
- API: **http://localhost:8080**

**Done!** 🎉 You should see the game library dashboard.

---

## 📖 What You Can Do Now

### 1. Add a Game
- Click "Add Game" button
- Fill in details (title, category, type)
- Choose game type: Basic, Card, Strategy, or Party
- Add optional info (description, author, year)
- Click "Confirm"

### 2. Add a Member
- Go to "Members" tab
- Click "Add Member"
- Enter name, email, phone, address
- Click "Add Member"

### 3. Borrow a Game
- Go to "Games" tab
- Find an available game (green "Available" badge)
- Click game card to expand
- Click "Borrow Game"
- Select member from dropdown
- Choose return period (days)
- Click "Confirm Borrow"

### 4. Return a Game
- Go to "Borrows" tab
- Find active borrow (yellow "Active" badge)
- Click "Return" button
- Game becomes available again

### 5. Track Overdue
- Go to "Borrows" tab
- Click "Overdue" filter
- See all late returns (red "Overdue" badge)

---

## 🧪 Test API Directly

### Get All Games
```bash
curl http://localhost:8080/jeux
```

### Create a Game
```bash
curl -X POST http://localhost:8080/jeux \
  -H "Content-Type: application/json" \
  -d '{
    "titre": "Catan",
    "categorie": "Strategy",
    "type": "JeuStrategie",
    "complexite": 7,
    "auteur": "Klaus Teuber",
    "anneePublication": 1995,
    "disponible": true
  }'
```

### Get Available Games
```bash
curl http://localhost:8080/jeux/disponibles/all
```

### Create a Member
```bash
curl -X POST http://localhost:8080/membres \
  -H "Content-Type: application/json" \
  -d '{
    "nom": "Alice Johnson",
    "email": "alice@example.com",
    "telephone": "555-1234",
    "adresse": "123 Main St"
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

---

## 🐛 Troubleshooting

### Backend won't start
```bash
# Check if port 8080 is in use
lsof -i :8080  # Linux/macOS
netstat -an | grep 8080  # Windows

# Kill process and restart
kill -9 <PID>
./game-library
```

### Frontend won't start
```bash
# Clear cache and reinstall
cd frontend
rm -rf node_modules package-lock.json
npm install
npm run dev
```

### Database connection error
```bash
# Verify MySQL is running
mysql -u root -p -e "SELECT 1"

# Check schema exists
mysql -u root -p game_library -e "SHOW TABLES"
```

### API returns 404
- Verify backend is running on port 8080
- Check API documentation in `/docs/API.md`
- Test with: `curl http://localhost:8080/jeux`

---

## 📊 Sample Data

The database comes with:
- **6 Games**: Uno, Magic, Chess, Catan, Monopoly, Pictionary
- **3 Members**: Jean Dupont, Marie Martin, Pierre Lefevre
- **0 Borrows**: Start fresh and add your own

---

## 🎮 Feature Tour

### Games Page
- **Search** games by title or category
- **Filter** by availability status
- **View** detailed game information
- **Add** new games with type-specific fields
- **Borrow** available games

### Members Page
- **View** all registered members
- **Add** new members
- **See** member's active borrows
- **Delete** members if needed

### Borrows Page
- **View** all borrow records
- **Filter** by status (active, overdue, completed)
- **Return** borrowed games
- **Track** overdue items

---

## 📚 Documentation

For more details, see:
- **README.md** - Full project overview
- **docs/API.md** - Complete API reference
- **docs/ARCHITECTURE.md** - System design
- **docs/UML.md** - Class diagrams
- **docs/BUILD.md** - Advanced setup

---

## 🔧 Development

### Modify the Backend
1. Edit files in `backend/src/`
2. Rebuild: `cd backend/build && cmake .. && make`
3. Restart: `./game-library`

### Modify the Frontend
1. Edit files in `frontend/src/`
2. Changes auto-reload in dev mode
3. Rebuild for production: `npm run build`

### Add Database Features
1. Modify schema in `database/migrations/`
2. Update C++ models accordingly
3. Restart backend and recreate database

---

## 🚢 Production Deployment

### Using Docker
```bash
docker-compose up
```

### Manual Deployment
1. Build backend in Release mode
2. Build frontend production build
3. Deploy to web server (Nginx, Apache)
4. Configure MySQL on production server
5. Setup SSL/TLS for HTTPS
6. Configure firewall rules

See `docs/BUILD.md` for detailed instructions.

---

## 💡 Tips & Tricks

### Keyboard Shortcuts
- None yet, but could be added!

### Search Tips
- Search works on title and category
- Search is case-insensitive
- Partial matches work

### Filter Tips
- Use "Available" filter to see borrowable games
- Use "Overdue" to find late returns
- Use "All" to see everything

### Performance Tips
- Database queries are optimized with indexes
- Frontend loads incrementally
- Use pagination for large datasets (coming soon)

---

## 🎓 Learning

This project teaches:
- **C++**: Modern C++17 with smart pointers, STL
- **Web Services**: REST API design with Drogon
- **React**: Functional components, hooks, state management
- **Databases**: MySQL, normalization, foreign keys
- **Architecture**: SOLID principles, design patterns
- **Full-Stack**: Connecting all layers

---

## 🆘 Getting Help

### Common Issues
1. **Port already in use** - Change port in config
2. **Database exists** - Drop and recreate: `DROP DATABASE game_library;`
3. **Dependencies missing** - See `docs/BUILD.md`

### Check Logs
- Backend: Console output shows errors
- Frontend: Browser console (F12)
- Database: MySQL error log

---

## ✨ Next Steps

1. ✅ **Run it** - Follow setup steps above
2. 🎮 **Play with it** - Add games, members, borrows
3. 📚 **Learn from it** - Read the code and docs
4. 🔧 **Extend it** - Add your own features
5. 🚀 **Deploy it** - Put it live!

---

## 🎯 What's Next?

### Easy Additions
- [ ] Game ratings/reviews
- [ ] Member wishlist
- [ ] Email notifications
- [ ] Game photos/images
- [ ] Search by author
- [ ] Member statistics

### Advanced Features
- [ ] User authentication
- [ ] Admin dashboard
- [ ] Late fee calculation
- [ ] Game reservations
- [ ] Mobile app (React Native)
- [ ] Real-time notifications

---

**Happy gaming! 🎲📚**

For detailed docs, see the `/docs` folder.

---

**Quick Start Guide Version**: 1.0.0  
**Last Updated**: January 2025
