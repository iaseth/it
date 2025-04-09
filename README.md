
# 🌳 ignore-tree (`it`)

`it` is a fast and minimal CLI tool that prints a directory tree with file sizes and "last modified" timestamps — while ignoring common build and cache directories.

🧹 Hidden files and junk folders are skipped by default, so you can focus on the actual content.

[![GitHub Repo](https://img.shields.io/badge/github-iaseth%2Fit-blue?logo=github)](https://github.com/iaseth/it)

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
git clone https://github.com/iaseth/it.git
cd it
mkdir build && cd build
cmake ..
make
```

This creates the `it` binary in `build/`.

---

## 📦 Usage

```bash
./it                  # Tree of current dir (ignores hidden files/dirs)
./it src test         # Multiple paths
./it --hidden .       # Include hidden files and folders
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
it/
├── CMakeLists.txt
├── LICENSE.md
├── README.md
└── src
    ├── analysis.c
    ├── analysis.h
    ├── ignore.c
    ├── ignore.h
    ├── main.c
    ├── stringutils.c
    ├── stringutils.h
    ├── tree.c
    ├── tree.h
    ├── utils.c
    └── utils.h

```

---

## 📜 License

MIT License — feel free to use, modify, and share.

---

Made with C for speed ⚡ by [Ankur Seth](https://github.com/iaseth)
```
