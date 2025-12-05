import React, { useState, useEffect } from 'react'
import { gameService, borrowService, memberService } from '../services/api'
import GameCard from '../components/GameCard'
import Modal from '../components/Modal'
import { Plus, Search, AlertCircle } from 'lucide-react'

const GamesPage = () => {
  const [games, setGames] = useState([])
  const [filteredGames, setFilteredGames] = useState([])
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)
  const [searchTerm, setSearchTerm] = useState('')
  const [filterAvailable, setFilterAvailable] = useState(null)
  const [showBorrowModal, setShowBorrowModal] = useState(false)
  const [selectedGame, setSelectedGame] = useState(null)
  const [members, setMembers] = useState([])
  const [borrowData, setBorrowData] = useState({ memberId: '', daysToReturn: 14 })
  const [showAddGameModal, setShowAddGameModal] = useState(false)
  const [newGame, setNewGame] = useState({ titre: '', categorie: '', type: 'Jeu', description: '', auteur: '' })

  useEffect(() => {
    loadGames()
    loadMembers()
  }, [])

  useEffect(() => {
    filterGames()
  }, [games, searchTerm, filterAvailable])

  const loadGames = async () => {
    try {
      setLoading(true)
      const response = await gameService.getAllGames()
      setGames(response.data)
      setError(null)
    } catch (err) {
      setError('Failed to load games')
      console.error(err)
    } finally {
      setLoading(false)
    }
  }

  const loadMembers = async () => {
    try {
      const response = await memberService.getAllMembers()
      setMembers(response.data)
    } catch (err) {
      console.error('Failed to load members:', err)
    }
  }

  const filterGames = () => {
    let filtered = games.filter(game => {
      const matchesSearch = game.titre.toLowerCase().includes(searchTerm.toLowerCase()) ||
                          game.categorie.toLowerCase().includes(searchTerm.toLowerCase())
      const matchesFilter = filterAvailable === null || game.disponible === filterAvailable
      return matchesSearch && matchesFilter
    })
    setFilteredGames(filtered)
  }

  const handleBorrowClick = (game) => {
    setSelectedGame(game)
    setShowBorrowModal(true)
  }

  const handleBorrowSubmit = async (e) => {
    e.preventDefault()
    if (!borrowData.memberId) {
      alert('Please select a member')
      return
    }

    try {
      const today = new Date()
      const returnDate = new Date(today.getTime() + borrowData.daysToReturn * 24 * 60 * 60 * 1000)
      const dateStr = returnDate.toISOString().split('T')[0]

      await borrowService.borrowGame({
        idJeu: selectedGame.id,
        idMembre: parseInt(borrowData.memberId),
        dateRetourPrevue: dateStr
      })

      alert('Game borrowed successfully!')
      setShowBorrowModal(false)
      setBorrowData({ memberId: '', daysToReturn: 14 })
      loadGames()
    } catch (err) {
      alert('Failed to borrow game: ' + err.response?.data?.error || err.message)
    }
  }

  const handleAddGame = async (e) => {
    e.preventDefault()
    try {
      const gamePayload = {
        ...newGame,
        disponible: true,
        anneePublication: newGame.anneePublication ? parseInt(newGame.anneePublication) : 0
      }

      if (newGame.type === 'JeuCarte') {
        gamePayload.nbCartes = parseInt(newGame.nbCartes) || 0
      } else if (newGame.type === 'JeuStrategie') {
        gamePayload.complexite = parseInt(newGame.complexite) || 5
      } else if (newGame.type === 'JeuSocieteClassique') {
        gamePayload.nbJoueurs = parseInt(newGame.nbJoueurs) || 2
        gamePayload.duree = parseInt(newGame.duree) || 30
      }

      await gameService.createGame(gamePayload)
      alert('Game added successfully!')
      setShowAddGameModal(false)
      setNewGame({ titre: '', categorie: '', type: 'Jeu', description: '', auteur: '' })
      loadGames()
    } catch (err) {
      alert('Failed to add game: ' + err.response?.data?.error || err.message)
    }
  }

  return (
    <div className="min-h-screen bg-gray-50 py-8">
      <div className="container">
        {/* Header */}
        <div className="flex justify-between items-center mb-8">
          <div>
            <h1 className="text-4xl font-bold text-gray-900">Board Games Catalog</h1>
            <p className="text-gray-600 mt-2">{filteredGames.length} games available</p>
          </div>
          <button
            onClick={() => setShowAddGameModal(true)}
            className="btn btn-success flex items-center gap-2"
          >
            <Plus className="w-5 h-5" />
            Add Game
          </button>
        </div>

        {/* Search and Filter */}
        <div className="bg-white rounded-lg shadow-md p-6 mb-8">
          <div className="flex flex-col md:flex-row gap-4">
            <div className="flex-1 relative">
              <Search className="absolute left-3 top-3 w-5 h-5 text-gray-400" />
              <input
                type="text"
                placeholder="Search games by title or category..."
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
                className="w-full pl-10 pr-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              />
            </div>
            <select
              value={filterAvailable === null ? '' : filterAvailable}
              onChange={(e) => setFilterAvailable(e.target.value === '' ? null : e.target.value === 'true')}
              className="px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            >
              <option value="">All Games</option>
              <option value="true">Available Only</option>
              <option value="false">Not Available</option>
            </select>
          </div>
        </div>

        {/* Games Grid */}
        {error && (
          <div className="bg-red-50 border border-red-200 rounded-lg p-4 mb-8 flex items-center gap-2 text-red-700">
            <AlertCircle className="w-5 h-5" />
            {error}
          </div>
        )}

        {loading ? (
          <div className="text-center py-12">
            <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mx-auto"></div>
            <p className="text-gray-600 mt-4">Loading games...</p>
          </div>
        ) : filteredGames.length === 0 ? (
          <div className="text-center py-12">
            <p className="text-gray-600 text-lg">No games found</p>
          </div>
        ) : (
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            {filteredGames.map(game => (
              <GameCard
                key={game.id}
                game={game}
                onBorrow={handleBorrowClick}
              />
            ))}
          </div>
        )}
      </div>

      {/* Borrow Modal */}
      <Modal
        isOpen={showBorrowModal}
        onClose={() => setShowBorrowModal(false)}
        title={`Borrow: ${selectedGame?.titre}`}
      >
        <form onSubmit={handleBorrowSubmit} className="space-y-4">
          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Select Member *
            </label>
            <select
              value={borrowData.memberId}
              onChange={(e) => setBorrowData({ ...borrowData, memberId: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              required
            >
              <option value="">Choose a member...</option>
              {members.map(member => (
                <option key={member.id} value={member.id}>
                  {member.nom}
                </option>
              ))}
            </select>
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Return in (days)
            </label>
            <input
              type="number"
              min="1"
              max="90"
              value={borrowData.daysToReturn}
              onChange={(e) => setBorrowData({ ...borrowData, daysToReturn: parseInt(e.target.value) })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>

          <div className="flex gap-2 pt-4">
            <button type="submit" className="flex-1 btn btn-success">
              Confirm Borrow
            </button>
            <button
              type="button"
              onClick={() => setShowBorrowModal(false)}
              className="flex-1 btn bg-gray-300 text-gray-700 hover:bg-gray-400"
            >
              Cancel
            </button>
          </div>
        </form>
      </Modal>

      {/* Add Game Modal */}
      <Modal
        isOpen={showAddGameModal}
        onClose={() => setShowAddGameModal(false)}
        title="Add New Game"
      >
        <form onSubmit={handleAddGame} className="space-y-4">
          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">Title *</label>
            <input
              type="text"
              value={newGame.titre}
              onChange={(e) => setNewGame({ ...newGame, titre: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              required
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">Category *</label>
            <input
              type="text"
              value={newGame.categorie}
              onChange={(e) => setNewGame({ ...newGame, categorie: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              required
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">Type *</label>
            <select
              value={newGame.type}
              onChange={(e) => setNewGame({ ...newGame, type: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            >
              <option value="Jeu">Basic Game</option>
              <option value="JeuCarte">Card Game</option>
              <option value="JeuStrategie">Strategy Game</option>
              <option value="JeuSocieteClassique">Party Game</option>
            </select>
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">Description</label>
            <textarea
              value={newGame.description}
              onChange={(e) => setNewGame({ ...newGame, description: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              rows="3"
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">Author</label>
            <input
              type="text"
              value={newGame.auteur}
              onChange={(e) => setNewGame({ ...newGame, auteur: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>

          {newGame.type === 'JeuCarte' && (
            <div>
              <label className="block text-sm font-medium text-gray-700 mb-1">Number of Cards</label>
              <input
                type="number"
                value={newGame.nbCartes || ''}
                onChange={(e) => setNewGame({ ...newGame, nbCartes: e.target.value })}
                className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              />
            </div>
          )}

          {newGame.type === 'JeuStrategie' && (
            <div>
              <label className="block text-sm font-medium text-gray-700 mb-1">Complexity (1-10)</label>
              <input
                type="number"
                min="1"
                max="10"
                value={newGame.complexite || ''}
                onChange={(e) => setNewGame({ ...newGame, complexite: e.target.value })}
                className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              />
            </div>
          )}

          {newGame.type === 'JeuSocieteClassique' && (
            <>
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">Number of Players</label>
                <input
                  type="number"
                  value={newGame.nbJoueurs || ''}
                  onChange={(e) => setNewGame({ ...newGame, nbJoueurs: e.target.value })}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                />
              </div>
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">Duration (minutes)</label>
                <input
                  type="number"
                  value={newGame.duree || ''}
                  onChange={(e) => setNewGame({ ...newGame, duree: e.target.value })}
                  className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
                />
              </div>
            </>
          )}

          <div className="flex gap-2 pt-4">
            <button type="submit" className="flex-1 btn btn-success">
              Add Game
            </button>
            <button
              type="button"
              onClick={() => setShowAddGameModal(false)}
              className="flex-1 btn bg-gray-300 text-gray-700 hover:bg-gray-400"
            >
              Cancel
            </button>
          </div>
        </form>
      </Modal>
    </div>
  )
}

export default GamesPage
