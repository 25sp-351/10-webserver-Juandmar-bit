CC = clang
CFLANGS = -Wall -Wextra -g
EXEC = web_server
SRC = request.c response.c helper.c web_server.c
PORT ?= 3000

all: $(EXEC)

$(EXEC): $(SRC)
		$(CC) $(CFLANGS) -o $(EXEC) $(SRC)

rebuild: clean all

run: $(EXEC)
		@echo "Running ... $(PORT)"
		./$(EXEC) -p $(PORT)

clean: 
		rm -f $(EXEC)