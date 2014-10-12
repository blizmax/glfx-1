# Needed to make glUniform1f work
CFLAGS += -DGL_GLEXT_PROTOTYPES

%.o: %.glsl
	ld -r -b binary -o $@ $^

demo: main.o glfx.o XDL.o frag.o
	$(CC) -lSDL2 -lGL -lm $^ -o$@
