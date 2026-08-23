# Durak

A multiplayer card game written in modern C++.

This project is developed from scratch as a personal C++ project. The main focus is on practicing modern C++, software architecture, networking, event-driven communication, and UI development.

## Technologies

* **C++23**
* **SFML**
* **STL**
* **Visual Studio**
* **Git / GitHub**

## Project Structure

The source code is organized into several modules:

```text
source/
├── core/
├── game/
├── network/
├── protocol/
├── states/
├── ui/
├── resources/
├── testData/
└── utils/
```

### Core

Contains functionality shared across different parts of the application.

The core module includes:

* Event system
* Input handling
* Scene management
* Client context
* Animation support
* Debug logging
* Common concepts and utilities

The event system allows different parts of the application to communicate without being directly coupled to each other.

### Game

Contains the game and lobby logic.

```text
game/
├── events/
├── scenes/
├── CardData
├── ClassicRules
├── Game
├── GameLogic
├── LobbyLogic
├── ClientLobbyLogic
├── ClientPlayerData
├── PlayerData
└── Rules
```

Game rules are separated from the general game logic through the `Rules` abstraction, with `ClassicRules` providing the implementation for the classic Durak rules.

Lobby-related functionality is separated from the game logic and includes lobby state, player data, and client-side lobby logic.

### Network

Contains the client and server networking layer.

```text
network/
├── Client
├── Server
├── ClientSession
├── ClientModel
├── Packet
├── PacketFactory
├── PendingPacket
├── ClientPacketDispatcher
├── ServerPacketDispatcher
└── ConnectionState
```

The networking layer is responsible for:

* Client/server communication
* Connection management
* Client sessions
* Packet creation
* Packet dispatching
* Pending packet management
* Connection state management

### Protocol

Contains data structures and definitions used for communication between the client and server.

The protocol is separated from the networking implementation so that the data being transferred is independent from the connection management itself.

### UI

A custom UI system built on top of SFML.

```text
ui/
├── core/
├── elements/
├── layers/
├── panels/
├── primitives/
├── state/
├── styles/
├── widgets/
├── ButtonBuilder
├── LobbyPlayerView
└── UIController
```

The UI is built around a hierarchy of reusable components.

The system separates basic UI elements from interactive elements and organizes the interface using layers, panels, widgets, and reusable UI components.

## Architecture

The application is divided into several logical layers:

```text
                 ┌───────────────┐
                 │      UI       │
                 └───────┬───────┘
                         │
                      Events
                         │
                 ┌───────▼───────┐
                 │ Game / Lobby  │
                 │     Logic     │
                 └───────┬───────┘
                         │
                      Protocol
                         │
                 ┌───────▼───────┐
                 │    Network    │
                 │ Client/Server │
                 └───────────────┘
```

The project is designed to keep the UI, game logic, and networking code separated from each other.

The event system is used to connect different parts of the application without requiring direct dependencies between unrelated components.

## C++ Features

The project uses modern C++ features and practices, including:

* C++23
* Object-oriented programming
* RAII
* Smart pointers
* `std::unique_ptr`
* `std::shared_ptr`
* STL containers
* STL algorithms
* Iterators
* Lambda expressions
* Templates
* Concepts
* Move semantics

Particular attention is paid to object ownership and lifetime management using RAII and smart pointers.

## Project Status

The project is currently **in development**.

The UI framework, application architecture, lobby system, and networking layer are being developed alongside the game logic.

The complete game is not finished yet.

## Building

### Requirements

* Windows
* Visual Studio
* C++23 compatible compiler
* SFML

### Build

Clone the repository:

```bash
git clone https://github.com/Lux1e/Durak.git
```

Open the solution in Visual Studio:

```text
Durak.sln
```

Select the required configuration and build the project.

## Project Goals

This project was created to gain practical experience with:

* Modern C++
* Software architecture
* Object lifetime and ownership
* Event-driven communication
* UI framework development
* Client/server architecture
* Network communication
* Separation of responsibilities between application modules
