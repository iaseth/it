
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
cd build
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
├── src --- 8 hours ago
│   ├── analysis.c --- 7 hours ago, 4 hashlines, 33 statements
│   ├── analysis.h --- 7 hours ago, 4 hashlines, 13 statements
│   ├── ignore.c --- 12 hours ago, 3 hashlines, 4 statements
│   ├── ignore.h --- 12 hours ago, 3 hashlines, 1 statements
│   ├── main.c --- 12 hours ago, 4 hashlines, 14 statements
│   ├── stringutils.c --- 8 hours ago, 3 hashlines, 10 statements
│   ├── stringutils.h --- 8 hours ago, 4 hashlines, 4 statements
│   ├── tree.c --- 7 hours ago, 13 hashlines, 52 statements
│   ├── tree.h --- 12 hours ago, 4 hashlines, 1 statements
│   ├── utils.c --- 12 hours ago, 4 hashlines, 27 statements
│   ├── utils.h --- 12 hours ago, 6 hashlines, 4 statements
├── CMakeLists.txt --- 8 minutes ago, 184.0 B
├── LICENSE.md --- 1 day ago, 0 headers
├── README.md --- 4 minutes ago, 7 headers
```

---

## 📜 License

MIT License — feel free to use, modify, and share.

---

Made with C for speed ⚡ by [Ankur Seth](https://github.com/iaseth)
```
