# Unreal Engine C++/Blueprint Hybrid Game Architecture

This repository contains the complete core software architecture and custom C++ source code for my undergraduate Final Year Project (FYP). The system leverages an industry-standard hybrid development workflow: utilizing low-level, optimized C++ for foundational logic, systems architecture, and memory management, while dynamically exposing properties to Unreal Engine's Blueprint visual scripting for modular implementation and UI events.



##  Academic Background & Overview
* *Degree:* Bachelor of Science in Computer Science (CGPA: 3.45 / 4.0)
* *Core Foundation:* 100% C++ based foundational coding, object-oriented programming (OOP), and data structures.
* *Project Type:* Undergraduate Final Year Project (FYP)



##  Core Engineering & Technical Features

* *Modular Object-Oriented Architecture:* The codebase is strictly partitioned into high-performance, independent modules to ensure a clean, production-ready environment.
* *C++ Performance Layer:* Handled complex math equations, player state logic, base actor initializations (such as Item and Soul), and interface mechanics in C++ to minimize execution overhead and eliminate heavy execution loops.
* *Blueprint Extensibility:* Architected robust UPROPERTY macro chains and exposed custom functions to provide flexible variables, visual effects mapping, and UI hooks for rapid design adjustment.
* *Clean Memory Management:* Leveraged custom C++ component structures, pointers, and interfaces to handle gameplay loops efficiently without redundant runtime allocations.



##  Repository Structure Overview
The codebase follows a professional, strictly mirrored architecture separating declarations (.h) from implementation (.cpp) across matching modular domains:

```text
Source/
└── GameDev/
    ├── Public/                  # Header (.h) Declarations
    │   ├── Item.h
    │   ├── Soul.h
    │   └── [Subfolders Below]
    │
    ├── Private/                 # Source (.cpp) Implementations
    │   ├── Item.cpp
    │   ├── Soul.cpp
    │   └── [Subfolders Below]
    │
    # Mirrored Module Subfolders inside both Public/ and Private/:
    ├── Breakable/               # Destructible environment logic
    ├── Characters/              # Player and AI character base classes
    ├── Components/              # Modular actor components for decoupled logic
    ├── Enemies/                 # Enemy state systems and behaviors
    ├── HUD/                     # Head-up display and UI data binding
    ├── Interfaces/              # C++ interfaces for cross-actor communication
    ├── Pawns/                   # Custom controllable world pawns
    ├── Treasure/                # Collectibles and reward mechanics
    └── Weapons/                 # Combat and projectile systems
