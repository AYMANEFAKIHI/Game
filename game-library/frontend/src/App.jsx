import React, { useState } from 'react'
import Navigation from './components/Navigation'
import GamesPage from './pages/GamesPage'
import MembersPage from './pages/MembersPage'
import BorrowsPage from './pages/BorrowsPage'

const App = () => {
  const [currentPage, setCurrentPage] = useState('games')

  const renderPage = () => {
    switch (currentPage) {
      case 'games':
        return <GamesPage />
      case 'members':
        return <MembersPage />
      case 'borrows':
        return <BorrowsPage />
      default:
        return <GamesPage />
    }
  }

  return (
    <div className="min-h-screen bg-gray-50">
      <Navigation currentPage={currentPage} onPageChange={setCurrentPage} />
      <main>
        {renderPage()}
      </main>
      <footer className="bg-gray-900 text-white py-8 mt-12">
        <div className="container text-center">
          <p>&copy; 2025 Board Game Library. All rights reserved.</p>
          <p className="text-gray-400 mt-2">Built with React, TailwindCSS & C++ Drogon</p>
        </div>
      </footer>
    </div>
  )
}

export default App
