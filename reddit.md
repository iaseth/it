# it - my poor man's version of tree command

I used to program C a few years ago, but recently I have mostly spent my time with Python and JavaScript. I always liked the tree command, but my `node_modules` and `.venv` folders didn't. Sure you can do something like this:

```
tree -I "node_modules|bower_components"
```

But I wanted a better solution. I wanted it to show last modified and size in a better way, and show more details for recognized file types. Like this:

```
├── src --- 10 hours ago
│	├── analysis.c --- 9 hours ago, 4 hashlines, 33 statements
│	├── analysis.h --- 9 hours ago, 4 hashlines, 13 statements
│	├── ignore.c --- 14 hours ago, 3 hashlines, 4 statements
│	├── ignore.h --- 14 hours ago, 3 hashlines, 1 statements
│	├── main.c --- 13 hours ago, 4 hashlines, 14 statements
│	├── stringutils.c --- 10 hours ago, 3 hashlines, 10 statements
│	├── stringutils.h --- 10 hours ago, 4 hashlines, 4 statements
│	├── tree.c --- 9 hours ago, 13 hashlines, 52 statements
│	├── tree.h --- 14 hours ago, 4 hashlines, 1 statements
│	├── utils.c --- 14 hours ago, 4 hashlines, 27 statements
│	├── utils.h --- 14 hours ago, 6 hashlines, 4 statements
├── CMakeLists.txt --- 2 hours ago, 184.0 B
├── LICENSE.md --- 1 day ago, 0 headers
├── README.md --- 1 hour ago, 7 headers
```

This is a project stucture for the this project itself. `Statements` just means lines ending with `semicolons`, `hashlines` or `headers` (markdown) means lines starting with a `#`. For `python`, it uses ending `:` to count the number of blocks and so on. I plan to add more features but it is already where it can be useful to me. Sharing it here so others may critique, use or learn from it - whichever applicable.

```
git clone https://github.com/iaseth/it.git
cd it/build
cmake ..
make
```

It ignores the following directories by default (which seems like common sense by somehow isn't):

```
const char *ignored_dirs[] = {
	"node_modules", ".venv", ".git", "build", "target",
	"__pycache__", "dist", "out", "bin", "obj", "coverage", ".cache"
};
```

I was coding in C after a long time, and ChatGPT was very useful for the first draft. Have not run valgrind on this one yet!

https://github.com/iaseth/it
