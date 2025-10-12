add_executable(adtest
	main.c
)

target_link_libraries(adtest PUBLIC adelphos)


INSTALL(TARGETS adtest
  RUNTIME DESTINATION bin
)


