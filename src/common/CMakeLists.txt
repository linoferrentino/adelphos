# common library for adelphos

add_library(ad_common SHARED
	trust.c
	vecptr.c
	misc.c
	alog.c
	crypt.c
)

target_link_libraries(ad_common PUBLIC m)
target_link_libraries(ad_common PUBLIC crypt)

install(TARGETS ad_common 
  LIBRARY DESTINATION lib
)
