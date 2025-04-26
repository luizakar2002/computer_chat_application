# Multi-Client Chat Application - using sockets and epoll in C++ & Qt for GUI

## Overview

This project is a **real-time chat application** that connects multiple computers over a network.  
It was built using **C++** for core networking logic and **Qt** for the client-side GUI.  
The system is architected around **sockets**, **epoll** (for efficient I/O multiplexing), and **modular design principles** to handle many clients simultaneously with minimal overhead.

---

## Architecture

### 1. Server

The server acts as the **central communication hub** between all connected clients.

Key components:
- **Socket Programming**: Accepts new TCP connections from clients.
- **epoll**: Efficiently monitors multiple client sockets for new messages or disconnections without busy-waiting.
- **Message Dispatching**: On receiving a message from any client, the server broadcasts it to all other connected clients.

The server requires only a **fixed IP address and port** (specified at startup), which it reserves and displays for clients to use.

---

### 2. Client

Each client is a standalone Qt application that can:

- Connect to the server using the server's **IP address** and **port**.
- Send messages to the server (which then relays them to all other clients).
- Display incoming messages from other participants in real time.
- Maintain a clean separation between **UI logic** and **networking logic**.

The client architecture ensures that the GUI remains responsive even when handling heavy network traffic.

---

## Why this matters

- **System-level thinking**: Designed for multiple simultaneous connections with high efficiency
