
add_library(adelphos SHARED
	adelphos.c
	addb.c
	json_api.c
)

target_include_directories(adelphos PUBLIC ../common)
target_link_libraries(adelphos PUBLIC ad_common)
target_link_libraries(adelphos PUBLIC sqlite3)

install(TARGETS adelphos
  LIBRARY DESTINATION lib
)
