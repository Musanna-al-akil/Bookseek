-- BookSeeks Database Schema

-- Drop existing tables if they exist
DROP TABLE IF EXISTS books;
DROP TABLE IF EXISTS users;

-- Create users table
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL
);

-- Create books table
CREATE TABLE books (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    author TEXT NOT NULL,
    user_id INTEGER NOT NULL,
    status TEXT DEFAULT 'To Read',
    FOREIGN KEY (user_id) REFERENCES users(id)
);

-- Create indexes for faster lookups
CREATE INDEX idx_books_user_id ON books(user_id);
CREATE INDEX idx_books_title ON books(title);
CREATE INDEX idx_books_author ON books(author); 