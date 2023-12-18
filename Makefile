NAME        = ./server

SRCS        = $(wildcard *.cpp)
OBJS        = $(patsubst %.cpp,objs/%.o,$(SRCS))
CC          = c++
FLAGS       = -Wall -Werror -Wextra
RM          = rm -rf

all: objs $(NAME)

objs:
	@mkdir -p objs

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

objs/%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
