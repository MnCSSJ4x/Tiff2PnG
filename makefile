output: tiff2.c png.c
	gcc -o output  tiff2.c png.c libz.a -lm
clean:
	rm -rf *o output
	rm -rf *o TargetImage.png
