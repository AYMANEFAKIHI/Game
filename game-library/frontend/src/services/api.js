import { supabase } from './supabase'

// Helper to mimic axios response structure { data: ... }
const wrap = async (promise) => {
  const { data, error } = await promise
  if (error) throw error
  return { data }
}

// ===================== JEUX ENDPOINTS =====================

export const gameService = {
  // Get all games
  getAllGames: () => wrap(supabase.from('jeux').select('*').order('id')),
  
  // Get available games
  getAvailableGames: () => wrap(supabase.from('jeux').select('*').eq('disponible', true)),
  
  // Get game by ID
  getGameById: (id) => wrap(supabase.from('jeux').select('*').eq('id', id).single()),
  
  // Create new game
  createGame: (gameData) => wrap(supabase.from('jeux').insert([gameData]).select().single()),
  
  // Update game
  updateGame: (id, gameData) => wrap(supabase.from('jeux').update(gameData).eq('id', id).select().single()),
  
  // Delete game
  deleteGame: (id) => wrap(supabase.from('jeux').delete().eq('id', id).select().single())
}

// ===================== MEMBRES ENDPOINTS =====================

export const memberService = {
  // Get all members
  getAllMembers: () => wrap(supabase.from('membres').select('*').order('id')),
  
  // Get member by ID
  getMemberById: (id) => wrap(supabase.from('membres').select('*').eq('id', id).single()),
  
  // Create new member
  createMember: (memberData) => wrap(supabase.from('membres').insert([memberData]).select().single()),
  
  // Update member
  updateMember: (id, memberData) => wrap(supabase.from('membres').update(memberData).eq('id', id).select().single()),
  
  // Delete member
  deleteMember: (id) => wrap(supabase.from('membres').delete().eq('id', id).select().single())
}

// ===================== EMPRUNTS ENDPOINTS =====================

export const borrowService = {
  // Get all borrows
  getAllBorrows: () => wrap(supabase.from('emprunts').select('*').order('id')),
  
  // Get active borrows for member
  getMemberBorrows: (memberId) => wrap(supabase.from('emprunts').select('*').eq('idMembre', memberId).eq('estRetourne', false)),
  
  // Get overdue borrows (requires RPC or client-side filter)
  getOverdueBorrows: () => wrap(supabase.rpc('get_emprunts_en_retard')),
  
  // Create borrow (complex logic: insert borrow + update game)
  borrowGame: async (borrowData) => {
    // 1. Insert borrow
    const { data: borrow, error: err1 } = await supabase.from('emprunts').insert([{
      idJeu: borrowData.idJeu,
      idMembre: borrowData.idMembre,
      dateEmprunt: borrowData.dateEmprunt,
      dateRetourPrevue: borrowData.dateRetourPrevue,
      estRetourne: false
    }]).select().single()
    if (err1) throw err1

    // 2. Update game availability
    const { error: err2 } = await supabase.from('jeux').update({ disponible: false }).eq('id', borrowData.idJeu)
    if (err2) console.error('Failed to update game availability', err2)

    return { data: borrow }
  },
  
  // Return game
  returnGame: async (borrowId) => {
    // 1. Get borrow to find game ID
    const { data: borrow, error: err0 } = await supabase.from('emprunts').select('*').eq('id', borrowId).single()
    if (err0) throw err0

    // 2. Update borrow
    const now = new Date().toISOString().split('T')[0]
    const { data: updated, error: err1 } = await supabase.from('emprunts').update({
      dateRetour: now,
      estRetourne: true
    }).eq('id', borrowId).select().single()
    if (err1) throw err1

    // 3. Update game availability
    const { error: err2 } = await supabase.from('jeux').update({ disponible: true }).eq('id', borrow.idJeu)
    if (err2) console.error('Failed to update game availability', err2)

    return { data: updated }
  }
}

export default supabase
