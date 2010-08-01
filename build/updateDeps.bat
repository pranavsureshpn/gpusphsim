echo Sample > exclude.txt
echo Playpen >> exclude.txt
xcopy ..\..\build-ogre\ogre-svn-1.7-install_x64\bin\release\*.dll Win_64_Release /Y /EXCLUDE:exclude.txt
xcopy ..\..\build-ogre\ogre-svn-1.7-install_x64\bin\debug\*.dll Win_64_Debug /Y /EXCLUDE:exclude.txt
xcopy ..\..\build-ogre\ogre-svn-1.7-install_x64\bin\debug\*.dll Win_64_EmuDebug /Y /EXCLUDE:exclude.txt

xcopy ..\..\build-ogre\ogre-svn-1.7-install_x32\bin\release\*.dll Win_32_Release /Y /EXCLUDE:exclude.txt
xcopy ..\..\build-ogre\ogre-svn-1.7-install_x32\bin\debug\*.dll Win_32_Debug /Y /EXCLUDE:exclude.txt
xcopy ..\..\build-ogre\ogre-svn-1.7-install_x32\bin\debug\*.dll Win_32_EmuDebug /Y /EXCLUDE:exclude.txt
del exclude.txt


xcopy /Y ..\..\build-ogre\mygui-trunk-build_x64\sdk\bin\Debug\*.dll Win_64_EmuDebug
xcopy /Y ..\..\build-ogre\mygui-trunk-build_x64\sdk\bin\Debug\*.dll Win_64_Debug
xcopy /Y ..\..\build-ogre\mygui-trunk-build_x64\sdk\bin\Release\*.dll Win_64_Release