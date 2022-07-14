# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/02 21:54:50 by moabid            #+#    #+#              #
#    Updated: 2022/07/05 15:37:15 by moabid           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= ./philo

SOURCES 	= $(shell find . -name "*.c")
HEADERS 	= $(shell find . -name "*.h")

OBJECTS 	= $(patsubst %.c, %.o, $(SOURCES))
DEPENDS 	= $(patsubst %.c, %.d, $(SOURCES))

CFLAGS =  -Wall -g

all: $(NAME)

%.o: %.c
	@$(CC) -Iincludes $(CFLAGS) -MMD -MP -c $< -o $@

$(NAME): $(OBJECTS)
	@echo "Dependencies Compiled !"
	@$(CC) -Iincludes $(OBJECTS)  -fsanitize=thread $(LDFLAGS) -o $(NAME)
	@echo "Compiled !"

clean:
	-@$(RM) $(OBJECTS) $(DEPENDS)
	@echo "Everything is Cleaned !"

fclean: clean
	-@$(RM) $(NAME)

re: clean all

.PHONY: re run fclean clean all

-include $(DEPENDS)