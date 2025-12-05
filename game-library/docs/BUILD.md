# Build Instructions

## Backend (C++ with Drogon)

### Requirements
- C++17 compiler (GCC 7+, Clang 5+, MSVC 2015+)
- CMake 3.5+
- Drogon Framework
- jsoncpp library
- MySQL development headers (future use)

### Ubuntu/Debian Installation

1. **Install dependencies**:
```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  git \
  libboost-all-dev \
  zlib1g-dev \
  openssl \
  libssl-dev \
  libc-ares-dev \
  uuid-dev
```

2. **Install Drogon Framework**:
```bash
git clone https://github.com/an-tao/drogon.git
cd drogon
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

3. **Build Backend**:
```bash
cd backend
mkdir build && cd build
cmake ..
make -j$(nproc)
```

4. **Run Backend**:
```bash
./game-library
# Server runs on http://localhost:8080
```

### Windows Installation (MSVC)

1. **Install Visual Studio 2019+** with C++ tools
2. **Install CMake** from cmake.org
3. **Install Drogon** (prebuilt or build from source)
4. **Build**:
```powershell
cd backend
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
msbuild game-library.sln
```

### macOS Installation

```bash
# Using Homebrew
brew install cmake boost openssl

# Install Drogon
git clone https://github.com/an-tao/drogon.git
cd drogon && mkdir build && cd build
cmake ..
make
sudo make install

# Build backend
cd ../../backend
mkdir build && cd build
cmake ..
make
```

---

## Frontend (React with Vite)

### Requirements
- Node.js 16+
- npm or yarn

### Installation

1. **Navigate to frontend directory**:
```bash
cd frontend
```

2. **Install dependencies**:
```bash
npm install
# or
yarn install
```

3. **Development mode**:
```bash
npm run dev
# Opens on http://localhost:5173
```

4. **Production build**:
```bash
npm run build
npm run preview
```

### Environment Configuration

Create `.env.local`:
```
VITE_API_URL=http://localhost:8080
VITE_API_TIMEOUT=5000
```

---

## Database Setup

### Prerequisites
- MySQL 5.7+
- Root access

### Manual Setup

1. **Create database and import schema**:
```bash
cd database/migrations
mysql -u root -p < 001_create_schema.sql
```

2. **Verify tables created**:
```sql
mysql -u root -p game_library
SHOW TABLES;
DESCRIBE jeux;
```

### Using Setup Script (Linux/macOS)

```bash
cd database/migrations
chmod +x setup_database.sh
./setup_database.sh localhost root your_password game_library
```

### Docker MySQL

```bash
docker run -d \
  --name game-library-mysql \
  -e MYSQL_ROOT_PASSWORD=root \
  -e MYSQL_DATABASE=game_library \
  -v ./database/migrations/001_create_schema.sql:/docker-entrypoint-initdb.d/init.sql \
  -p 3306:3306 \
  mysql:8.0
```

---

## Running the Full Stack

### Option 1: Local Development

**Terminal 1 - Backend**:
```bash
cd backend/build
./game-library
```

**Terminal 2 - Frontend**:
```bash
cd frontend
npm run dev
```

**Terminal 3 - MySQL** (if not running):
```bash
# Start MySQL service (Linux)
sudo systemctl start mysql

# Or Docker
docker start game-library-mysql
```

### Option 2: Docker Compose

Create `docker-compose.yml` in root directory:

```yaml
version: '3.8'

services:
  mysql:
    image: mysql:8.0
    container_name: game-library-mysql
    environment:
      MYSQL_ROOT_PASSWORD: root
      MYSQL_DATABASE: game_library
    volumes:
      - ./database/migrations/001_create_schema.sql:/docker-entrypoint-initdb.d/init.sql
      - mysql_data:/var/lib/mysql
    ports:
      - "3306:3306"
    healthcheck:
      test: ["CMD", "mysqladmin", "ping", "-h", "localhost"]
      interval: 10s
      timeout: 5s
      retries: 5

  backend:
    build:
      context: ./backend
      dockerfile: Dockerfile
    container_name: game-library-backend
    ports:
      - "8080:8080"
    depends_on:
      mysql:
        condition: service_healthy
    environment:
      DB_HOST: mysql
      DB_USER: root
      DB_PASSWORD: root
      DB_NAME: game_library

  frontend:
    build:
      context: ./frontend
      dockerfile: Dockerfile
    container_name: game-library-frontend
    ports:
      - "5173:5173"
    depends_on:
      - backend
    environment:
      VITE_API_URL: http://localhost:8080

volumes:
  mysql_data:
```

Create `backend/Dockerfile`:
```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libboost-all-dev zlib1g-dev openssl libssl-dev \
    libc-ares-dev uuid-dev

WORKDIR /app

# Install Drogon
RUN git clone https://github.com/an-tao/drogon.git /tmp/drogon && \
    cd /tmp/drogon && mkdir build && cd build && \
    cmake .. && make -j$(nproc) && make install && \
    rm -rf /tmp/drogon

COPY . .
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

EXPOSE 8080
CMD ["./build/game-library"]
```

Create `frontend/Dockerfile`:
```dockerfile
FROM node:18-alpine

WORKDIR /app

COPY package*.json ./
RUN npm install

COPY . .
RUN npm run build

EXPOSE 5173
CMD ["npm", "run", "preview"]
```

**Run**:
```bash
docker-compose up
```

---

## Troubleshooting

### Backend Build Issues

**CMake not found**:
```bash
# Ubuntu
sudo apt-get install cmake

# macOS
brew install cmake

# Windows
# Download from cmake.org
```

**Drogon not found**:
```bash
# Verify installation
drogon_create_app --help

# Add to CMake path if needed
export CMAKE_PREFIX_PATH=/usr/local/lib/drogon:$CMAKE_PREFIX_PATH
```

### Frontend Build Issues

**npm errors**:
```bash
# Clear cache
npm cache clean --force

# Reinstall
rm -rf node_modules package-lock.json
npm install
```

**Port already in use**:
```bash
# Change Vite port in vite.config.js
export default defineConfig({
  server: {
    port: 3000  // Change this
  }
})
```

### Database Connection Issues

**MySQL not running**:
```bash
# Check status
mysql -u root -p -e "SELECT 1"

# Start service
sudo systemctl start mysql
# or
docker start game-library-mysql
```

**Connection refused**:
```bash
# Check MySQL port
netstat -an | grep 3306

# Verify credentials in connection string
# Default: localhost:3306, user: root, password: root
```

### API Connection Issues

**CORS errors**:
- Add CORS headers to Drogon (see controllers)
- Ensure frontend URL matches CORS whitelist

**Timeout errors**:
- Check backend is running: `curl http://localhost:8080/jeux`
- Increase timeout in frontend `.env`

---

## Testing

### Backend Unit Tests

Install Google Test:
```bash
sudo apt-get install libgtest-dev
```

Create `backend/tests/test_jeu.cpp`:
```cpp
#include <gtest/gtest.h>
#include "../src/models/Jeu.h"

TEST(JeuTest, Construction) {
  Jeu jeu(1, "Test", "Category");
  EXPECT_EQ(jeu.getId(), 1);
  EXPECT_EQ(jeu.getTitre(), "Test");
}
```

Build and run:
```bash
cd backend/build
cmake -DBUILD_TESTS=ON ..
make
./game-library-tests
```

### Frontend Tests

```bash
cd frontend
npm install --save-dev vitest @testing-library/react

# Create tests in src/__tests__/
npm run test
```

### API Tests (Manual)

```bash
# Test game creation
curl -X POST http://localhost:8080/jeux \
  -H "Content-Type: application/json" \
  -d '{"titre":"Test","categorie":"Test","type":"Jeu","disponible":true}'

# Test member creation
curl -X POST http://localhost:8080/membres \
  -H "Content-Type: application/json" \
  -d '{"nom":"John Doe","email":"john@example.com"}'
```

Or use Postman/Insomnia for GUI testing.

---

## Performance Optimization

### Backend
- Use `-O3` compiler flag for optimizations
- Add connection pooling for database
- Implement caching layer (Redis)
- Use async/await for I/O operations

### Frontend
- Code splitting with dynamic imports
- Image optimization
- Lazy loading for routes
- Production build with minification

### Database
- Add indexes on frequently queried columns
- Use query optimization tools
- Monitor slow query logs

---

## Deployment Checklist

- [ ] Backend builds without errors
- [ ] Frontend builds without errors
- [ ] Database schema created
- [ ] Sample data inserted
- [ ] API endpoints tested
- [ ] Frontend connects to API
- [ ] CORS configured
- [ ] Security headers added
- [ ] Logging enabled
- [ ] Error handling verified
- [ ] Performance tested
- [ ] Backup strategy defined

---

**Build Guide**: January 2025
**Version**: 1.0.0
