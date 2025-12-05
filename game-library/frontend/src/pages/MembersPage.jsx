import React, { useState, useEffect } from 'react'
import { memberService } from '../services/api'
import { Plus, AlertCircle, Mail, Phone, MapPin, Calendar } from 'lucide-react'
import Modal from '../components/Modal'

const MembersPage = () => {
  const [members, setMembers] = useState([])
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)
  const [showAddModal, setShowAddModal] = useState(false)
  const [newMember, setNewMember] = useState({ nom: '', email: '', telephone: '', adresse: '' })

  useEffect(() => {
    loadMembers()
  }, [])

  const loadMembers = async () => {
    try {
      setLoading(true)
      const response = await memberService.getAllMembers()
      setMembers(response.data)
      setError(null)
    } catch (err) {
      setError('Failed to load members')
      console.error(err)
    } finally {
      setLoading(false)
    }
  }

  const handleAddMember = async (e) => {
    e.preventDefault()
    try {
      await memberService.createMember(newMember)
      alert('Member added successfully!')
      setShowAddModal(false)
      setNewMember({ nom: '', email: '', telephone: '', adresse: '' })
      loadMembers()
    } catch (err) {
      alert('Failed to add member: ' + err.response?.data?.error || err.message)
    }
  }

  const handleDeleteMember = async (id) => {
    if (window.confirm('Are you sure you want to delete this member?')) {
      try {
        await memberService.deleteMember(id)
        alert('Member deleted successfully!')
        loadMembers()
      } catch (err) {
        alert('Failed to delete member: ' + err.response?.data?.error || err.message)
      }
    }
  }

  return (
    <div className="min-h-screen bg-gray-50 py-8">
      <div className="container">
        {/* Header */}
        <div className="flex justify-between items-center mb-8">
          <div>
            <h1 className="text-4xl font-bold text-gray-900">Library Members</h1>
            <p className="text-gray-600 mt-2">{members.length} members registered</p>
          </div>
          <button
            onClick={() => setShowAddModal(true)}
            className="btn btn-success flex items-center gap-2"
          >
            <Plus className="w-5 h-5" />
            Add Member
          </button>
        </div>

        {/* Members Grid */}
        {error && (
          <div className="bg-red-50 border border-red-200 rounded-lg p-4 mb-8 flex items-center gap-2 text-red-700">
            <AlertCircle className="w-5 h-5" />
            {error}
          </div>
        )}

        {loading ? (
          <div className="text-center py-12">
            <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-600 mx-auto"></div>
            <p className="text-gray-600 mt-4">Loading members...</p>
          </div>
        ) : members.length === 0 ? (
          <div className="text-center py-12">
            <p className="text-gray-600 text-lg">No members found</p>
          </div>
        ) : (
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            {members.map(member => (
              <div key={member.id} className="card p-6">
                <h3 className="text-lg font-bold text-gray-900 mb-4">{member.nom}</h3>
                
                {member.email && (
                  <div className="flex items-center gap-2 text-gray-600 mb-2">
                    <Mail className="w-4 h-4" />
                    <a href={`mailto:${member.email}`} className="hover:text-blue-600">
                      {member.email}
                    </a>
                  </div>
                )}
                
                {member.telephone && (
                  <div className="flex items-center gap-2 text-gray-600 mb-2">
                    <Phone className="w-4 h-4" />
                    <a href={`tel:${member.telephone}`} className="hover:text-blue-600">
                      {member.telephone}
                    </a>
                  </div>
                )}
                
                {member.adresse && (
                  <div className="flex items-start gap-2 text-gray-600 mb-3">
                    <MapPin className="w-4 h-4 flex-shrink-0 mt-1" />
                    <span>{member.adresse}</span>
                  </div>
                )}
                
                {member.dateInscription && (
                  <div className="flex items-center gap-2 text-gray-500 text-sm">
                    <Calendar className="w-4 h-4" />
                    Joined: {new Date(member.dateInscription).toLocaleDateString()}
                  </div>
                )}

                {member.empruntsActifs && member.empruntsActifs.length > 0 && (
                  <div className="mt-4 pt-4 border-t border-gray-200">
                    <span className="badge badge-warning text-sm">
                      {member.empruntsActifs.length} active borrow(s)
                    </span>
                  </div>
                )}

                <button
                  onClick={() => handleDeleteMember(member.id)}
                  className="w-full mt-4 btn btn-danger btn-sm"
                >
                  Delete Member
                </button>
              </div>
            ))}
          </div>
        )}
      </div>

      {/* Add Member Modal */}
      <Modal
        isOpen={showAddModal}
        onClose={() => setShowAddModal(false)}
        title="Add New Member"
      >
        <form onSubmit={handleAddMember} className="space-y-4">
          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Full Name *
            </label>
            <input
              type="text"
              value={newMember.nom}
              onChange={(e) => setNewMember({ ...newMember, nom: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              required
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Email
            </label>
            <input
              type="email"
              value={newMember.email}
              onChange={(e) => setNewMember({ ...newMember, email: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Phone Number
            </label>
            <input
              type="tel"
              value={newMember.telephone}
              onChange={(e) => setNewMember({ ...newMember, telephone: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
            />
          </div>

          <div>
            <label className="block text-sm font-medium text-gray-700 mb-1">
              Address
            </label>
            <textarea
              value={newMember.adresse}
              onChange={(e) => setNewMember({ ...newMember, adresse: e.target.value })}
              className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"
              rows="3"
            />
          </div>

          <div className="flex gap-2 pt-4">
            <button type="submit" className="flex-1 btn btn-success">
              Add Member
            </button>
            <button
              type="button"
              onClick={() => setShowAddModal(false)}
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

export default MembersPage
