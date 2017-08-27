#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: $0 <version>"
	exit 1
fi

version=$1
echo Deploy for version $version

dir_deploy=deploy
dir_dbsrc=$dir_deploy/RappelzRDBTool-$version-db-src
dir_dblua=$dir_deploy/RappelzRDBTool-$version-db-lua
dir_dev=$dir_deploy/RappelzRDBTool-$version-dev
dir_guibin=$dir_deploy/RappelzRDBToolQt-$version

rm -r $dir_dbsrc $dir_dblua $dir_dev $dir_guibin
rm $dir_dbsrc.zip $dir_dblua.zip $dir_dev.zip $dir_guibin.zip
mkdir -p $dir_dbsrc $dir_dblua $dir_dev $dir_guibin

pushd gui && lupdate RappelzRDBToolQt.pro ; popd

cp Databases/*.cpp $dir_dbsrc/
cp Databases/lua/*.lua $dir_dblua/
cp Base/I*.h Base/DataType.h Base/ExportDLL.h Base/Guid.h Base/SpecialDatabaseRules.h bin/RpzRdbBase.lib $dir_dev/
cp bin/RappelzRDBToolQt.exe bin/RpzRdbBase.dll bin/*Database.dll gui/RappelzRDBToolQt_sample.ts $dir_guibin/

pushd $dir_dev && zip ../../$dir_dev.zip * && popd
pushd $dir_dbsrc && zip ../../$dir_dbsrc.zip * && popd
pushd $dir_dblua && zip ../../$dir_dblua.zip * && popd
pushd $dir_guibin && zip ../../$dir_guibin.zip * && popd

code_version=`grep setApplicationVersion gui/main.cpp | sed -r 's/.*setApplicationVersion\("(.*)"\).*/\1/g'`
if [ "$code_version" != "$version" ]; then
	echo Version mismatch: main.cpp contains $code_version, deploying for version $version
fi
