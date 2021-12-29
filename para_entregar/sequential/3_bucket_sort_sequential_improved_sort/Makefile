# source files.
SRC =bucket_original.c

OBJ = $(SRC:.cpp=.o)

OUT = bucket.out

# include directories
#INCLUDES = -I.
 
# C compiler flags 
# -ftree-vectorize -msse4
# -g -fno-omit-frame-ointer
#-I/share/apps/papi/5.4.1/include
CCFLAGS = -std=c99 -O2 

# compiler
CCC = gcc 

# libraries
#LIBS = -L/share/apps/papi/5.4.1/lib -lm -lpapi

.SUFFIXES: .cpp .c 


default: $(OUT)

.cpp.o:
	$(CCC) $(CCFLAGS) $(INCLUDES)  -c $< -o $@

.c.o:
	$(CCC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) -o $(OUT) $(CCFLAGS) $(OBJ) $(LIBS) 

depend:  dep
#
#dep:
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f *.o .a *~ Makefile.bak $(OUT)
