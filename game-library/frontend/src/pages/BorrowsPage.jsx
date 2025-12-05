import React, { useState, useEffect } from 'react'
import { borrowService, gameService, memberService } from '../services/api'
import { AlertCircle, CheckCircle, Clock, XCircle } from 'lucide-react'

const BorrowsPage = () => {
  const [borrows, setborrows] = useState([])
  const [games, setGames] = useState({})
  const [members, setMembers] = useState({})
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)
  const [filterStatus, setFilterStatus] = useState('active')

  useEffect(() => {
    loadData()
  }, [])

  const loadData = async () => {
    try {
      setLoading(true)
      const [borrowsRes, gamesRes, membersRes] = await Promise.all([
        borrowService.getAllBorrows(),
        gameService.getAllGames(),
        memberService.getAllMembers()
      ])

      setborrows(borrowsRes.data)
      
      const gamesMap = {}
      gamesRes.data.forEach(game => {
        gamesMap[game.id] = game
      })
      setGames(gamesMap)

      const membersMap = {}
      membersRes.data.forEach(member => {
        membersMap[member.id] = member
      })
      setMembers(membersMap)

      setError(null)
    } catch (err) {
      setError('Failed to load borrows')
      console.error(err)
    } finally {
      setLoading(false)
    }
  }

  const handleReturnGame = async (borrowId) => {
    try {
      await borrowService.returnGame(borrowId)
      alert('Game returned successfully!')
      loadData()
    } catch (err) {
      alert('Failed to return game: ' + err.response?.data?.error || err.message)
    }
  }

  const getFilteredBorrows = () => {
    if (filterStatus === 'active') {
      return borrows.filter(b => !b.estRetourne)
    } else if (filterStatus === 'overdue') {
      return borrows.filter(b => !b.estRetourne && b.estEnRetard)
    } else if (filterStatus === 'completed') {
      return borrows.filter(b => b.estRetourne)
    }
    return borrows
  }

  const getStatusIcon = (borrow) => {
    if (borrow.estRetourne) {
      return <CheckCircle className="w-5 h-5 text-green-600" />
    } else if (borrow.estEnRetard) {
      return <AlertCircle className="w-5 h-5 text-red-600" />
    } else {
      return <Clock className="w-5 h-5 text-blue-600" />
    }
  }

  const getStatusBadge = (borrow) => {
    if (borrow.estRetourne) {
      return <span className="badge badge-success">Returned</span>
    } else if (borrow.estEnRetard) {
      return <span className="badge badge-danger">Overdue</span>
    } else {
      return <span className="badge badge-warning">Active</span>
    }
  }

  return (
    <div className="min-h-screen bg-gray-50 py-8">
      <div className="container">
        {/* Header */}
        <div className="mb-8">
          <h1 className="text-4xl font-bold text-gray-900">Borrowing Management</h1>
          <p className="text-gray-600 mt-2">Total: {borrows.length} record(s)</p>
        </div>

        {/* Filter Tabs */}
        <div className="bg-white rounded-lg shadow-md p-4 mb-8">
          <div className="flex flex-wrap gap-2">
            {[
              { id: 'all', label: 'All', count: borrows.length },
              { id: 'active', label: 'Active', count: borrows.filter(b => !b.estRetourne).length },
              { id: 'overdue', label: 'Overdue', count: borrows.filter(b => !b.estRetourne && b.estEnRetard).length },
              { id: 'completed', label: 'Returned', count: borrows.filter(b => b.estRetourne).length }
            ].map(tab => (
              <button
                key={tab.id}
                onClick={() => setFilterStatus(tab.id)}
                className={`px-4 py-2 rounded-lg font-medium transition-colors ${
                  filterStatus === tab.id
                    ? 'bg-blue-600 text-white'
                    : 'bg-gray-100 text-gray-700 hover:bg-gray-200'
                }`}
              >
                {tab.label} ({tab.count})
              </button>
            ))}
          </div>
        </div>

        {/* Error Message */}
        {error && (
          <div className="bg-red-50 border border-red-200 rounded-lg p-4 mb-8 flex items-center gap-2 text-red-700">
            <AlertCircle className="w-5 h-5" />
            {error}
          </div>
        )}

        {/* Loading State */}
        {loading ? (
          <div className="text-center py-12">
            <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mx-auto"></div>
            <p className="text-gray-600 mt-4">Loading borrows...</p>
          </div>
        ) : getFilteredBorrows().length === 0 ? (
          <div className="text-center py-12">
            <p className="text-gray-600 text-lg">No borrows found</p>
          </div>
        ) : (
          <div className="overflow-x-auto">
            <table className="w-full bg-white rounded-lg shadow-md overflow-hidden">
              <thead className="bg-gray-100 border-b border-gray-200">
                <tr>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Game</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Member</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Borrow Date</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Due Date</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Return Date</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Status</th>
                  <th className="px-6 py-3 text-left text-sm font-semibold text-gray-900">Action</th>
                </tr>
              </thead>
              <tbody>
                {getFilteredBorrows().map(borrow => (
                  <tr key={borrow.id} className="border-b border-gray-200 hover:bg-gray-50 transition-colors">
                    <td className="px-6 py-4 text-sm text-gray-900">
                      {games[borrow.idJeu]?.titre || 'Game #' + borrow.idJeu}
                    </td>
                    <td className="px-6 py-4 text-sm text-gray-900">
                      {members[borrow.idMembre]?.nom || 'Member #' + borrow.idMembre}
                    </td>
                    <td className="px-6 py-4 text-sm text-gray-600">
                      {new Date(borrow.dateEmprunt).toLocaleDateString()}
                    </td>
                    <td className="px-6 py-4 text-sm text-gray-600">
                      {new Date(borrow.dateRetourPrevue).toLocaleDateString()}
                    </td>
                    <td className="px-6 py-4 text-sm text-gray-600">
                      {borrow.dateRetour ? new Date(borrow.dateRetour).toLocaleDateString() : '-'}
                    </td>
                    <td className="px-6 py-4 text-sm">
                      <div className="flex items-center gap-2">
                        {getStatusIcon(borrow)}
                        {getStatusBadge(borrow)}
                      </div>
                    </td>
                    <td className="px-6 py-4 text-sm">
                      {!borrow.estRetourne && (
                        <button
                          onClick={() => handleReturnGame(borrow.id)}
                          className="btn btn-success btn-sm"
                        >
                          Return
                        </button>
                      )}
                      {borrow.estRetourne && (
                        <span className="text-gray-500">Completed</span>
                      )}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
      </div>
    </div>
  )
}

export default BorrowsPage
