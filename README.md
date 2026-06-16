# todo - simple command-line task manager

A small utility for maintaining a todo list right in the terminal. All tasks are stored in `~/.local/state/todo/`.

## Example how it looks

```txt
My tasks
[→] 1. Write report // >
[✓] 2. Send email   // +
[✗] 3. Buy coffee   // -
[ ] 4. Call client  // *
```

## Usage

```bash
$ todo add "Write report"
$ todo add "Send email"
$ todo current 1      # mark first as current
$ todo end 2          # mark second as finished
$ todo
[→] 1. Write report
[✓] 2. Send email

$ todo new "Work"
Switched to list: Work

$ todo title "My tasks"  # set title
$ todo
My tasks
[ ] 1. ...
```

## Installation

```bash
git clone https://github.com/smysloff/todo.git
cd todo
make install
```

## Uninstallation

```bash
make uninstall    # removes the program and task list
```

After installation, the program is available as the todo command.

## List of commands

| fullname | shortname | argument 1 | argument 2 | description                                                 |
|----------|-----------|------------|------------|-------------------------------------------------------------|
|          |           |            |            | todo without command displays the current list              |
| add      |           | <value>    |            | adds an item to the list                                    |
| remove   | rm        | <index>    |            | removes the item at the given index                         |
| move     | mv        | <index1>   | <index2>   | moves the item at index1 to position index2                 |
| swap     |           | <index1>   | <index2>   | swaps the items at the two given indexes                    |
| edit     | ed        | <index>    | <value>    | replaces the text of the task at given index                |
| clear    |           |            |            | deletes all items from the current list                     |
| current  |           | <index>    |            | marks the item as currently working on                      |
| end      |           | <index>    |            | marks the item as finished                                  |
| cancel   |           | <index>    |            | marks the item as canceled or failed                        |
| unmark   |           | <index>    |            | removes all marks from the item                             |
| new      |           |            |            | creates new todo list and switches to it                    |
| title    |           | <value>    |            | sets the title for the current list                         |
| list     | ls        |            |            | prints all existing lists                                   |
| change   | ch        | <index>    |            | switches to the list at the given index                     |
| help     | h         |            |            | shows this help message                                     |
| version  | v         |            |            | shows the current version                                   |

## To-Do

### Core Features (for 1.0.0)
* Create, delete items ✓
* Work with Unix system directories ✓
* Move items within the list ✓
* Edit items
* Mark items (current, finished, canceled)
* Multiple lists support
* version command
* help/usage command
* List titles (name command)
* List descriptions (description command)  ← добавлено
* Support for arguments without quotes (auto-join multiple args)
* Color highlighting
* Support for Linux / BSD / MacOS
* Basic tests
* Man page

### Future Features (post-1.0.0)
------------------------------------------------
* Automatic publishing
* Support for Windows
* Pre-built binaries
* Arch PKGBUILD, Debian .deb, RPM packages

## Current version

**0.3.1**

## Requirements

* Linux (or any POSIX-compatible system)
* C compiler (gcc or clang)

## License

Distributed under the GNU General Public License v3.0 or later. See [LICENSE](LICENSE) for more information.
