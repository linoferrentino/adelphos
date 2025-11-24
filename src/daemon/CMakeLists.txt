add_executable(add
	main.c
	request.c
)

target_include_directories(add PUBLIC ../common)
target_include_directories(add PUBLIC ../lib)

target_link_libraries(add PUBLIC ad_common)
target_link_libraries(add PUBLIC adelphos)

INSTALL(TARGETS add
  RUNTIME DESTINATION bin
)


