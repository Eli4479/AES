# AES Encryption/Decryption Project

A comprehensive full-stack web application implementing Advanced Encryption Standard (AES) encryption and decryption functionality, featuring a C++ core, a modern Next.js (TypeScript) frontend, and a Python FastAPI backend.

## Overview

With this application, you can quickly and securely encrypt or decrypt your data in real time using trusted, industry-standard AES algorithms. Whether you're just starting to explore the world of cryptography or looking to protect your sensitive information, our user-friendly interface makes the process simple and accessible for everyone. Enjoy learning, experimenting, and keeping your data safe—all in one place!

## Features

- **C++ Algorithm**: Core AES encryption and decryption implemented in C++ for high performance
- **Python FastAPI Backend**: Secure RESTful API endpoints for encryption/decryption using FastAPI
- **Next.js Frontend**: Modern, intuitive web interface built with Next.js and TypeScript
- **Real-Time Processing**: Instant encryption and decryption with live results
- **Educational Resource**: Well-structured code demonstrating AES implementation principles


## 🚀 Technology Stack

- 🧠 **C++ Algorithm**: Blazing-fast AES encryption and decryption at the core
- ⚡ **Python FastAPI Backend**: Lightning-quick, secure RESTful API endpoints
- 🎨 **Next.js Frontend**: Sleek, modern web interface built with TypeScript & React

## AES Algorithm

Below is a visual representation of the AES encryption process:

![Diagram showing the AES Encryption Algorithm](https://github.com/user-attachments/assets/23e06f8c-0e65-419d-8f2b-1d5ed8369ef2)



## Installation

### Prerequisites

- Node.js (v14 or higher)
- Python 3.8 or higher
- C++ compiler (GCC or equivalent)
- Git


### Setup Instructions

1. **Clone the Repository**

```bash
git clone https://github.com/Eli4479/AES.git
cd AES
```

2. **Backend Setup**

```bash
cd backend
pip install fastapi uvicorn python-multipart cryptography
uvicorn main:app --reload
```

3. **Frontend Setup**

```bash
cd frontend
npm install
npm start
```

4. **C++ Core Setup**

```bash
cd ../AES
g++ -o aes_encrypt main.cpp
./aes_encrypt
```


## Usage

### Web Interface

1. Navigate to the frontend application (typically `http://localhost:3000`)
2. Enter your text in the input field
3. Provide an encryption key (must be 16 characters or less for AES-128)
4. Click "Encrypt" to encrypt your data or "Decrypt" to decrypt previously encrypted data
5. View results in real-time

### API Endpoints

The FastAPI backend provides the following endpoints:

- `GET /` - Health check or welcome endpoint
- `POST /aes` - Handles both encryption and decryption based on request parameters
- `GET /docs` - Interactive API documentation



## AES Implementation Details

### Algorithm Specifications

The project implements the Advanced Encryption Standard (AES) with the following specifications:

- **Block Size**: 128 bits (16 bytes)
- **Key Sizes**: 128 bits
- **Padding**: PKCS7 padding for data alignment


### Security Features

- **Strong Encryption**: Uses industry-standard AES algorithm with robust key management
- **Multiple Key Lengths**: Supports AES-128 for security requirements
- **Input Validation**: Comprehensive validation of keys, data, and parameters


## API Documentation

### Encryption Endpoint

```http
POST /aes
Content-Type: application/json

{
  "input": "Your plaintext/encrypted text here",
  "key": "your-encryption-key",
  "mode": "encrypt/decrypt"
}
```


### Response Format

```json
{
    "result": "Encrypted or decrypted text here",
    "status": "success"
}
```


## Development

### Project Structure

```
AES/
├── frontend/           # TypeScript/React frontend
├── backend/           # FastAPI Python backend
├── AES/              # C++ core implementation
├── .gitignore        # Git ignore rules
└── README.md         # Project documentation
```


### Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style

- **TypeScript**: Follow ESLint configuration with strict type checking
- **Python**: Adhere to PEP 8 style guidelines with FastAPI best practices
- **C++**: Use modern C++17 standards with clear naming conventions


## Testing

The project includes comprehensive testing suites:

- **Unit Tests**: Individual component testing for all encryption/decryption functions
- **Integration Tests**: End-to-end testing of API endpoints
- **Security Tests**: Validation of encryption strength and key management

## License

This project is open source and available under the MIT License.

## Acknowledgments

- Built using modern web development best Practices
- Implements AES standards as defined by NIST
- Utilizes FastAPI framework for high-performance API development
- Frontend developed with Next.js with TypeScript for type safety and maintainability


## Support

For questions, issues, or contributions, please visit the [GitHub repository](https://github.com/Eli4479/AES) or open an issue in the project's issue tracker.
