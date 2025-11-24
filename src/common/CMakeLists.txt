# common library for adelphos

add_library(ad_common SHARED
	alog.c
	crypt.c
	hash.c
	jfsm.c
	jsmn.c
	jsmn_user.c
	misc.c
	strstream.c
	trust.c
	units.c
	vecptr.c
)

target_link_libraries(ad_common PUBLIC m)
target_link_libraries(ad_common PUBLIC crypt)

install(TARGETS ad_common 
  LIBRARY DESTINATION lib
)
