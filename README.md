# todo - simple command-line task manager

A small utility for maintaining a todo list right in the terminal. All tasks are stored in `~/.local/state/todo/todo.list`.

## Installation

```bash
git clone https://github.com/smysloff/todo.git
cd todo
make install
```

After installation, the program is available as the todo command.

## Usage

```bash
# Show all tasks
todo

# Add a new task
todo add "Buy milk"

# Delete a task by number
todo del 3

# Create a new empty list
todo new

# Show help
todo help
```

## Example

```bash
$ todo add "Call mom"
$ todo add "Finish project"
$ todo add "Buy bread"
$ todo

1) Call mom
2) Finish project
3) Buy bread

$ todo del 2
$ todo

1) Call mom
2) Buy bread
```

## Uninstallation

```bash
sudo make uninstall    # removes the program
rm -rf ~/.local/state/todo  # removes all tasks (optional)
```

## Requirements

* Linux (or any POSIX-compatible system)
* C compiler (gcc or clang)

## License

Distributed under the GNU General Public License v3.0 or later. See [LICENSE](LICENSE) for more information.
