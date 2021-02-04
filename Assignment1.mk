# The compiler: since it's C it will be gcc
CC = gcc

alg1 = sobel

alg1_args = garb34.pgm sobel_mag.pgm sobel_low.pgm sobel_high.pgm 40 110

alg2 = canny

alg2_args = garb34.pgm canny_mag.pgm canny_peaks.pgm canny_final.pgm 1 0.0465

all: run_alg1 run_alg2

$(alg1): $(alg1).c
	$(CC) -o $(alg1) $(alg1).c

$(alg2): $(alg2).c
	$(CC) -o $(alg2) $(alg2).c

run_alg1: $(alg1)
	./$(alg1) $(alg1_args)

run_alg2: $(alg2)
	./$(alg2) $(alg2_args)

.PHONY : clean
#rm for linux systems, del for windows systems
clean:
	del sobel_mag.pgm sobel_low.pgm sobel_high.pgm canny_mag.pgm canny_peaks.pgm canny_final.pgm
	del sobel.exe marrh.exe
