import axios from 'axios'

const API_BASE = 'http://localhost:3002'

const api = axios.create({
  baseURL: API_BASE,
  headers: {
    'Content-Type': 'application/json'
  }
})

// Error handler
api.interceptors.response.use(
  response => response,
  error => {
    console.error('API Error:', error)
    throw error
  }
)

// ===================== JEUX ENDPOINTS =====================

export const gameService = {
  // Get all games
  getAllGames: () => api.get('/jeux'),
  
  // Get available games
  getAvailableGames: () => api.get('/jeux/disponibles/all'),
  
  // Get game by ID
  getGameById: (id) => api.get(`/jeux/${id}`),
  
  // Create new game
  createGame: (gameData) => api.post('/jeux', gameData),
  
  // Update game
  updateGame: (id, gameData) => api.put(`/jeux/${id}`, gameData),
  
  // Delete game
  deleteGame: (id) => api.delete(`/jeux/${id}`)
}

// ===================== MEMBRES ENDPOINTS =====================

export const memberService = {
  // Get all members
  getAllMembers: () => api.get('/membres'),
  
  // Get member by ID
  getMemberById: (id) => api.get(`/membres/${id}`),
  
  // Create new member
  createMember: (memberData) => api.post('/membres', memberData),
  
  // Update member
  updateMember: (id, memberData) => api.put(`/membres/${id}`, memberData),
  
  // Delete member
  deleteMember: (id) => api.delete(`/membres/${id}`)
}

// ===================== EMPRUNTS ENDPOINTS =====================

export const borrowService = {
  // Get all borrows
  getAllBorrows: () => api.get('/emprunts'),
  
  // Get active borrows for member
  getMemberBorrows: (memberId) => api.get(`/emprunts/membre/${memberId}`),
  
  // Get overdue borrows
  getOverdueBorrows: () => api.get('/emprunts/retard/all'),
  
  // Create borrow
  borrowGame: (borrowData) => api.post('/emprunter', borrowData),
  
  // Return game
  returnGame: (borrowId) => api.post('/rendre', { idEmprunt: borrowId })
}

export default api
