# `Pipex`

The purpose of this project is to recreate the way two commands are piped together via `|` in a shell terminal.
</br>
Allows for the discovery of UNIX mechanisms - `pipe`.

## Mandatory part

The program execution is as follows:

`$> ./pipex file1 cmd1 cmd2 file2`

It takes four arguments:
- `file1` and `file2` are file names
- `cmd1` and `cmd2` are shell commands with their parameters

It behaves the same as the shell command below:

- `$> < file1 cmd1 | cmd2 > file2`


## Bonus part

### Handle multiple pipes
The program execution is as follows:

`$> ./pipex_bonus file1 cmd1 cmd2 cmd3 ... cmdn file2`

Behaves like this:

`$> < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2`

It takes at least four arguments:
- `file1` and `file2` input and output file names
- `cmd1` to `cmdn` are shell commands with their parameters
##
### 🌟 A Quick Note
I hope this repository proves useful to you, whether you're working on the Push Swap project yourself, exploring my profile, or just curious about the 42 curriculum.
<br />Feel free to dive in, learn, and adapt the code as you see fit.
<br />Best of luck on your coding journey! 🚀
