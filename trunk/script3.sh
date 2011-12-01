#!/bin/bash
make -j
for i in -1.36136 -1.04720 -0.73304 -0.41888 -0.10472 0.20944 0.52360 0.83776 1.15192 1.46608


do
	echo "using t = " $i
	./bin/ray_tracer $i -a
done
