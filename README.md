# MicrogridSim

**A high-fidelity C++ microgrid simulator featuring PV I-V curve modeling, adaptive MPPT control, DC-DC converter dynamics, and energy storage dispatch logic.**

---

<details>
<summary><b>Contents</b></summary>

- [🚀 Engineering Overview](#-engineering-overview)
- [✨ Key Features](#-key-features)
- [📁 Project Structure](#-project-structure)
- [🛠️ Tech Stack](#️-tech-stack)
- [⚙️ Quick Start](#️-quick-start)
  - [1. Prerequisites](#1-prerequisites)
  - [2. Build & Run](#2-build--run)
  - [3. Basic Usage Example](#3-basic-usage-example)
- [📊 Simulation Capabilities](#-simulation-capabilities)
- [🎯 Use Cases](#-use-cases)
- [📋 Roadmap](#-roadmap)
- [👤 Author](#-author)

</details>

## 🚀 Engineering Overview

**MicrogridSim** is a high-performance C++ simulation framework for modeling and analyzing small-scale microgrids with renewable integration. It provides detailed physics-based models for photovoltaic (PV) arrays, battery energy storage, DC-DC power converters, and intelligent control algorithms including Maximum Power Point Tracking (MPPT) and energy dispatch.

Designed for research, education, and prototyping in power systems engineering, the simulator captures real-world dynamics such as environmental variability (irradiance and temperature), converter losses, battery state-of-charge management, and grid interaction. It enables users to test control strategies, optimize system sizing, and study energy balance under varying load and generation profiles.

---

## ✨ Key Features

* **Accurate PV Modeling:** Full single-diode I-V curve implementation with temperature and irradiance dependence.
* **Adaptive MPPT Controllers:** Perturb & Observe (P&O) and Incremental Conductance algorithms with configurable step sizes.
* **Power Electronics:** DC-DC converter (boost topology) with efficiency modeling and duty cycle control.
* **Energy Storage:** Battery model with SOC tracking, charge/discharge limits, and realistic dynamics.
* **Centralized Dispatch:** Intelligent energy management controller coordinating PV, battery, and grid exchange.
* **Data-Driven Simulation:** Supports time-series environmental and load profiles (CSV).
* **Comprehensive Logging:** Detailed CSV output and console reporting for post-analysis.
* **Modular & Extensible:** Clean OOP design with interfaces for easy addition of new models or controllers.

---

## 📁 Project Structure

```
MicrogridSim/
├── CMakeLists.txt
├── data/
│   └── daily_profile.csv
├── include/
│   ├── controllers/
│   │   ├── microgrid/
│   │   │   └── DispatchController.hpp
│   │   └── mppt/
│   │       ├── IncrementalConductance.hpp
│   │       ├── MpptAlgorithm.hpp
│   │       └── PerturbAndObserve.hpp
│   ├── models/
│   │   ├── converters/
│   │   │   └── DcDcConverter.hpp
│   │   ├── pv/
│   │   │   └── PvPanel.hpp
│   │   └── storage/
│   │       └── Battery.hpp
│   └── utils/
│       ├── Config.hpp
│       └── Logger.hpp
└── src/
    ├── controllers/
    │   ├── microgrid/
    │   │   └── DispatchController.cpp
    │   └── mppt/
    │       ├── IncrementalConductance.cpp
    │       └── PerturbAndObserve.cpp
    ├── main.cpp
    ├── models/
    │   ├── converters/
    │   │   └── DcDcConverter.cpp
    │   ├── pv/
    │   │   └── PvPanel.cpp
    │   └── storage/
    │       └── Battery.cpp
    └── utils/
        └── Logger.cpp
```


---

## 🛠️ Tech Stack

* **Core Language:** C++ (C++17)
* **Build System:** CMake
* **Mathematics:** Custom physics models (no external linear algebra libs required)
* **Data Handling:** Standard library I/O with CSV support
* **Simulation:** Discrete-time stepping with configurable profiles

---

## ⚙️ Quick Start

### 1. Prerequisites

* CMake (>= 3.15)
* C++ compiler with C++17 support (GCC/Clang/MSVC)
* (Optional) Make or Ninja for building

### 2. Build & Run

```bash
git clone https://github.com/nabilkhondaker/MicrogridSim.git
cd MicrogridSim

mkdir build && cd build
cmake ..
make

# Run the simulator
./microgrid_sim
```

### 3. Basic Usage Example

```cpp
// From main.cpp - core simulation loop
auto pvArray = std::make_shared<Models::PvPanel>(pvSpecs);
auto batteryStorage = std::make_shared<Models::Battery>(5000.0, 2000.0, 2000.0, 0.5);
auto dcDcConv = std::make_shared<Models::DcDcConverter>(Models::DcDcConverter::Topology::BOOST, 0.98);

auto mpptAlgo = std::make_shared<Controllers::MPPT::PerturbAndObserve>(0.5, 0.02);

Controllers::DispatchController mgController(pvArray, batteryStorage, dcDcConv, mpptAlgo);

auto profile = loadProfile("data/daily_profile.csv");

for (const auto& step : profile) {
    double gridExchange = mgController.step(step.time, step.irradiance, step.temp, step.load);
    // Results logged automatically
}
```

---

## 📊 Simulation Capabilities

- **PV System:** Temperature-adjusted I-V curves, power calculation at operating points.
- **MPPT Algorithms:** Real-time tracking with voltage perturbation or conductance-based decisions.
- **Battery Management:** SOC limits, power rate constraints, and energy balancing.
- **Converter Dynamics:** Duty cycle control for maximum power transfer to DC bus.
- **System Integration:** Coordinated dispatch handling excess generation (to grid) or deficits (from grid/battery).
- **Output:** Time-stamped logs of key variables (SOC, power flows, MPPT status) for visualization in tools like Python/MATLAB.

---

## 🎯 Use Cases

- Renewable energy system design and sizing
- MPPT algorithm comparison and tuning
- Microgrid energy management strategy testing
- Power systems education and demonstrations
- Research on hybrid PV-battery-grid interactions
- Pre-hardware validation of control logic

---

## 📋 Roadmap

- GUI visualization (e.g., Dear ImGui plots)
- Additional converter topologies and inverter models
- More MPPT variants and advanced control (Model Predictive Control)
- Stochastic weather modeling
- Python bindings for easier parameter sweeps and analysis
- Hardware-in-the-loop (HIL) integration support

---

## 👤 Author
*Engineered by Nabil Khondaker*
