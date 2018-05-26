#!/bin/sh
echo `date '+%T %H %MM %SS'`
echo millis(){  python -c "import time; print(int(time.time()*1000))"; }

`python nrooks-2.py 3 > outputLog.log`
millis(){  python -c "import time; print(int(time.time()*1000))"; }

echo `date '+%T %H %MM %SS'`
