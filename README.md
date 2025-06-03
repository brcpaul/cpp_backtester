# Matching Engine in C++

Welcome to the **Matching Engine** project in **C++**. This project implements a high-performance matching engine in modern C++ designed for financial markets. It parses trading orders from a CSV file, processes them using a limit order book per instrument, and outputs the results to a CSV file.

---

## 📋 Table of Contents

- [Matching Engine in C++](#matching-engine-in-c)
  - [📋 Table of Contents](#-table-of-contents)
  - [🌟 Introduction](#-introduction)
  - [✨ Features](#-features)
  - [🔧 Prerequisites](#-prerequisites)
  - [📦 Installation](#-installation)
  - [🚀 Usage](#-usage)
  - [🗃️ Input Format](#%EF%B8%8F-input-format)
  - [📤 Output Format](#-output-format)
  - [✅ Unit Tests](#-unit-tests)
  - [📁 Project Structure](#-project-structure)
---

## 🌟 Introduction

The objective of this project is to develop a reliable and efficient matching engine in C++ that processes real-world trading orders—including LIMIT and MARKET types—with support for standard actions such as NEW, MODIFY, and CANCEL. Orders are matched according to the market-standard price-time priority, and execution results are serialized to include status, executed quantity, and counterparty information.
You may find the subject [here](docs/Subject.pdf).

---

## ✨ Features

- CSV-based order intake with configurable instruments.

- Matching engine with correct priority logic (price first, then time).

- Output of all order actions and matching results to CSV.

- Order types supported: LIMIT, MARKET.

- Order actions supported: NEW, MODIFY, CANCEL.

- Fully unit-tested and modular architecture.

---

## 🔧 Prerequisites

Before starting, make sure your environment has the following tools installed:

- **C++17** or later
- **STL** (standard library only – no external trading libraries)
- **CMake** for build configuration
- **Git** 

---

## 📦 Installation

To run the machine engine, execute the following commands:

    git clone https://github.com/brcpaul/cpp_backtester.git
    cd cpp_backtester
    mkdir build && cd build
    cmake ..
    make

---

## 🚀 Usage

Nécessaire ou non?

---

## 🗃️ Input Format

Each row in the input file follows this schema:

    timestamp,order_id,instrument,side,type,quantity,price,action
    
Example:

    1617278400000000000,1,AAPL,BUY,LIMIT,100,150.25,NEW

---

## 📤 Output Format

Output rows follow this schema:

    timestamp,order_id,instrument,side,type,quantity,price,action,status,executed_quantity,execution_price,counterparty_id
    
Example:

    1617278400000000100,2,AAPL,SELL,LIMIT,0,150.25,NEW,EXECUTED,50,150.25,1

---

## ✅ Unit Tests

To run the unit tests, execute the following command:

    ./tests/run_tests

---

## 📁 Project Structure

![](docs/file-tree.drawio.svg)

---






