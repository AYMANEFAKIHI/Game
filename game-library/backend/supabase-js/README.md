# Supabase JS Client for Game Library

This module provides a small set of reusable functions to interact with the Supabase backend for the Game Library project.

Files:
- `index.js` : main client functions (getAllGames, addGame, updateGame, deleteGame, getMembers, addMember, borrowGame, returnGame)
- `test.js` : a small smoke test script

Usage:

1. Ensure `.env` is at project root with `SUPABASE_URL` and `SUPABASE_ANON_KEY`.
2. Install dependencies:

```bash
cd backend/supabase-js
npm install
```

3. Run smoke tests:

```bash
npm test
```

Notes:
- The client uses the official `@supabase/supabase-js` package and will throw detailed errors on failure.
- For production, consider running these calls server-side (not in a browser) or secure keys appropriately.
