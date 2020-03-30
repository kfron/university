

reversi: reversi.c main.c lin-fifo.c
	gcc main.c reversi.c lin-fifo.c -std=c11 -o reversi `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm reversi
