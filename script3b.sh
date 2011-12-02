#!/bin/bash
make -j
for i in -0.41888 0.20944 0.83776 1.46608


do
	echo "using t = " $i
	./bin/ray_tracer $i -a
done
