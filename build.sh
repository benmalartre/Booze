cd Framework
make -f MakefileMacOS clean
make -f MakefileMacOS -j7
cd ../Purebasic
make -f MakefileMacOS clean
make -f MakefileMacOS -j7
cp PBAlembic.so /Users/benmalartre/Documents/RnD/PureBasic/Noodle/libs/x64/macosx/PBAlembic.so
cd ../