
# 🌳 ignore-tree (`it`)

`it` is a fast and minimal CLI tool that prints a directory tree with file sizes, last modified and other details — while ignoring common build and cache directories.

🧹 Hidden files and junk folders are skipped by default, so you can focus on the actual content.

[![GitHub Repo](https://img.shields.io/badge/github-iaseth%2Fit-blue?logo=github)](https://github.com/iaseth/it)

---

## 🚀 Features

- 📁 **Folders before files**, sorted alphabetically
- 🕒 **Last modified** shown as `3 days ago`, `15 minutes ago`, etc.
- 📦 **File sizes** shown in human-readable units
- 🧩 **Extra details** shown for recognized file types
- 🔥 Ignores junk dirs like: `node_modules`, `.git`, `__pycache__`, `build/`, `dist/`, etc.
- 🫣 Hidden files are **not shown by default**
- 🎛 Use `--hidden` to **include** dotfiles/folders
- 🧩 Use `--simple` to **not show** extra details
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
./it --simple .       # Dot't show extra attibutes
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
├── src --- 19 hours ago
│   ├── analysis.c --- 4 hours ago, 4 hashlines, 11 blocks
│   ├── analysis.h --- 4 hours ago, 4 hashlines, 1 blocks
│   ├── ignore.c --- 22 hours ago, 3 hashlines, 3 blocks
│   ├── ignore.h --- 22 hours ago, 3 hashlines
│   ├── main.c --- 5 minutes ago, 4 hashlines, 9 blocks
│   ├── stringutils.c --- 6 hours ago, 4 hashlines, 7 blocks
│   ├── stringutils.h --- 6 hours ago, 5 hashlines
│   ├── tree.c --- 4 minutes ago, 11 hashlines, 22 blocks
│   ├── tree.h --- 5 minutes ago, 6 hashlines, 1 blocks
│   ├── utils.c --- 5 hours ago, 4 hashlines, 15 blocks
│   ├── utils.h --- 5 hours ago, 7 hashlines
├── CMakeLists.txt --- 10 hours ago, 13 lines
├── LICENSE.md --- 1 day ago
├── README.md --- just now, 7 headers
├── reddit.md --- 8 hours ago, 1 headers
```

---

## 📜 License

MIT License — feel free to use, modify, and share.

---

Made with C for speed ⚡ by [Ankur Seth](https://github.com/iaseth)
```
