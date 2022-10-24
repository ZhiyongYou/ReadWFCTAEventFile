inc_dir = include/
src_dir = src/
obj_dir = obj/
CCOMPILER  = g++
DEBUG = -O2 #-ggdb  #-g -gstabs -gstabs+ -ggdb -glevel

object = $(obj_dir)main.o \
		$(obj_dir)EventDict.o \
		$(obj_dir)WFCTAEvent.o

main: $(obj_dir)main.a
	cp $(src_dir)EventDict_rdict.pcm ./
	$(CCOMPILER) -o main $(obj_dir)main.a `root-config --cflags --libs` -lMinuit -lMatrix

$(obj_dir)main.a: $(object)
	ar -r $(obj_dir)main.a $(object)

$(obj_dir)main.o: main.cc Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)EventDict.o: $(src_dir)EventDict.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

$(obj_dir)WFCTAEvent.o: $(src_dir)WFCTAEvent.cpp Makefile $(inc_dir)
	$(CCOMPILER) $(DEBUG) -c $< -o $@ -DSCAN -I $(inc_dir) `root-config --cflags --libs`

.PHONY : clean
 clean :
	rm main EventDict_rdict.pcm obj/*
