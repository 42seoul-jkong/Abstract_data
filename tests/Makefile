main: test_ft test_std
	time ./test_ft 42 > ft.txt
	time ./test_std 42 > std.txt
	diff ft.txt std.txt

test_ft: main.cpp
	c++ -Wall -Wextra -Werror --std=c++98 $^ -o $@ -I.. -O3 -fsanitize=address -g3

test_std: main.cpp
	c++ -Wall -Wextra -Werror --std=c++98 $^ -o $@ -I.. -DFT_USE_STL -O3 -fsanitize=address -g3
