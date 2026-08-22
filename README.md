Durak

A multiplayer card game Durak written in modern C++.

The project is developed from scratch as a personal C++ project. The main focus is not only the game itself, but also designing a modular application architecture, networking layer, event system and a reusable UI framework.

Technologies
C++23
SFML
STL
Visual Studio
Git / GitHub
Project Structure

The source code is organized into several independent modules:

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
Core

Contains functionality shared across different parts of the application.

The core module includes:

Event system
Client context
Input handling
Scene management
Common concepts and utilities
Animation support
Debug logging

The event system is used to communicate between application components without requiring them to depend directly on each other.

Game

Contains the game and lobby logic.

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

Game rules are separated from the general game logic through the Rules abstraction, with ClassicRules providing the implementation for classic Durak rules.

Lobby-related functionality is separated from the actual game logic and includes client-side lobby state and player data.

Network

The project contains separate client and server components for network communication.

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

The networking layer is responsible for:

Client/server communication
Connection management
Packet creation
Packet dispatching
Client sessions
Tracking pending packets
Connection state management

The network code is separated from the game and UI layers.

Protocol

The protocol module contains the definitions used for communication between the client and server.

Keeping protocol-related code separate from the networking implementation makes it possible to distinguish what data is transferred from how the connection is managed.

UI

A custom UI system is implemented on top of SFML.

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

The UI system is based around a common UIElement hierarchy.

Interactive elements are implemented separately from the base UI element, allowing input handling to be added to components that require it.

The UI is organized into layers and reusable components rather than being implemented directly inside game scenes.

Architecture

The application is divided into several logical layers:

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

The main goal of this separation is to prevent the UI, game logic and networking code from becoming tightly coupled.

For example, UI components can publish events without directly depending on the implementation of the game or lobby logic.

C++ Features

The project uses modern C++ features and practices, including:

RAII
Smart pointers
std::unique_ptr
STL containers
STL algorithms
Iterators
Lambda expressions
Templates
Concepts
Move semantics
Object-oriented programming
Compile-time type constraints

Object ownership and object lifetime are explicitly managed using RAII and smart pointers.

Current State

The project is under active development.

The application architecture, UI framework, lobby system and networking layer are being developed alongside the game itself.

The complete game is not finished yet. The project is primarily used as a practical exercise in developing a non-trivial C++ application from scratch.

Building
Requirements
Windows
Visual Studio
C++23 compatible compiler
SFML
Build

Clone the repository:

git clone https://github.com/Lux1e/Durak.git

Open the Visual Studio solution:

Durak.sln

Build the project using Visual Studio.

Project Goals

The project was created to gain practical experience with:

Modern C++
Software architecture
Object lifetime and ownership
Event-driven communication
UI framework development
Client/server architecture
Network communication
Separation of responsibilities between application modules
