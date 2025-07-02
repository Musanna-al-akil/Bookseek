# BookSeeks: Personal Books Management System

## Project Overview

BookSeeks is a command-line interface (CLI) based personal books management system built with C++. This application helps users organize, track, and manage their personal book collection efficiently.

## Run this code

```shell
cd src && g++ -o Bookseek main.cpp services/users.cpp services/book.cpp
```

```shell
./Bookseek
```

## Technical Stack

-   **Language**: C++
-   **Database**: binary file
-   **UI**: CLI with beautification libraries

## Features

### User Authentication

-   User registration system with username and password
-   Secure login mechanism
-   Password encryption

### Core Book Management

-   Add new books with details:
    -   Title, author(s), publisher
    -   Publication date
    -   Page count, genre/category
    -   Format (hardcover, paperback, e-book, audiobook)
-   Edit book information
-   Delete books
-   Search and filter books by various attributes
-   View detailed information about a specific book

### Book Organization

-   Create and manage custom bookshelves/collections
-   Sort books by different attributes
-   Organize books by genre, author, series, etc.

### Reading Progress Tracking

-   Mark books as: "To Read," "Currently Reading," "Completed", "Not Interested"
-   Set and track reading goals (weekly, monthly)
-   Record start and finish dates

### AI-Based Book Recommendation System

-   Generate personalized book recommendations based on reading history
-   Content-based filtering using book attributes (genre, author, themes)
-   Collaborative filtering based on similar user preferences
-   Discover new authors and genres based on reading patterns
-   "Books you might like" feature

### Personal Insights

-   Rate books on a customizable scale
-   Write and store personal reviews/notes
-   Track reading statistics (books read per month/year, pages read, etc.)

### Library Management

-   Track physical location of books (which shelf/room)
-   Manage book lending (lent to whom, when, return date)
-   Track borrowed books
-   Wishlist for books you want to acquire
-   Track book purchases (price, where purchased, date)

## Implementation Tasks

### Phase 1: Core Setup and Basic Functionality

-   [x] Set up the C++ project structure
-   [x] Implement SQLite3 database connectivity
-   [x] Create database schema for books and users
-   [x] Implement user registration and login system
-   [x] Create basic CLI interface
-   [x] Implement core CRUD operations for books

### Phase 2: Advanced Book Management

-   [x] Implement search and filter functionality
-   [ ] Add book categorization and tagging
-   [ ] Develop reading progress tracking
-   [ ] Create rating and review system

### Phase 3: Enhanced Features

-   [x] Add lending/borrowing tracking
-   [x] Create wishlist functionality
-   [ ] Implement reading goals and tracking
-   [ ] Design and implement AI recommendation algorithm

### Phase 4: Data and UI Enhancements

-   [ ] Enhance CLI with better visualization and colors
-   [ ] Add interactive menus and keyboard shortcuts

### Phase 5: Final Polishing

-   [ ] Optimize performance
-   [ ] Optimzize UI for better user experience
-   [ ] Perform testing and bug fixes

## Setup Instructions

1. Install required dependencies:
    - C++ compiler (GCC/G++ or Clang)
    - Required CLI libraries
2. Clone the repository
3. Build the project using CMake
4. Run the application

## Usage

The application will provide an interactive command-line interface with menus to navigate through different features. Detailed usage instructions will be provided as the project develops.
