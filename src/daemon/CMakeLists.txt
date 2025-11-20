add_executable(add
	main.c
	request.c
)

target_include_directories(add PUBLIC ../common)

target_link_libraries(add PUBLIC ad_common)

INSTALL(TARGETS add
  RUNTIME DESTINATION bin
)


