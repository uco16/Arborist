CXX=g++
RM=rm -f

# Executable name
EXE=Arborist

# Source and object files
SRCS=main.cpp position.cpp types.cpp misc.cpp evaluate.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE).out $(OBJS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
