# Qt6_Oscilloscope

Un osciloscop software modern scris în C++ și Qt6, cu două canale (CH1 & CH2), zoom, pan și semnal de test sinusoidal. Inspirat din osciloscoapele clasice, dar cu o interfață modernă și extensibilă.

---

## 📦 Funcționalități principale

- ✅ Două canale: CH1 (verde), CH2 (roșu)
- ✅ Afișare semnal în timp real
- ✅ Grilă osciloscop cu axă X/Y și unități (ms/div, V/div)
- ✅ Zoom pe X (timp) cu scroll
- ✅ Zoom pe Y (volt) cu slider vertical
- ✅ Factor de conversie configurabil pentru a afisa valorile in volti
- ✅ Pan stânga-dreapta cu mouse drag
- ✅ Generator de semnal sinusoidal pentru testare locală

---

## 🛠️ Cerințe

- Qt6 (`qt6-base-dev`)
- CMake
- Compilator C++17 (ex: GCC, Clang, MSVC)

---

## 🔧 Build local (Linux/Codex)

```bash
sudo apt update
sudo apt install -y qt6-base-dev qt6-base-dev-tools cmake build-essential

git clone https://github.com/<user>/Qt6_Oscilloscope.git
cd Qt6_Oscilloscope
mkdir build
cd build
cmake ..
make
./Qt6_Oscilloscope
