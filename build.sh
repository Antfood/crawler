if [ "$1" == "--build" ]; then
  mkdir -p build
  cd build; cmake ../; make; cd .. 
fi

if [ "$1" == "--run" ]; then
  cd build; make; cd .. ; ./build/crawler
fi

if [ "$1" == "--release" ]; then
  cd build; make; cd .. ; 
  cp build/crawler "./releases/"
fi
