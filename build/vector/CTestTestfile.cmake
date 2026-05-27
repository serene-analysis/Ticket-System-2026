# CMake generated Testfile for 
# Source directory: /home/enovo/STLite-ACM-2026/vector
# Build directory: /home/enovo/STLite-ACM-2026/build/vector
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(vector_one "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_one >/tmp/one_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/one/answer.txt /tmp/one_out.txt>/tmp/one_diff.txt")
set_tests_properties(vector_one PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;10;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_two "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_two >/tmp/two_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/two/answer.txt /tmp/two_out.txt>/tmp/two_diff.txt")
set_tests_properties(vector_two PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;12;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_three "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_three >/tmp/three_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/three/answer.txt /tmp/three_out.txt>/tmp/three_diff.txt")
set_tests_properties(vector_three PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;14;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_four "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_four >/tmp/four_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/four/answer.txt /tmp/four_out.txt>/tmp/four_diff.txt")
set_tests_properties(vector_four PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;16;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_five "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_five >/tmp/five_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/five/answer.txt /tmp/five_out.txt>/tmp/five_diff.txt")
set_tests_properties(vector_five PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;18;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_six "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_six >/tmp/six_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/six/answer.txt /tmp/six_out.txt>/tmp/six_diff.txt")
set_tests_properties(vector_six PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;20;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
add_test(vector_seven "sh" "-c" "/home/enovo/STLite-ACM-2026/build/vector/vector_seven >/tmp/seven_out.txt        && diff -u /home/enovo/STLite-ACM-2026/vector/data/seven/answer.txt /tmp/seven_out.txt>/tmp/seven_diff.txt")
set_tests_properties(vector_seven PROPERTIES  _BACKTRACE_TRIPLES "/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;22;add_test;/home/enovo/STLite-ACM-2026/vector/CMakeLists.txt;0;")
