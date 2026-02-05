# Minimal C++ Terminal Game Framework

A lightweight, dependency free **terminal game framework** built from scratch using **standard C++ (C++17)**.
This project demonstrates core **game engine fundamentals**, including a real time game loop, entity based engine, collision detection, and non blocking input. All rendered using ASCII graphics in the terminal

## 🎮 Overview

This project implements a small grid-based game where the player navigates a bouded word, interacts with objects, and avoids enemies. Despite its simplicity, the framework mirrors real game engine architecture at a smaller scale.

The goal of the project is **educational**: to understand how game engines work internally without relying on external libraries

## ✨ Features

### 🔂 Real-Time Game Loop
* Fixed timestep loop (~10 FPS)
* Clear seperation between **Update** and **Render** phases
* Consistent behavior regardless of input timing

### 🧩 Entity System (OOP)
* Abstract `Entity` base class
* Polymorphic `update()` and `render()` methods
* Concrete entity types:
    * `Player`
    * `Enemy`
    * `Wall`
    * `Coin`

### 🧱 Collision Detection
* **Grid-based AABB collision**
* Position-based checks using integer coordinates
* Supports:
    * Solid collisions (walls block movement)
    * Trigger collisions (coins, enemies)

## 🕹️ Controls

| Key | Action |
| --- | --- |
| **W** | Move Up |
| **A** | Move Left |
| **S** | Move Down |
| **D** | Move Right |
| **Q** | **Quit Game** |

## 🧠 Architecture Overview

The codebase follows a **modular objed-oriented design:**

### 🧩 Game
* Owns the main game loop