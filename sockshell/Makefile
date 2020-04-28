all: dropshell.sh

dropshell: dropshell.asm
	nasm -f bin -o dropshell dropshell.asm

dropshell.sh: dropshell
	printf 'printf "' > dropshell.sh
	hexdump -v -e '1 1 "\\x%02x"' dropshell >> dropshell.sh
	printf '" > dropshell; chmod +x dropshell; ./dropshell; rm dropshell' >> dropshell.sh

clean:
	rm -f dropshell dropshell.sh
