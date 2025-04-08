
# 🌳 tree-that-ignores (`tti`)

`tti` is a fast and minimal CLI tool that prints a directory tree with file sizes and "last modified" timestamps — while ignoring common build and cache directories.

🧹 Hidden files and junk folders are skipped by default, so you can focus on the actual content.

[![GitHub Repo](https://img.shields.io/badge/github-iaseth%2Ftree--that--ignores-blue?logo=github)](https://github.com/iaseth/tree-that-ignores)

---

## 🚀 Features

- 📁 **Folders before files**, sorted alphabetically
- 🕒 **"Last modified"** shown as `3 days ago`, `15 minutes ago`, etc.
- 📦 **File sizes** shown in human-readable units
- 🔥 Ignores junk dirs like: `node_modules`, `.git`, `__pycache__`, `build/`, `dist/`, etc.
- 🫣 Hidden files are **not shown by default**
- 🎛 Use `--hidden` to include dotfiles/folders
- 🧵 Supports **multiple paths**

---

## 🛠 Build

Requires CMake and a C compiler (GCC, Clang, etc.)

```bash
git clone https://github.com/iaseth/tree-that-ignores.git
cd tree-that-ignores
mkdir build && cd build
cmake ..
make
```

This creates the `tti` binary in `build/`.

---

## 📦 Usage

```bash
./tti                  # Tree of current dir (ignores hidden files/dirs)
./tti src test         # Multiple paths
./tti --hidden .       # Include hidden files and folders
```

---

## 🛑 Ignored folders

The following are always ignored to keep output clean:

```
node_modules  .venv  .git  build  target  __pycache__  dist  out  bin  obj  coverage  .cache
```

---

## 📂 Project Structure

```
tree-that-ignores/
├── src/
│   └── tti.c
├── CMakeLists.txt
└── README.md
```

---

## 📜 License

MIT License — feel free to use, modify, and share.

---

Made with C for speed ⚡ by [Ankur Seth](https://github.com/iaseth)
```
