import React, { useState, useEffect } from 'react'
import { gameService } from '../services/api'
import { ChevronDown, Plus, Search } from 'lucide-react'

const GameCard = ({ game, onBorrow }) => {
  const [expanded, setExpanded] = useState(false)

  const getTypeLabel = (type) => {
    const labels = {
      'Jeu': 'Game',
      'JeuCarte': 'Card Game',
      'JeuStrategie': 'Strategy Game',
      'JeuSocieteClassique': 'Party Game'
    }
    return labels[type] || type
  }

  return (
    <div className="card p-4 mb-4 hover:shadow-xl transition-shadow">
      <div className="flex justify-between items-start">
        <div className="flex-1">
          <h3 className="text-lg font-bold text-gray-900">{game.titre}</h3>
          <p className="text-sm text-gray-600 mt-1">{game.categorie}</p>
          <div className="flex items-center gap-2 mt-2">
            <span className="badge badge-warning text-xs">{getTypeLabel(game.type)}</span>
            {game.disponible ? (
              <span className="badge badge-success text-xs">Available</span>
            ) : (
              <span className="badge badge-danger text-xs">Not Available</span>
            )}
          </div>
        </div>
        <button
          onClick={() => setExpanded(!expanded)}
          className="text-gray-400 hover:text-gray-600"
        >
          <ChevronDown className={`w-5 h-5 transition-transform ${expanded ? 'rotate-180' : ''}`} />
        </button>
      </div>

      {expanded && (
        <div className="mt-4 pt-4 border-t border-gray-200">
          {game.description && (
            <p className="text-sm text-gray-600 mb-3">{game.description}</p>
          )}
          {game.auteur && (
            <p className="text-sm text-gray-500 mb-1"><strong>Author:</strong> {game.auteur}</p>
          )}
          {game.anneePublication && (
            <p className="text-sm text-gray-500 mb-1"><strong>Year:</strong> {game.anneePublication}</p>
          )}

          {game.type === 'JeuCarte' && (
            <p className="text-sm text-gray-500 mb-1"><strong>Cards:</strong> {game.nbCartes}</p>
          )}
          {game.type === 'JeuStrategie' && (
            <p className="text-sm text-gray-500 mb-1"><strong>Complexity:</strong> {game.complexite}/10</p>
          )}
          {game.type === 'JeuSocieteClassique' && (
            <>
              <p className="text-sm text-gray-500 mb-1"><strong>Players:</strong> {game.nbJoueurs}</p>
              <p className="text-sm text-gray-500 mb-3"><strong>Duration:</strong> {game.duree} min</p>
            </>
          )}

          <button
            onClick={() => onBorrow(game)}
            disabled={!game.disponible}
            className={`mt-3 w-full ${game.disponible ? 'btn btn-primary' : 'btn bg-gray-400 text-white cursor-not-allowed'}`}
          >
            {game.disponible ? 'Borrow Game' : 'Not Available'}
          </button>
        </div>
      )}
    </div>
  )
}

export default GameCard
