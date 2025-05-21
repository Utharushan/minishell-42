# libftprintf - A Modular C Library

## Overview

This repository contains a combination of some fundamental C functions often used in the 42 curriculum:
- **libft**: A collection of utility functions like memory manipulation, string handling, and more.
- **ft_printf**: A custom implementation of the `printf` function.
- **get_next_line**: A function to read a line from a file descriptor.

This repository consolidates all three projects into one unified project for ease of use and management. You can compile and use these functions in any C project by linking the static library `libft.a` or directly integrating the source files into your own project.

## Contents

- **libft**: Core C utility functions (memory, string, output, linked lists, etc.).
- **ft_printf**: A custom implementation of the `printf` function for formatted output.
- **get_next_line**: Reads the next line from a file descriptor, useful for processing text files.

## Project Structure

```
src/
  libft/
    ft_is/
      ft_isalnum.c
      ft_isalpha.c
      ft_isascii.c
      ft_isdigit.c
      ft_isprint.c
      ft_isspace.c
      ft_tolower.c
      ft_toupper.c
    ft_mem/
      ft_bzero.c
      ft_calloc.c
      ft_memchr.c
      ft_memcmp.c
      ft_memcpy.c
      ft_memmove.c
      ft_memset.c
    ft_put/
      ft_putchar_fd.c
      ft_putnbr_fd.c
      ft_putendl_fd.c
      ft_putstr_fd.c
    ft_str/
      ft_atoi.c
      ft_atof.c
      ft_itoa.c
      ft_split.c
      ft_strchr.c
      ft_strdup.c
      ft_striteri.c
      ft_strjoin.c
      ft_strlcat.c
      ft_strlcpy.c
      ft_strlen.c
      ft_strmapi.c
      ft_strcmp.c
      ft_strncmp.c
      ft_strnstr.c
      ft_strrchr.c
      ft_strtrim.c
      ft_substr.c
    ft_lst/
      ft_lstadd_back.c
      ft_lstadd_front.c
      ft_lstclear.c
      ft_lstdelone.c
      ft_lstiter.c
      ft_lstlast.c
      ft_lstmap.c
      ft_lstnew.c
      ft_lstsize.c
  ft_printf/
    ft_printf.c
    ft_printf_utils.c
  get_next_line/
    get_next_line.c
inc/
  libft.h
  ft_printf.h
  get_next_line.h
Makefile
README.md
```

## Installation

### Prerequisites

To compile and use this library, you will need:
- **A C compiler** (e.g., `cc`).
- **Make** utility to compile the project.

### Compilation

1. Clone this repository:

    ```bash
    git clone https://github.com/Utharushan/libftprintf-42.git
    cd libftprintf-42
    ```

2. Build the static library `libft.a`:

    ```bash
    make
    ```

3. This will compile all the source files and create a static library `libftprintf.a`.

### Cleaning up

To remove the compiled object files and static library, run:

```bash
make clean
```

To completely remove all the compiled files, including the library, run:

```bash
make fclean
```

If you want to recompile the library from scratch, use:

```bash
make re
```

## Usage

### Option 1: Link the Static Library

1. In your project, include the header file:

    ```c
    #include "libft.h" // or ft_printf.h / get_next_line.h
    ```

2. During compilation, link your project with the `libft.a` static library:

    ```bash
    cc -Wall -Wextra -Werror -I/path/to/libftprintf/inc -o my_program my_program.c -L/path/to/libftprintf -lft
    ```

This will allow you to use all the functions provided by `libft`, `ft_printf`, and `get_next_line` in your project.

### Option 2: Direct Integration of Source Files

Alternatively, you can directly copy the `.c` files from `src/libft/`, `src/ft_printf/`, and `src/get_next_line/` into your project and include their corresponding header files.

## Functions Provided

### libft Functions

- Memory Functions: `ft_bzero`, `ft_calloc`, `ft_memchr`, `ft_memcmp`, `ft_memcpy`, `ft_memmove`, `ft_memset`
- String Functions: `ft_atoi`, `ft_itoa`, `ft_strdup`, `ft_strlen`, `ft_strjoin`, `ft_strchr`, `ft_strrchr`, `ft_substr`, `ft_strtrim`, `ft_split`, `ft_strmapi`, etc.
- List Functions: `ft_lstadd_back`, `ft_lstadd_front`, `ft_lstclear`, `ft_lstdelone`, `ft_lstiter`, `ft_lstlast`, `ft_lstmap`, `ft_lstnew`, `ft_lstsize`
- Output Functions: `ft_putchar_fd`, `ft_putnbr_fd`, `ft_putstr_fd`, `ft_putendl_fd`

### ft_printf Functions

- `ft_printf`: A custom implementation of the `printf` function that handles formatted output.

### get_next_line Functions

- `get_next_line`: Reads a line from a file descriptor. This function is particularly useful for handling text files line-by-line.

## Example

Here is an example of how you can use some of these functions in your own project:

```c
#include "libft.h"

int main()
{
    char  *str = "Hello, World!";
    
    // Using libft function
    ft_putstr_fd(str, 1); // Prints "Hello, World!" to stdout

    // Using ft_printf function
    ft_printf("Number: %d\n", 42);

    // Using get_next_line function
    int  fd = open("file.txt", O_RDONLY);
    char  *line = get_next_line(fd);
    ft_putstr_fd(line, 1);
    close(fd);

    return (0);
}
```

## Contributing

Feel free to contribute to this project by submitting issues, pull requests, or suggestions. To contribute:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Create a pull request.
