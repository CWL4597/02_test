
SRC=pos/console.o main.o p/start_mplayer.o

main:$(SRC) pos/console.h
	gcc -o $@ $^
	
%.o:%.c
	gcc -c -o $@ $<
console.o:pos/console.c pos/console.h
	gcc -c -o pos/console.o pos/console.c
start_mplayer.o:p/start_mplayer.c p/start_mplayer.h
	gcc -c -o p/start_mplayer.o p/start_mplayer.c

debug:
	@echo $(SRC)
	@echo $(PWD)
	
clean:
	rm main $(SRC) -rf
