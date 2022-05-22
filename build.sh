if [ "$1" == "--build" ]; then
  cd build; cmake ../; make; cd .. 
fi

if [ "$1" == "--run" ]; then
  cd build; make; cd .. ; ./build/crawler
fi

if [ "$1" == "--release" ]; then
  cd build; make; cd .. ; 
  cp build/crawler "/Users/pedro/Dropbox (Antfood, LLC)/ANTFOOD WORLDWIDE/crawler"
fi
