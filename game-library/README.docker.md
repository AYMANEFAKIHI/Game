# Run with Docker

This repository includes Dockerfiles and a `docker-compose.yml` to build and run the frontend and the C++ backend inside containers. This avoids installing CMake or other build tools on the host.

Prerequisites:
- Docker and Docker Compose installed on your machine.

Quick start:

1. From the `game-library` project root, copy your `.env` values into the environment or export them for Docker Compose. For example (PowerShell):

```powershell
$env:SUPABASE_URL = "https://ymbvaeszbsflffzaxrvf.supabase.co"
$env:SUPABASE_ANON_KEY = "<your anon key>"
docker compose up --build
```

2. Frontend will be available at http://localhost:5173 and backend at http://localhost:8080 (proxied under `/api` by nginx).

Notes:
- The backend image builds Drogon from source and then builds the project; the build may take several minutes.
- The Compose file forwards SUPABASE_URL and SUPABASE_ANON_KEY from your environment into the backend container. Make sure they are set.
- If you prefer to run only the frontend or backend, use `docker compose up --build frontend` or `docker compose up --build backend`.
