BUILD_DIR=debug
if [ -d $BUILD_DIR ]; then
  $BUILD_DIR/concurrency $1
else
  echo "Directory build doesn't exists."
fi