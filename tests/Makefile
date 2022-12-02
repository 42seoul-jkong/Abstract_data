.PHONY: all clean fclean re

CXXFLAGS += -Wall -Wextra -Werror --std=c++98 -I.. -O3 -fsanitize=address -g3

all: test_ft test_std
	time ./test_ft 42 > ft.txt
	time ./test_std 42 > std.txt
	diff ft.txt std.txt

clean:
	$(RM) ft.txt
	$(RM) std.txt
	$(RM) -r test_ft.dSYM
	$(RM) -r test_std.dSYM

fclean: clean
	$(RM) test_ft
	$(RM) test_std

re: fclean
	$(MAKE) all

test_ft: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 

test_std: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -DFT_USE_STL
