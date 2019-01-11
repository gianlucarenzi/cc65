CL ?= cl65
MACH ?= atari

all: charmapping.com  defdev.com  displaylist.com mem.com ostype.com sys.com

charmapping.com: charmapping.c
	$(CL) -t $(MACH) charmapping.c -o charmapping.com

defdev.com: defdev.c
	$(CL) -t $(MACH) defdev.c -o defdev.com

displaylist.com: displaylist.c
	$(CL) -t $(MACH) displaylist.c -o displaylist.com

mem.com: mem.c
	$(CL) -t $(MACH) mem.c -o mem.com

ostype.com: ostype.c
	$(CL) -t $(MACH) ostype.c -o ostype.com

sys.com: sys.c
	$(CL) -t $(MACH) sys.c -o sys.com

clean:
	rm *.com
	rm *.o
