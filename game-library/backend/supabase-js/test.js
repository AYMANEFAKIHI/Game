import {
  getAllGames,
  addGame,
  updateGame,
  deleteGame,
  getMembers,
  addMember,
  borrowGame,
  returnGame,
  supabase
} from './index.js'

async function runSmoke() {
  try {
    console.log('Testing Supabase client smoke tests...')

    const games = await getAllGames()
    console.log('getAllGames:', Array.isArray(games) ? `${games.length} items` : games)

    const newMember = await addMember({ nom: 'Test User', email: 'test@example.com', telephone: '', adresse: '', dateInscription: new Date().toISOString().split('T')[0] })
    console.log('addMember:', newMember)

    // Clean up (try to delete created member) - ignore errors
    try {
      await supabase.from('membres').delete().eq('id', newMember.id)
      console.log('Cleanup member deleted')
    } catch (e) {
      console.warn('Cleanup failed:', e.message || e)
    }

    console.log('Smoke tests completed')
  } catch (err) {
    console.error('Smoke test error:', err)
    process.exit(2)
  }
}

runSmoke()
