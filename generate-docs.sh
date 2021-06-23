#!/bin/bash

set -ex

cd "$( dirname "${BASH_SOURCE[0]}")"

mkdir tmp || true

body=""
for f in $(find examples/ -maxdepth 1 -type f | sort)
do
  export source=$(basename $f)
  export code=$(cat $f | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'"'"'/\&#39;/g')

  g++ -g -I./src/ -I../wave_generator/src/  ../wave_generator/src/wave-generator.cpp ./src/melody.cpp $f -o tmp/${source}
  ./tmp/${source} docs/${source}.wav > tmp/${source}.data
  gnuplot -e "set terminal png size 2500,250; set output 'docs/${source}.png'; plot 'tmp/${source}.data' with lines"

  example_content=$(envsubst < generate-template.html)
  body="$body $example_content"
done

export body
export index=$(envsubst < generate-layout.html)
echo "$index" > "docs/index.html"

rm -Rf tmp
xdg-open "docs/index.html"
