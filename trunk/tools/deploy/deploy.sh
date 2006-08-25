cd ../deploy

svn co https://svn.sourceforge.net/svnroot/sagaengine/trunk sagaengine
cd sagaengine
rm -Rf test
rm -Rf samples/navmesh
rm -Rf `find -name ".svn"`
cd core
rm `find -name "vc"`
cd ../add_on
rm -Rf samples/angelscript
rm -Rf samples/ncurs
rm -Rf samples/fmod
cd ../..

tar -zcvf sagaengine-$1.tgz sagaengine

cd sagaengine
unix2dos `find -name "*.cpp"`
unix2dos `find -name "*.hpp"`
unix2dos `find -name "*.txt"`

zip -r ../sagaengine-$1.zip *
