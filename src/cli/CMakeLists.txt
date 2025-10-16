add_executable(adcli
	main.c
	cli.c
)

target_include_directories(adcli PUBLIC ../lib)
target_link_libraries(adcli PUBLIC adelphos)
target_link_libraries(adcli PUBLIC readline)

INSTALL(TARGETS adcli
  RUNTIME DESTINATION bin
)


