## Change your name here:
MYNAME=DACOSTA_CHOGNOT

## DO NOT change below
SRC=IO/src
HERE=$(PWD)
MAINCLASS=Files
SRCJAVA=$(wildcard $(SRC)/*.java)
CLASSFILES = $(SRCJAVA:.java=.class)

all: run

# Compile
%.class: %.java
	javac $^ -d $(HERE)

# Run
run: $(SRC)/$(MAINCLASS).class
	java $(SRC)/$(MAINCLASS) 

tar: clean
	dir=$$(basename $$PWD/$$SRC) && cd .. && \
	tar cvfz "TP04-$(MYNAME).tgz" --exclude=".git" "$$dir"

clean:
	rm -f $(SRC)/*.class $(SRC)/*~ *~
