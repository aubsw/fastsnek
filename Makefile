# Directories
RAYLIB_DIR = raylib
RAYLIB_SRC = $(RAYLIB_DIR)/src
DIST_DIR = dist

# Tools
EMCC = emcc
EMAR = emar
GIT = git
NPROC = $(shell nproc || echo 4)
MAKE_FLAGS = -j$(NPROC)

# Flags
CFLAGS = -Os -Wall -DPLATFORM_WEB -DGRAPHICS_API_OPENGL_ES2
LDFLAGS = -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file $(RAYLIB_DIR)/src/shell.html

# Output files
RAYLIB_A = libraylib.a
OUTPUT = hello_world.html
HTACCESS = .htaccess

# Raylib object files
RAYLIB_OBJS = rcore.o rshapes.o rtextures.o rtext.o rmodels.o utils.o raudio.o

.PHONY: all clean check-raylib compile-raylib create-htaccess serve

all: $(DIST_DIR)/$(OUTPUT) $(DIST_DIR)/$(HTACCESS)

# Create dist directory
$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# Check if raylib exists, clone if not
check-raylib:
	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
		echo "Cloning raylib repository..."; \
		$(GIT) clone git@github.com:raysan5/raylib.git; \
	else \
		echo "raylib repository already exists"; \
	fi

# Track object files individually
$(RAYLIB_SRC)/%.o: $(RAYLIB_SRC)/%.c
	$(EMCC) -c $< -o $@ $(CFLAGS)

# Compile raylib only when needed
compile-raylib: check-raylib $(addprefix $(RAYLIB_SRC)/,$(RAYLIB_OBJS))
	@echo "Creating raylib archive..."
	cd $(RAYLIB_SRC) && $(EMAR) rcs $(RAYLIB_A) $(RAYLIB_OBJS)

# Create htaccess file
$(DIST_DIR)/$(HTACCESS): | $(DIST_DIR)
	@echo "Creating .htaccess file..."
	@echo "AddType application/wasm .wasm" > $@

# Compile hello_world.cc
# Compile hello_world.cc
$(DIST_DIR)/$(OUTPUT): hello_world.cc compile-raylib | $(DIST_DIR)
	@echo "Compiling hello_world.cc..."
	$(EMCC) $< -o $@ $(CFLAGS) $(LDFLAGS) -I$(RAYLIB_SRC) -L$(RAYLIB_SRC) -lraylib

# Serve the app
serve: all
	@echo "Starting HTTP server on http://localhost:8000/$(OUTPUT)"
	cd $(DIST_DIR) && python3 -m http.server 8000

# Clean build artifacts
clean:
	rm -rf $(DIST_DIR)
	cd $(RAYLIB_SRC) && rm -f *.o $(RAYLIB_A)

# Deep clean - remove raylib too
deep-clean: clean
	rm -rf $(RAYLIB_DIR)
