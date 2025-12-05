import express from 'express'
import cors from 'cors'
import bodyParser from 'body-parser'
import dotenv from 'dotenv'
import path from 'path'
import { fileURLToPath } from 'url'

// Load .env from project root
const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)
dotenv.config({ path: path.resolve(__dirname, '..', '.env') })

import * as supa from '../supabase-js/index.js'

const app = express()
const PORT = process.env.SUPABASE_PROXY_PORT || 3002

app.use(cors())
app.use(bodyParser.json())

// Development: set a permissive Content-Security-Policy to avoid blocking DevTools and local connections
app.use((req, res, next) => {
  // Relaxed policy for local development only. Do NOT use in production as-is.
  res.setHeader(
    'Content-Security-Policy',
    "default-src 'self' 'unsafe-inline' 'unsafe-eval' data: blob: *; connect-src *; img-src * data:; style-src * 'unsafe-inline'; script-src * 'unsafe-inline' 'unsafe-eval'"
  )
  next()
})

// Respond to Chrome DevTools app-specific discovery to avoid 404 console noise
app.get('/.well-known/appspecific/com.chrome.devtools.json', (req, res) => {
  // Return 204 No Content to indicate presence without exposing data
  res.status(204).send()
})

// Games endpoints
app.get('/jeux', async (req, res) => {
  try {
    const data = await supa.getAllGames()
    res.json(data)
  } catch (err) {
    console.error('GET /jeux error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.post('/jeux', async (req, res) => {
  try {
    const data = await supa.addGame(req.body)
    res.status(201).json(data)
  } catch (err) {
    console.error('POST /jeux error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.put('/jeux/:id', async (req, res) => {
  try {
    const id = parseInt(req.params.id)
    const data = await supa.updateGame(id, req.body)
    res.json(data)
  } catch (err) {
    console.error('PUT /jeux/:id error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.delete('/jeux/:id', async (req, res) => {
  try {
    const id = parseInt(req.params.id)
    const data = await supa.deleteGame(id)
    res.json(data)
  } catch (err) {
    console.error('DELETE /jeux/:id error', err)
    res.status(500).json({ error: err.message || err })
  }
})

// Members endpoints
app.get('/membres', async (req, res) => {
  try {
    const data = await supa.getMembers()
    res.json(data)
  } catch (err) {
    console.error('GET /membres error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.post('/membres', async (req, res) => {
  try {
    const data = await supa.addMember(req.body)
    res.status(201).json(data)
  } catch (err) {
    console.error('POST /membres error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.put('/membres/:id', async (req, res) => {
  try {
    const id = parseInt(req.params.id)
    const { data } = await supa.supabase.from('membres').update(req.body).eq('id', id).select().single()
    res.json(data)
  } catch (err) {
    console.error('PUT /membres/:id error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.delete('/membres/:id', async (req, res) => {
  try {
    const id = parseInt(req.params.id)
    const { data } = await supa.supabase.from('membres').delete().eq('id', id).select().single()
    res.json(data)
  } catch (err) {
    console.error('DELETE /membres/:id error', err)
    res.status(500).json({ error: err.message || err })
  }
})

// Emprunts endpoints
app.get('/emprunts', async (req, res) => {
  try {
    const { data, error } = await supa.supabase.from('emprunts').select('*').order('id')
    if (error) throw error
    res.json(data)
  } catch (err) {
    console.error('GET /emprunts error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.post('/emprunter', async (req, res) => {
  try {
    const { idJeu, idMembre, dateEmprunt, dateRetourPrevue } = req.body
    const data = await supa.borrowGame(idJeu, idMembre, dateEmprunt, dateRetourPrevue)
    res.status(201).json(data)
  } catch (err) {
    console.error('POST /emprunter error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.post('/rendre', async (req, res) => {
  try {
    const { empruntId } = req.body
    const data = await supa.returnGame(empruntId)
    res.json(data)
  } catch (err) {
    console.error('POST /rendre error', err)
    res.status(500).json({ error: err.message || err })
  }
})

app.get('/emprunts/retard/all', async (req, res) => {
  try {
    const { data, error } = await supa.supabase.rpc('get_emprunts_en_retard')
    if (error) throw error
    res.json(data)
  } catch (err) {
    console.error('GET /emprunts/retard/all error', err)
    res.status(500).json({ error: err.message || err })
  }
})

// Start server
app.listen(PORT, () => {
  console.log(`Supabase proxy server listening on http://localhost:${PORT}`)
})
