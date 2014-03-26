version=$1
echo Deploy for version $version

dir_deploy=deploy
dir_dbsrc=$dir_deploy/RappelzRDBTool-$version-db-src
dir_dev=$dir_deploy/RappelzRDBTool-$version-dev
dir_guibin=$dir_deploy/RappelzRDBToolQt-$version

rm -r $dir_deploy
mkdir -p $dir_dbsrc $dir_dev $dir_guibin

pushd gui && lupdate RappelzRDBToolQt.pro && popd

cp Databases/*.cpp $dir_dbsrc/
cp Base/I*.h Base/DataType.h Base/ExportDLL.h Base/Guid.h Base/SpecialDatabaseRules.h bin/RpzRdbBase.lib $dir_dev/
cp bin/RappelzRDBToolQt.exe bin/RpzRdbBase.dll bin/*Database.dll gui/RappelzRDBToolQt_sample.ts $dir_guibin/

pushd $dir_dev && zip ../../$dir_dev.zip * && popd
pushd $dir_dbsrc && zip ../../$dir_dbsrc.zip * && popd
pushd $dir_guibin && zip ../../$dir_guibin.zip * && popd
