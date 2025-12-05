import { createClient } from '@supabase/supabase-js'
import dotenv from 'dotenv'
import path from 'path'
import fs from 'fs'

// Load .env from current folder or parent project root if present
const envPathsToTry = [
  path.resolve(process.cwd(), '.env'),
  path.resolve(process.cwd(), '..', '.env'),
  path.resolve(process.cwd(), '..', '..', '.env')
]

let loaded = false
for (const p of envPathsToTry) {
  if (fs.existsSync(p)) {
    dotenv.config({ path: p })
    loaded = true
    break
  }
}

if (!loaded) {
  // fallback to default behavior
  dotenv.config()
}

const SUPABASE_URL = process.env.SUPABASE_URL
const SUPABASE_ANON_KEY = process.env.SUPABASE_ANON_KEY

if (!SUPABASE_URL || !SUPABASE_ANON_KEY) {
  console.error('Missing SUPABASE_URL or SUPABASE_ANON_KEY in .env')
  process.exit(1)
}

const supabase = createClient(SUPABASE_URL, SUPABASE_ANON_KEY, {
  fetch: (...args) => import('node-fetch').then(({default: fetch}) => fetch(...args))
})

// Types (JSDoc) for clarity
/**
 * @typedef {Object} Jeu
 * @property {number} id
 * @property {string} titre
 * @property {string} categorie
 * @property {string} type
 * @property {boolean} disponible
 * @property {string} description
 * @property {string} auteur
 * @property {number} anneePublication
 * @property {number} [nbCartes]
 * @property {number} [complexite]
 * @property {number} [nbJoueurs]
 * @property {number} [duree]
 */

/**
 * @typedef {Object} Membre
 * @property {number} id
 * @property {string} nom
 * @property {string} email
 * @property {string} telephone
 * @property {string} adresse
 * @property {string} dateInscription
 * @property {number[]} [empruntsActifs]
 */

/**
 * @typedef {Object} Emprunt
 * @property {number} id
 * @property {number} idJeu
 * @property {number} idMembre
 * @property {string} dateEmprunt
 * @property {string} dateRetourPrevue
 * @property {string|null} dateRetour
 * @property {boolean} estRetourne
 */

// Utility logger
function log(level, ...args) {
  const ts = new Date().toISOString()
  console[level](`[${ts}]`, ...args)
}

// Generic helpers
async function handleResponse(promise) {
  const { data, error } = await promise
  if (error) {
    log('error', 'Supabase error:', error)
    throw error
  }
  return data
}

// Games
export async function getAllGames() {
  return handleResponse(supabase.from('jeux').select('*').order('id'))
}

export async function addGame(game) {
  return handleResponse(supabase.from('jeux').insert([game]).select().single())
}

export async function updateGame(id, data) {
  return handleResponse(supabase.from('jeux').update(data).eq('id', id).select().single())
}

export async function deleteGame(id) {
  return handleResponse(supabase.from('jeux').delete().eq('id', id).select().single())
}

// Members
export async function getMembers() {
  return handleResponse(supabase.from('membres').select('*').order('id'))
}

export async function addMember(member) {
  // Try inserting with provided fields; if a column is missing in remote schema,
  // attempt a minimal insert without optional fields such as dateInscription or empruntsActifs.
  try {
    return await handleResponse(supabase.from('membres').insert([member]).select().single())
  } catch (err) {
    log('warn', 'addMember initial insert failed, attempting fallback insert:', err.message || err)
    // Create fallback object with minimal required fields
    const fallback = {
      nom: member.nom || 'Unknown',
      email: member.email || null,
      telephone: member.telephone || null,
      adresse: member.adresse || null
    }
    try {
      return await handleResponse(supabase.from('membres').insert([fallback]).select().single())
    } catch (err2) {
      log('error', 'Fallback addMember failed:', err2)
      throw err2
    }
  }
}

// Borrowing
export async function borrowGame(idJeu, idMembre, dateEmprunt, dateRetourPrevue) {
  // transaction-like behavior: insert into emprunts and set jeu disponible = false, add emprunt id to membre
  // Supabase doesn't support multi-statement transactions via JS client for REST; use RPC or do sequentially with error handling
  try {
    const { data: empruntInsert, error: insertErr } = await supabase
      .from('emprunts')
      .insert([
        {
          idJeu,
          idMembre,
          dateEmprunt,
          dateRetourPrevue,
          dateRetour: null,
          estRetourne: false
        }
      ])
      .select()
      .single()

    if (insertErr) throw insertErr

    // mark game unavailable
    const { error: gameErr } = await supabase
      .from('jeux')
      .update({ disponible: false })
      .eq('id', idJeu)

    if (gameErr) throw gameErr

    // optionally update membre.empruntsActifs if such column exists as array
    try {
      await supabase
        .from('membres')
        .update({ empruntsActifs: supabase.rpc('array_append', { col: 'empruntsActifs', val: empruntInsert.id }) })
    } catch (e) {
      // ignore if not present; logging only
      log('warn', 'Could not append emprunt id to membre.empruntsActifs:', e.message || e)
    }

    return empruntInsert
  } catch (err) {
    log('error', 'borrowGame failed:', err)
    throw err
  }
}

export async function returnGame(empruntId) {
  try {
    const { data: emprunt, error: empruntErr } = await supabase
      .from('emprunts')
      .select('*')
      .eq('id', empruntId)
      .single()

    if (empruntErr) throw empruntErr

    const now = new Date().toISOString().split('T')[0]

    const { data: updated, error: updateErr } = await supabase
      .from('emprunts')
      .update({ dateRetour: now, estRetourne: true })
      .eq('id', empruntId)
      .select()
      .single()

    if (updateErr) throw updateErr

    // set game disponible = true
    const { error: gameErr } = await supabase
      .from('jeux')
      .update({ disponible: true })
      .eq('id', emprunt.idJeu)

    if (gameErr) throw gameErr

    // optionally remove from membre.empruntsActifs
    try {
      await supabase
        .from('membres')
        .update({ empruntsActifs: supabase.rpc('array_remove', { col: 'empruntsActifs', val: empruntId }) })
    } catch (e) {
      log('warn', 'Could not remove emprunt id from membre.empruntsActifs:', e.message || e)
    }

    return updated
  } catch (err) {
    log('error', 'returnGame failed:', err)
    throw err
  }
}

// Export raw client for advanced use
export { supabase }
