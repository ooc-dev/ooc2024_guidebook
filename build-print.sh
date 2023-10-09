#!/bin/sh
echo [Guidebook 2]
echo Building Guidebook2.pdf into src directory...
docker run --rm -v $PWD/src:/work vvakame/review:3.1 /bin/sh -c "cd /work && review-pdfmaker config-print.yml"
