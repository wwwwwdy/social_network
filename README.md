# Social Network

Educational REST API service written in **C++** using the **U++ framework** and **Skylark** web framework.

The service provides basic functionality for a social network:

- user registration
- authentication
- getting user information

The application runs inside Docker and uses **PostgreSQL** as a database.

---

# Tech Stack

- C++
- U++ Framework
- Skylark
- PostgreSQL
- Docker
- Docker Compose
- jwt-cpp
- bcrypt

---

# Project Structure

```
social_network
│
├── social_network/           # main application source code
│
├── docs/
│   └── social_network_postman_collection.json
│
├── extras/                   # additional U++ package definitions
│
├── Dockerfile
├── docker-compose.yml
│
├── .env.example
├── .env
│
└── README.md
```

---

# Environment Variables

Create a `.env` file based on `.env.example`.

```
cp .env.example .env
```

Example `.env`:

```
DB_HOST=db
DB_NAME=social_network
DB_USER=postgres
DB_PASSWORD=postgres

APP_PORT=8000

JWT_SECRET=secret
JWT_ISSUER=auth_service
```

### Variables description

| Variable | Description |
|--------|--------|
| DB_HOST | PostgreSQL host |
| DB_NAME | Database name |
| DB_USER | Database user |
| DB_PASSWORD | Database password |
| APP_PORT | Application port |
| JWT_SECRET | Secret used to sign JWT tokens |
| JWT_ISSUER | JWT issuer name |

---

# Running the Project with Docker

## 1. Build and start containers

```
docker compose up --build
```

This command will start:

- **PostgreSQL database**
- **social_network application**

---

## 2. Services

### PostgreSQL

```
localhost:5432
```

Database name:

```
social_network
```

---

### Application

```
http://localhost:8000
```

---

# API Endpoints

### Register user

```
POST /user/register
```

Example request body:

```json
{
  "username": "john",
  "password": "password"
}
```

---

### Login

```
POST /login
```

Returns a **JWT token**.

---

### Get user

```
GET /user/get/:userId
```

Example:

```
GET /user/get/1
```

---

# Postman Collection

A ready-to-use Postman collection is available in:

```
docs/social_network_postman_collection.json
```

## Import

1. Open **Postman**
2. Click **Import**
3. Select the file above

The collection includes:

- `POST /user/register`
- `POST /login`
- `GET /user/get/:userId`

Default base URL:

```
http://127.0.0.1:8000
```

---

# Docker Build Details

The Docker build process performs the following steps:

1. Installs build dependencies
2. Clones **Ultimate++**
3. Installs **Skylark**
4. Clones external libraries:
   - jwt-cpp
   - bcrypt
5. Compiles the application using **umk**

Compilation command used in Docker:

```
/opt/upp/umk /app,/opt/upp/uppsrc,/opt/upp/upphub/Skylark social_network CLANG -bs /app/social_network
```

---

# Stopping the Project

```
docker compose down
```

Remove volumes:

```
docker compose down -v
```

