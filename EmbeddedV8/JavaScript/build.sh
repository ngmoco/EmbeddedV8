#!/bin/bash

closure-library/closure/bin/calcdeps.py -i Main.js -p src/ -p closure-library/ -o script > gen_out.js
java -jar compiler.jar --js gen_out.js --compilation_level=ADVANCED_OPTIMIZATIONS --externs externs.js > Compiled.js
rm gen_out.js
