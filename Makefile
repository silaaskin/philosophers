NAME = philo

CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread

SRCS =	main.c \
		routine.c \
		monitor.c \
		routine_utils.c \
		utils.c \
		init.c 

OBJS = $(SRCS:.c=.o)
%.o :	%.c
		$(CC) $(CFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) :	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean :
		rm -rf $(OBJS)

fclean :
		rm -rf $(NAME)
		rm -rf $(OBJS)
re :	fclean all 