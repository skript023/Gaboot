.DATA
	Hello db 'Hello ', 0
.CODE
	Greeting proc
	    lea rax, Hello
		ret
    Greeting endp
END