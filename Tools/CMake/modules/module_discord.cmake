# -----------------------------------------------------------------------------
# Copyright (c) 2021 Draconic Entity
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
# -----------------------------------------------------------------------------

option(TORQUE_DISCORD_ENABLED "Enable Discord module" OFF)
if(TORQUE_DISCORD_ENABLED)
	addPathRec( "${srcDir}/Discord/Discord" )
	addPathRec( "${srcDir}/Discord/Binding" )
	addInclude( "${srcDir}/Discord" )

	set(TORQUE_DISCORD_APPLICATION_ID "" CACHE STRING "Discord Application ID")
	addDef( "TORQUE_DISCORD_APPLICATION_ID" ${TORQUE_DISCORD_APPLICATION_ID} )

	# Libs
    link_directories( "${srcDir}/Discord/Lib/x86_64" )
	if( WIN32 )
		addLib( "discord_game_sdk.dll.lib" )

		# File Copy for Release
		INSTALL(FILES "${srcDir}/Discord/Lib/x86_64/discord_game_sdk.dll" DESTINATION "${projectOutDir}")
    elseif(UNIX AND NOT APPLE)
        addLib( "discord_game_sdk" )

        # File Copy for Release
        INSTALL(FILES "${srcDir}/Discord/Lib/x86_64/discord_game_sdk.so" DESTINATION "${projectOutDir}")
	endif()
endif()
