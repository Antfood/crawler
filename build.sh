if [ "$1" == "--build" ]; then
  cd build; cmake ../; make; cd .. ; ./build/crawler
fi

if [ "$1" == "--run" ]; then
  cd build; make; cd .. ; ./build/crawler
fi
