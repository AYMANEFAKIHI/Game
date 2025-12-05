#!/bin/bash

# Database setup script for Game Library
# Usage: bash setup_database.sh <host> <user> <password> <database>

HOST=${1:-localhost}
USER=${2:-root}
PASSWORD=${3:-}
DATABASE=${4:-game_library}

echo "Setting up Board Game Library database..."
echo "Host: $HOST"
echo "User: $USER"
echo "Database: $DATABASE"

# Run migration
mysql -h "$HOST" -u "$USER" -p"$PASSWORD" < 001_create_schema.sql

if [ $? -eq 0 ]; then
    echo "✓ Database setup completed successfully!"
else
    echo "✗ Database setup failed!"
    exit 1
fi
