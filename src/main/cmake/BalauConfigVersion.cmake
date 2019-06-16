
set(PACKAGE_VERSION 2019.6.1)

if(NOT "${PACKAGE_FIND_VERSION_COUNT}" EQUAL 3)
	set(PACKAGE_VERSION_UNSUITABLE 1)
	set(PACKAGE_VERSION_COMPATIBLE 0)
elseif("${PACKAGE_FIND_VERSION_MAJOR}" EQUAL 2019)
	if("${PACKAGE_FIND_VERSION_MINOR}" EQUAL 6)
		set(PACKAGE_VERSION_COMPATIBLE 1)

		if("${PACKAGE_FIND_VERSION_PATCH}" EQUAL 1)
			set(PACKAGE_VERSION_EXACT 1)
		endif()
	endif()
endif()
