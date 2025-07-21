# 🐚 Minishell

> A simplified Unix shell implementation, developed as part of the 42 School curriculum.

---

## 🧠 Introduction

**Minishell** is a project that aims to create a functional shell, inspired by Bash, with built-in command support, redirections, pipelines, variable expansions, and signal handling.
It’s a great exercise in process management, parsing, memory allocation, and system calls in a POSIX environment.

---

## 🚀 Features

### ✅ Mandatory:

* Prompt loop with input reading.
* Execution of commands with full path or via `$PATH`.
* Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
* Redirections: `>`, `<`, `>>`, `<<` (heredoc).
* Pipelines: `ls | grep .c | wc -l`.
* Environment variable expansion: `$HOME`, `$?`, etc.
* Quote handling: `'` and `"` following POSIX rules.
* Signal handling (`Ctrl-C`, `Ctrl-\`, etc.).

---

## 🧪 Usage

```bash
./minishell
```

A prompt (`minishell$`) will appear. You can now use built-in or system commands.

Examples:

```bash
minishell$ echo Hello World
minishell$ export PATH=/bin:/usr/bin
minishell$ ls -l | grep minishell > output.txt
minishell$ cat << EOF
> hello
> EOF
```

---

## 📚 Implementation Highlights

### 🔤 Lexer & Parser

* Tokenizes the user input and builds a command structure.
* Supports syntax error checking and quote balancing.

### 🧩 Expansion

* Expands environment variables, `$?`, and handles double quotes correctly.

### 🔧 Execution Engine

* Manages child processes, file descriptors, pipes, and built-in execution without forking.
* Implements a heredoc manager and redirection handlers.

### 💥 Signals

* Restores terminal behaviors and handles `SIGINT`, `SIGQUIT` gracefully.
* Prevents crashes and zombie processes.

---

## 👨‍💻 Authors

🧑‍🎓 Tharushan – [utharushan](https://github.com/utharushan)
🧑‍🎓 El Bachir – [ebella](https://github.com/Elbaaaaaa)
