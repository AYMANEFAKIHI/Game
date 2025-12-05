import React from 'react'
import { BookOpen, Users, Repeat2, Menu, X } from 'lucide-react'
import { useState } from 'react'

const Navigation = ({ currentPage, onPageChange }) => {
  const [mobileMenuOpen, setMobileMenuOpen] = useState(false)

  const navItems = [
    { id: 'games', label: 'Games', icon: BookOpen },
    { id: 'members', label: 'Members', icon: Users },
    { id: 'borrows', label: 'Borrows', icon: Repeat2 }
  ]

  return (
    <nav className="bg-gradient-to-r from-blue-600 to-purple-600 text-white shadow-lg">
      <div className="container flex justify-between items-center h-16">
        <div className="flex items-center gap-2">
          <BookOpen className="w-8 h-8" />
          <h1 className="text-2xl font-bold">Game Library</h1>
        </div>

        {/* Desktop Menu */}
        <div className="hidden md:flex gap-1">
          {navItems.map(({ id, label, icon: Icon }) => (
            <button
              key={id}
              onClick={() => onPageChange(id)}
              className={`flex items-center gap-2 px-4 py-2 rounded-md transition-colors ${
                currentPage === id
                  ? 'bg-white text-blue-600 font-semibold'
                  : 'hover:bg-blue-500 text-white'
              }`}
            >
              <Icon className="w-4 h-4" />
              {label}
            </button>
          ))}
        </div>

        {/* Mobile Menu Button */}
        <button
          onClick={() => setMobileMenuOpen(!mobileMenuOpen)}
          className="md:hidden text-white"
        >
          {mobileMenuOpen ? <X className="w-6 h-6" /> : <Menu className="w-6 h-6" />}
        </button>
      </div>

      {/* Mobile Menu */}
      {mobileMenuOpen && (
        <div className="md:hidden bg-blue-700 border-t border-blue-500">
          {navItems.map(({ id, label, icon: Icon }) => (
            <button
              key={id}
              onClick={() => {
                onPageChange(id)
                setMobileMenuOpen(false)
              }}
              className={`w-full flex items-center gap-2 px-4 py-3 text-left transition-colors ${
                currentPage === id
                  ? 'bg-white text-blue-600 font-semibold'
                  : 'hover:bg-blue-600 text-white'
              }`}
            >
              <Icon className="w-4 h-4" />
              {label}
            </button>
          ))}
        </div>
      )}
    </nav>
  )
}

export default Navigation
