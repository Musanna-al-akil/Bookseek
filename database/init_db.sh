#!/bin/bash

# Exit on error
set -e

# Navigate to the project root directory
cd "$(dirname "$0")/.."

# Create a new database (or overwrite existing one)
sqlite3 bookseeks.db < database/schema.sql

echo "Database initialized successfully. File: bookseeks.db" 