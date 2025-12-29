BUILD_DIR=build
if [ -d $BUILD_DIR ]; then
  $BUILD_DIR/test $1
else
  echo "Directory build doesn't exists."
fi