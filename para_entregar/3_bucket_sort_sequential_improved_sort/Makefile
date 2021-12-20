# source files.
SRC =bucket.c

OBJ = $(SRC:.cpp=.o)

OUT = bucket.out

# include directories
INCLUDES = -I.
 
# C compiler flags 
# -ftree-vectorize -msse4
# -g -fno-omit-frame-ointer
CCFLAGS = -std=c99 -O2 -I/share/apps/papi/5.4.1/include

# compiler
CCC = gcc 

# libraries
LIBS = -L/share/apps/papi/5.4.1/lib -lm -lpapi

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
