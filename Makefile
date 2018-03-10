LCC = gcc
WCC = x86_64-w64-mingw32-gcc

CWW = -Wall -Werror -Wextra

FNAME = gamemap.c

all: compileW compileL

CW: compileWall

compileW: $(FNAME)
	$(WCC) $(FNAME) -o wgamemap.exe 

compileL: $(FNAME)
	$(LCC) $(FNAME) -o lgamemap.out

compileWall: $(FNAME)
	$(LCC) $(CWW) $(FNAME) -o lgamemap.out

clean:
	rm -rf *.exe rm -rf *.out
